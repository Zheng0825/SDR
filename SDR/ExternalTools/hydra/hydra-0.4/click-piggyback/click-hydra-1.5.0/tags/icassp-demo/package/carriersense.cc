/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/carriersense.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/packet.hh>
#include <click/straccum.hh>
//#include <click/standard/scheduleinfo.hh>

#include <math.h>
#include <click/hydra_anno.hh>
#include "carriersense.hh"
CLICK_DECLS

CarrierSense::CarrierSense() 
  : _cs_threshold(0.0), _rx_state(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

CarrierSense::~CarrierSense()
{
}

int
CarrierSense::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int cs_threshold_db = 0; 

  Element* e = 0;
  int res = cp_va_parse(conf, this, errh,
			cpReal10, "carrier sense threshold dB", 3, &cs_threshold_db,
			cpOptional,
			cpKeywords,
			"RXSTATE", cpElement, "rx state", &e,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  if((float)cs_threshold_db > (float)-100.0)
  {
    _cs_threshold = pow10((((float)cs_threshold_db)/10));
  }

  if(e)
  {
    _rx_state = (RXState *)e->cast("RXState");
    if(!_rx_state)
      return errh->error("RXSTATE must be set to the RXState element");
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
CarrierSense::initialize(ErrorHandler *) 
{
  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  _cs_states.clear();
  CSState cs = CS_IDLE;
  for(int i=0; i<ninputs(); i++)
    _cs_states.push_back(cs); 

  return 0;
}

// FIXME: added generate HYDRA_CCA_EIDLE and process HYDRA_CCA_EIDLE
void
CarrierSense::push(int port, Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::push:", this);

  assert(p);

  CSState pstate = current_state();

  struct click_hydra_anno *cha = (struct click_hydra_anno *) p->all_user_anno();
  if( cha->type == HYDRA_CCA_BUSY)
  {
    _cs_states[port] = CS_BUSY; 
  }
  else if (cha->type == HYDRA_CCA_IDLE)
  { 
    _cs_states[port] = CS_IDLE; 
  }
  else
  {
     hydra_chatter(0, _verbose, "%{element}::push: invalid packet", this);
     p->kill();
     return;
  }

  CSState cstate = current_state();
  
  if( (pstate == CS_IDLE) && (cstate == CS_BUSY) )
  {
    hydra_chatter(8, _verbose, "%{element}::push: push busy ", this);
    output(0).push(p);
  }
  else if( (pstate == CS_BUSY) && (cstate == CS_IDLE) )
  {
      hydra_chatter(8, _verbose, "%{element}::push: push idle ", this);
     //if( _rx_state && !_rx_state->state() )
     //   cha->type = HYDRA_CCA_EIDLE;
     output(0).push(p); 
  }
  else
  {
    hydra_chatter(8, _verbose, "%{element}::push: no case type = %d", this, cha->type);
    p->kill();
  }

  return;
}


CarrierSense::CSState 
CarrierSense::current_state()
{
  for(int i=0; i<_cs_states.size(); i++)
  {
    if(_cs_states[i] == CS_BUSY)
      return CS_BUSY;
  }

  return CS_IDLE;
}

enum {H_VERBOSE};

String
CarrierSense::read_param(Element *e, void *vparam)
{
  CarrierSense *tp = (CarrierSense *)e;
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
      return String(tp->_verbose);
    default:
      return "";
  }

  return String();
}

int
CarrierSense::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  CarrierSense *tp = (CarrierSense *)e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      tp->_verbose = verbose;
      break; 
    }
    default:
      break;
  }

  return 0;
}

void
CarrierSense::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

EXPORT_ELEMENT(CarrierSense)

#include <click/vector.cc>
template class Vector<CarrierSense::CSState>;
CLICK_ENDDECLS
