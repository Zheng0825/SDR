/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/ratecontrol.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/packet_anno.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>

#include "ratecontrol.hh"

CLICK_DECLS

RateControl::RateControl()
  : _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RateControl::~RateControl()
{
}

void*
RateControl::cast(const char *n)
{
  if( strcmp(n, "RateControl") == 0 )
    return (RateControl *) this;
  else
    return 0;
}

int
RateControl::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }


  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
RateControl::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  //struct click_hydra_anno* cha = (struct click_hydra_anno *) p_in->all_user_anno();
  // FIXME: can I use Vector in click annotation?
  //cha->power_vector.clear();
  // do feedback algoithm here
  // and forward
  return p_in;
}

WritablePacket*
RateControl::get_txrate(Packet* p_in)
{
   
  //struct click_hydra_anno* cha = (struct click_hydra_anno *) p_in->all_user_anno();
  // FIXME: can I use Vector in click annotation?
  //cha->rate_vector.clear();

  WritablePacket* p_out = p_in->uniqueify();

  return p_out;
}

enum {H_VERBOSE};

String
RateControl::read_handler(Element *e, void *thunk)
{
  RateControl *src = (RateControl *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
RateControl::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  RateControl *src = (RateControl *) e;
  String s = cp_uncomment(arg);
  switch((int)vparam) {
  case H_VERBOSE: {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("verbose parameter must be unsigned");
    src->_verbose = m;
    break;
  }
  }
  return 0;
}

void
RateControl::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RateControl)

