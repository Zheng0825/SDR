/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
//#include <click/packet_anno.hh>
//#include <clicknet/ether.h>
//#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "ratecontrolarf.hh"

CLICK_DECLS

RateControlArf::RateControlArf()
  : _timer(this), 
    _rate(6), _num_successes(10), _num_fails(2), 
    _cur_successes(0), _cur_fails(0), _first_try(true)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RateControlArf::~RateControlArf()
{
}

void*
RateControlArf::cast(const char *n)
{
  if( strcmp(n, "RateControlArf") == 0 )
    return (RateControlArf *) this;
  else
    return RateControl::cast(n);
}

int
RateControlArf::configure(Vector<String> &conf, ErrorHandler *errh)
{
  unsigned timeout_us = 0;
  if (cp_va_parse(conf, this, errh,
		  cpUnsigned, "Initial rate", &_rate, 
		  cpUnsigned, "Number of consectutive successes", &_num_successes, 
		  cpUnsigned, "Number of consectutive fails", &_num_fails, 
		  cpSecondsAsMicro, "timeout time", &timeout_us, 
		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }

  _timeout_val.tv_sec = timeout_us/1000000;
  _timeout_val.tv_usec = timeout_us%1000000;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int 
RateControlArf::initialize(ErrorHandler*)
{
  _timer.initialize(this);

  return 0;
}

Packet *
RateControlArf::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  // Nothing to do for ARF

  return p_in;
}

WritablePacket*
RateControlArf::get_txrate(Packet* p_in)
{
  hydra_chatter(8, _verbose, "%{element}::get_txrate: ", this);

  WritablePacket* p_out = p_in->uniqueify(); 

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();

  cha->rate_mbps = (uint16_t) _rate;

  return p_out;

}


void 
RateControlArf::success()
{
  _cur_fails = 0;
  _first_try = false;

  //printf("num_success = %d\n", _cur_successes +1);
  if(++_cur_successes == _num_successes)
  {
    increase_rate();
    _cur_successes = 0;
  }
}

void 
RateControlArf::fail()
{
  _cur_successes = 0;

  if((++_cur_fails == _num_fails) || _first_try)
  {
    decrease_rate();
    _cur_fails = 0;
  }
}

void RateControlArf::run_timer(Timer* )
{
  _cur_successes = _num_successes -1;
  success();
}

void 
RateControlArf::increase_rate()
{
  uint8_t rate_index = rate_to_index(_rate);

  rate_index++;

  if(rate_index == NUM_ARF_RATE)
    rate_index--;

  _rate = index_to_rate(rate_index); 
  printf("ARF: rate %02d\n", _rate);

  _first_try = true;
  if(_timer.scheduled())
    _timer.unschedule();

}

void 
RateControlArf::decrease_rate()
{
  uint8_t rate_index = rate_to_index(_rate);
  
  if(rate_index == 0)
    return;

  rate_index--;

  _rate = index_to_rate(rate_index);
  printf("ARF: rate %02d\n", _rate);

  if(_timer.scheduled())
    _timer.unschedule();

  _timer.schedule_after(_timeout_val);
}

uint8_t RateControlArf::rate_to_index(uint16_t anno_rate)
{
  uint8_t index = ARF_6M;

  switch(anno_rate)
  {
    case  6: index = ARF_6M;  break;
    case  9: index = ARF_9M;  break;
    case 12: index = ARF_12M; break;
    case 18: index = ARF_18M; break;
    case 24: index = ARF_24M; break;
    case 36: index = ARF_36M; break;
    case 48: index = ARF_48M; break;
    case 54: index = ARF_54M; break;
  }

  return index;

}

uint16_t RateControlArf::index_to_rate(uint8_t index)
{
  uint8_t rate = 0;

  switch(index)
  {
    case ARF_6M:  rate =  6; break;
    case ARF_9M:  rate =  9; break;
    case ARF_12M: rate = 12; break;
    case ARF_18M: rate = 18; break;
    case ARF_24M: rate = 24; break;
    case ARF_36M: rate = 36; break;
    case ARF_48M: rate = 48; break;
    case ARF_54M: rate = 54; break;
  }

  return rate;

}

enum {H_VERBOSE};

String
RateControlArf::read_handler(Element *e, void *thunk)
{
  RateControlArf *src = (RateControlArf *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
RateControlArf::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  RateControlArf *src = (RateControlArf *) e;
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
RateControlArf::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RateControlArf)

