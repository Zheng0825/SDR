/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/virtualcarriersense.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>

#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "virtualcarriersense.hh"

CLICK_DECLS

VirtualCarrierSense::VirtualCarrierSense() 
  : _cs_state(CS_IDLE), _timer(this), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

VirtualCarrierSense::~VirtualCarrierSense()
{
}

int
VirtualCarrierSense::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return -1;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
VirtualCarrierSense::initialize(ErrorHandler *) 
{
  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  _timer.initialize(this);

  return 0;
}

void
VirtualCarrierSense::push(int, Packet *p)
{

  assert(p);


  struct common_80211* common_hdr = (struct common_80211 *) p->data();

#ifndef IEEE80211_LONG_TIMER
  uint16_t duration = *(uint16_t *)common_hdr->i_dur;
  
  // duration is 0-32767, so nothing is done for NAV
  // It is ID for CFP and PS-POLL
  // maximum duration is about 33ms
  if( duration > 0x7fff || duration == 0)
  {
    goto end;
  }
#else
  uint32_t duration = *(uint32_t *)common_hdr->i_dur;
  hydra_chatter(8, _verbose, "%{element}::push: nav = %d.%06d\n", this, duration/1000000, duration%1000000);
  
  if( duration == 0)
  {
    goto end;
  }
#endif

  struct timeval now;
  click_gettimeofday(&now);

  if(_cs_state == CS_IDLE)
  {
    assert(!_timer.busy());

    _cs_state = CS_BUSY;
    _timer.start_timer(duration, now);
    Packet* p_out = prepare_packet(HYDRA_CCA_BUSY);
    output(0).push(p_out);
  }
  else if(_cs_state == CS_BUSY)  
  {
    _timer.pause_timer();

#ifndef IEEE80211_LONG_TIMER
    if( duration > _timer.remains().tv_usec )
#else
    /*
    if( ((duration/1000000) > (unsigned int)_timer.remains().tv_sec)  &&
        ((duration%1000000) > (unsigned int)_timer.remains().tv_usec) )
    */
    if( ( (duration/1000000) > (unsigned int)_timer.remains().tv_sec ) || 
        ( (duration/1000000 == (unsigned int)_timer.remains().tv_sec) && 
          (duration%1000000 > (unsigned int)_timer.remains().tv_usec) ) )
#endif
    {
      _timer.stop_timer();
      _timer.start_timer(duration, now);
    }
    else
      _timer.resume_timer();
  }

end:
  if(noutputs() > 1)
    output(1).push(p);
  else
    p->kill();

  return;
}



void
VirtualCarrierSense::run_timer(Timer* )
{
  hydra_chatter(8, _verbose, "%{element}::run_timer: ", this);
  assert(_cs_state == CS_BUSY);

  _cs_state = CS_IDLE;

  Packet* p_out = prepare_packet(HYDRA_CCA_IDLE);

  output(0).push(p_out);
}

Packet*
VirtualCarrierSense::prepare_packet(uint8_t type)
{
  Packet* p_out = Packet::make(HYDRA_HEADROOM, 0, 0, HYDRA_TAILROOM); 
  struct click_hydra_anno *cha = (struct click_hydra_anno *) p_out->all_user_anno();
  cha->type = type;

/*
  if(type == HYDRA_CCA_BUSY)
  {
    struct click_hydra_power chp;
    chp.channel_number = HYDRA_ALL_CHANNEL;
    chp.rx_ssi = 0xff;  //Maximum recieve strength 
   
    //cha->power_vector.push_back(chp); 
  }
*/
  return p_out;
}

enum{H_VERBOSE};

String
VirtualCarrierSense::read_param(Element *e, void *vparam)
{
  VirtualCarrierSense *tp = (VirtualCarrierSense *)e;
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
      return cp_unparse_bool(tp->_verbose);
    default:
      break;
  }
  return String();
}

int
VirtualCarrierSense::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  VirtualCarrierSense *tp = (VirtualCarrierSense *)e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
    {
      bool verbose;
      if(!cp_bool(s, &verbose))
	return errh->error("verbose must be boolean");
      tp->_verbose = verbose;
      break; 
    }
  }
  return 0;
}

void
VirtualCarrierSense::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}


CLICK_ENDDECLS
EXPORT_ELEMENT(VirtualCarrierSense)
