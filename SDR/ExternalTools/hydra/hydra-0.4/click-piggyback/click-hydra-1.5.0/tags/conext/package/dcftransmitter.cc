/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/dcftransmitter.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/straccum.hh>
//#include <clicknet/ether.h>

#include <clicknet/80211.h>
#include <click/hydra_anno.hh>

#include "dcftransmitter.hh"
#include "setdefer.hh"

CLICK_DECLS

DCFTransmitter::DCFTransmitter() 
  : _tx_state(IDLE), _waiting_packet(0),
    _trigger_defer_timer(run_trigger_defer_timer, this),
    _response_defer_timer(run_response_defer_timer, this),
    _backoff_timer(run_backoff_timer, this),
    _slot_time(0),
    _control_state(CS_IDLE)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

DCFTransmitter::~DCFTransmitter()
{
}

void*
DCFTransmitter::cast(const char *n)
{
  if( strcmp(n, "DCFTransmitter") == 0 )
    return (DCFTransmitter *) this;

  return MACTransmitter::cast(n);
}


int
DCFTransmitter::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* e_notifier = 0;
  Element* e_slottime = 0;
  int res = cp_va_parse(conf, this, errh,
 			cpOptional,
			cpKeywords,
			"NOTIFIER", cpElement, "mac notifier", &e_notifier,
			"DELAY", cpSecondsAsMicro, "timeout for delay", &_delay,
			"SLOTTIME", cpElement, "a slot time for backoff", &e_slottime,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if(res < 0)
    return res;

  if(e_notifier)
  {
    _tx_notifier = (MACNotifier *) e_notifier->cast("MACNotifier");
    if(!_tx_notifier)
      return errh->error("NOTIFIER must be set using MACNotifier");
  }

  // now, DCFTransmitter do not use _tx_notifier
  //_tx_notifier = 0;

  if(e_slottime)
  {
    SetDefer* defer = (SetDefer *) e_slottime->cast("SetDefer");
    if(!defer)
      return errh->error("SLOTTIME must be set using SetDefer");

    _slot_time = defer->slot_time();
  }
  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}


int
DCFTransmitter::initialize(ErrorHandler *errh) 
{

  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  _trigger_defer_timer.initialize(this);
  _response_defer_timer.initialize(this);
  _backoff_timer.initialize(this);

  ScheduleInfo::join_scheduler(this, &_task, errh);

  _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);
  if(_tx_notifier)
  {
    _tx_notifier->add_listener( (MACListener *) this);
  }

  _nonfull_signal = Notifier::downstream_full_signal(this, 0, &_task);

  // Why this initialization occurrs in configure() of fullnoteqeueue.cc?
  _nonfull_notifier.initialize(router());
  _nonfull_notifier.set_active(true);
  //check();
  return 0;
}


void
DCFTransmitter::push(int port, Packet *p)
{
  struct timeval tv;
  click_gettimeofday(&tv);
  hydra_chatter(8, _verbose, "%{element}::push: %d.%06d", this, tv.tv_sec, tv.tv_usec);

  // control port for feedback or scheduling info
  assert(port != 0);
  //check();

  if (is_control(port, p))
    process_control(port, p);
  else
    process_response(port, p); 
  
  //check();
}

bool
DCFTransmitter::run_task()
{
  hydra_chatter(9, _verbose, "%{element}::run_task: ", this);

  // if there is nothing to do, go to sleep
  if(!_nonempty_signal && !_nonfull_signal)
  {
    hydra_chatter(4, _verbose, "%{element}: sleep due to no signal", this);
    return false;
  }

  if(!is_ctrlstate(CS_IDLE))
  {
    hydra_chatter(8, _verbose, "%{element}: sleep due to CS_BUSY", this);
    return false;
  }

  // first check if there is packet to push, 
  // assuming that only one packet can be sent
  if(_waiting_packet && 
     (is_txstate(BACKOFF_DEFER) || is_txstate(RESPONSE_DEFER)) )
  {

    if(_nonfull_signal) 
    {
      push_packet(0, _waiting_packet);   
      _waiting_packet = 0;
    }

    // if _waiting_packet != 0, then nonfull signal wakes it up later.
    return true;
  }

 
  if(!is_txstate(IDLE))
  {
    hydra_chatter(8, _verbose, "%{element}::run_task: sleep due to non IDLE", this);
    return false;
  }

  Packet *p = input(0).pull();

  if(p)
  {
    hydra_chatter(8, _verbose, "%{element}::run_task: trigger pulled", this);
    // FIXME: sleep only listeners connected to input(0)
    _nonfull_notifier.sleep();
    _trigger_packet = p; 
    
    start_trigger_defer_timer(_trigger_packet);
  }
  else if (!_nonempty_signal)
  {
    // upstream is empty
    // so, sleep until new packet is arrived in port 0 or downstream queue is not full
    hydra_chatter(4, _verbose, "%{element}: sleep due to no nonempty signal", this);
    return false;
  }

  _task.fast_reschedule();
  return true;
}

bool
DCFTransmitter::start_trigger_defer_timer(Packet* p)
{
  struct timeval cur;
  click_gettimeofday(&cur);

  hydra_chatter(8, _verbose, "%{element}::start_trigger_defer_timer: %d.%06d", this, cur.tv_sec, cur.tv_usec);
  //assert(!_waiting_packet);
  //assert(!is_txstate(TRIGGER_DEFER));
  assert(!_trigger_defer_timer.busy());

  struct click_hydra_anno* cha = 
    (struct click_hydra_anno *) p->all_user_anno();
  struct timeval tv = p->timestamp_anno().timeval(); 

  set_txstate(TRIGGER_DEFER);

  return  _trigger_defer_timer.start_timer(cha->defer_usec, tv);
}

bool
DCFTransmitter::start_response_defer_timer(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::start_response_defer_timer:", this);
  assert(!_response_defer_timer.busy());
  //assert(!_waiting_packet);
  //assert(is_txstate(IDLE) || is_txstate(TRIGGER_DEFER));

  struct click_hydra_anno* cha = 
    (struct click_hydra_anno *)p->all_user_anno();
  struct timeval tv = p->timestamp_anno().timeval(); 

  set_txstate(RESPONSE_DEFER);

  hydra_chatter(8, _verbose, "%{element}::start_response_defer_timer: %d", this, cha->defer_usec);

  return  _response_defer_timer.start_timer(cha->defer_usec, tv);
}

bool
DCFTransmitter::start_backoff_timer(Packet* p)
{
  struct timeval cur;
  click_gettimeofday(&cur);

  hydra_chatter(8, _verbose, "%{element}::start_backoff_timer: %d.%06d", this, cur.tv_sec, cur.tv_usec);

  assert(!_backoff_timer.busy());
  //assert(is_txstate(TRIGGER_DEFER));

  set_txstate(BACKOFF_DEFER);

  if(_backoff_timer.paused())
  {
    hydra_chatter(8, _verbose, "%{element}::start_backoff_timer: resume backoff timer remain = %d.%06d", this, _backoff_timer.remains().tv_sec, _backoff_timer.remains().tv_usec );
    bool is_now =  _backoff_timer.resume_timer(_slot_time);
    hydra_chatter(8, _verbose, "%{element}::start_backoff_timer: resume backoff timer slotted remain = %d.%06d", this, _backoff_timer.remains().tv_sec, _backoff_timer.remains().tv_usec );
    return is_now;
  }

  struct click_hydra_anno* cha = 
    (struct click_hydra_anno *)p->all_user_anno();
  struct timeval tv = p->timestamp_anno().timeval(); 
 
  hydra_chatter(8, _verbose, "%{element}::start_backoff_timer: new start %d", this, cha->slot);
  return _backoff_timer.start_timer(cha->slot, tv);
}

void
DCFTransmitter::run_trigger_defer_timer(Timer* , void* thunk)
{
  DCFTransmitter * dcf = (DCFTransmitter *) thunk;

  hydra_chatter(8, dcf->_verbose, "%{element}::run_trigger_defer_timer:", dcf);

  assert(!dcf->_trigger_defer_timer.busy());
  assert( dcf->is_txstate(TRIGGER_DEFER) );
  assert( dcf->is_ctrlstate(CS_IDLE) );

  // if already backoffed full slot
  if(dcf->_waiting_trigger_packet)
  { 
    dcf->set_txstate(BACKOFF_DEFER);
    dcf->_waiting_packet = dcf->_waiting_trigger_packet;
    dcf->_waiting_trigger_packet = 0;
    dcf->_task.reschedule();
    return;
  }  

  dcf->start_backoff_timer(dcf->_trigger_packet); 

  return;
}

void
DCFTransmitter::run_response_defer_timer(Timer* , void* thunk)
{
  struct timeval tv;
  click_gettimeofday(&tv);

  DCFTransmitter * dcf = (DCFTransmitter *) thunk;
  assert(!dcf->_response_defer_timer.busy());
  assert( dcf->is_txstate( RESPONSE_DEFER));
  // FIXME: do we need to carrier sense before sending response?
  //assert( dcf->is_ctrlstate(CS_IDLE) );
  hydra_chatter(8, dcf->_verbose, "%{element}::run_response_defer_timer: %d.%06d", dcf, tv.tv_sec, tv.tv_usec);

  //hydra_chatter(8, dcf->_verbose, "%{element}::run_response_defer_timer:", dcf);

  dcf->_waiting_packet = dcf->_response_packet;

  if(dcf->_nonfull_signal) 
    dcf->push_packet(0, dcf->_waiting_packet);  

  //check();
}

void
DCFTransmitter::run_backoff_timer(Timer* , void* thunk)
{

  DCFTransmitter * dcf = (DCFTransmitter *) thunk;
  assert(!dcf->_backoff_timer.busy());
  assert( dcf->is_txstate(BACKOFF_DEFER) );
  assert( dcf->is_ctrlstate(CS_IDLE) );

  struct timeval cur;
  click_gettimeofday(&cur);

  hydra_chatter(8, dcf->_verbose, "%{element}::run_backoff_timer: %d.%06d", dcf, cur.tv_sec, cur.tv_usec);
  // non zero _waiting packet means the end of deferring
  // although the state is not changed

  dcf->_waiting_packet = dcf->_trigger_packet;

  if(dcf->_nonfull_signal) 
    dcf->push_packet(0, dcf->_trigger_packet);   
    
  //check();
}

void
DCFTransmitter::push_packet(int port, Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::push_packet:", this);

  if( is_txstate(BACKOFF_DEFER) ) 
    set_txstate(TRIGGER_TRANSMITTING);
  else if( is_txstate(RESPONSE_DEFER) ) 
    set_txstate(RESPONSE_TRANSMITTING);

  //struct click_hydra_anno* debug = (struct click_hydra_anno*) p->all_user_anno();
  //printf("dcftransmitter: 0x%x\n", debug->mac_type);

  output(port).push(p); 

  if(!_tx_notifier)
  {
    MACListener::MACEvent e = MACListener::TX_DONE;
    notified(0, e, p); 
  }
}

void 
DCFTransmitter::notified(uint32_t , MACEvent e, const Packet *)
{
  hydra_chatter(8, _verbose, "%{element}::notified:", this);

  //assert(is_txstate(RESPONSE_TRANSMITTING) || is_txstate(TRIGGER_TRANSMITTING));
  // currently, this is only event.
  assert(e == MACListener::TX_DONE || e == MACListener::TX_CHANGE);

  _waiting_packet = 0;

  if(is_txstate(TRIGGER_TRANSMITTING))
  {
    _trigger_packet = 0; 
  }
  else if(is_txstate(RESPONSE_TRANSMITTING))
  {
    _response_packet = 0; 
  }

  set_txstate(IDLE);

  // resume trigger packet
  // we assume that two responses packet is not possible
  // check again

  if(_trigger_packet)
  {
    if(is_ctrlstate(CS_IDLE)) 
      start_trigger_defer_timer(_trigger_packet);
    else
      set_txstate(TRIGGER_DEFER); 
  }
  else 
  {
    _nonfull_notifier.wake();
    _task.reschedule(); 
  }

}

bool
DCFTransmitter::is_control(int port, Packet* p)
{
  assert(p && port != 0);

  // I think it's better to decide the control packet using port number
  // than using the type in hydra annotation,
  // since port is local information.
  if(port >= 2)
    return true;

  return false;
}


void
DCFTransmitter::process_response(int port, Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::process_response:", this);

  assert(port == 1);

  // already processing response packet, so drops later one.
  // FIXME: do we need to drop response when trigger packet waits for transmitting?
  //if(_response_packet || _trigger_packet)
  if(_tx_state == TRIGGER_TRANSMITTING || 
     _tx_state == RESPONSE_DEFER || 
     _tx_state == RESPONSE_TRANSMITTING)
  {
    hydra_chatter(0, _verbose, "%{element}::process_response:  already processing the other packet", this);
    p->kill();
    return;
  }

  // strict checking
  struct common_80211* cmm_hdr = (struct common_80211 *) p->data();
  
  if( !(is_cts(cmm_hdr->i_fc)  || 
        (is_unicast(*cmm_hdr)) || 
        is_ack(cmm_hdr->i_fc)) )
  {
    //wonsoo +3
    // exceptional case: aggregation with unicast/broadcast frame
    struct click_hydra_anno* cha =
                 (struct click_hydra_anno *) p->all_user_anno();
    if(!cha->agg_anno->has_unicast) 
    {   
      hydra_chatter(1, _verbose, "%{element}::process_response: got wrong packet", this);
      p->kill();
      return;
    }
  }
 
  _response_packet = p;

  switch( _tx_state )
  {
    case IDLE:
      break;
    // timers related trigger should be canceled or paused by process_cs_busy
    case TRIGGER_DEFER:
    {
      if(_trigger_defer_timer.busy()) 
        _trigger_defer_timer.stop_timer();
 
      break;
    }
    case BACKOFF_DEFER:
    {
      if(_backoff_timer.busy()) 
      {
        _backoff_timer.pause_timer();
        hydra_chatter(8, _verbose, "%{element}::process_response: pause backoff timer remain = %d.%06d", this, _backoff_timer.remains().tv_sec, _backoff_timer.remains().tv_usec );
      }
      else if(_waiting_packet)
      {
        // this case can be considered as collision
        // using the same time slot
        //_trigger_packet->kill(); _trigger_packet = 0;
        _waiting_trigger_packet = _waiting_packet;
        _waiting_packet = 0; 
      }
      break;
    }
    // if currently trigger packet is transmitting.
    // new response packet should be sent after finishing the previous one.
    // it should be handled by notified()
    // FIXME:
    case TRIGGER_TRANSMITTING:
      //return;
    case RESPONSE_DEFER:
    case RESPONSE_TRANSMITTING:
    default:
    {
      hydra_chatter(0, _verbose, "%{element}::process_response: never this case", this);
      return;
    }
      
  }

  start_response_defer_timer(_response_packet);
}

void
DCFTransmitter::process_control(int port, Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::process_control:", this);

  assert(port>=2);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();

  // strict checking the type
  if( !(cha->type == HYDRA_CCA_IDLE ||
        cha->type == HYDRA_CCA_EIDLE ||
        cha->type == HYDRA_CCA_BUSY) )
   {
      hydra_chatter(0, _verbose, "%{element}::process_control: wrong packet", this);
      p->kill();
      return;
   }   
 
  if( cha->type == HYDRA_CCA_BUSY) 
    process_cca_busy(p);
  else if ( cha->type == HYDRA_CCA_IDLE) 
    process_cca_idle(p);

  
  p->kill();
}


void
DCFTransmitter::process_cca_busy(Packet* )
{
  hydra_chatter(8, _verbose, "%{element}::process_cca_busy:", this);

  if(is_ctrlstate(CS_BUSY)) 
    return;

  set_ctrlstate(CS_BUSY);

  switch(_tx_state)
  {
    case TRIGGER_DEFER:
    {
      hydra_chatter(8, _verbose, "%{element}::process_cca_busy: trigger defering, so cancele", this);
      if(_trigger_defer_timer.busy()) 
        _trigger_defer_timer.stop_timer();
      else
        hydra_chatter(1, _verbose, "%{element}::process_cca_busy: never this casee", this);
      break;
    }
    case BACKOFF_DEFER:
    {
      hydra_chatter(8, _verbose, "%{element}::process_cca_busy: trigger defering, so pause", this);
      if(_backoff_timer.busy()) 
      {
        _backoff_timer.pause_timer();
        hydra_chatter(8, _verbose, "%{element}::process_cca_busy: pause backoff timer remain = %d.%06d", this, _backoff_timer.remains().tv_sec, _backoff_timer.remains().tv_usec );
      }
      else if(_waiting_packet)
      {
        // this case can be considered as collision
        // using the same time slot
        //_trigger_packet->kill(); _trigger_packet = 0;
        _waiting_trigger_packet = _waiting_packet;
        _waiting_packet = 0; 
      }
      break;
    }
    case RESPONSE_DEFER:
    {
      hydra_chatter(1, _verbose, "%{element}::process_cca_busy: this mean wrong SIFS value", this);
      break;
    }
    case IDLE:
    case TRIGGER_TRANSMITTING:
    case RESPONSE_TRANSMITTING:
    default:
      hydra_chatter(8, _verbose, "%{element}::process_cca_busy: state = %d", this, _tx_state);
      break;
  }

}

void
DCFTransmitter::process_cca_idle(Packet* )
{
  hydra_chatter(8, _verbose, "%{element}::process_cca_idle:", this);

  if(is_ctrlstate(CS_IDLE)) 
    return;

  set_ctrlstate(CS_IDLE);

  switch(_tx_state)
  {
    case IDLE:
    {
      hydra_chatter(8, _verbose, "%{element}::process_cca_idle: state = IDLE", this);
      _task.reschedule();
      break;
    }
    case TRIGGER_DEFER:
    case BACKOFF_DEFER:
    {
      hydra_chatter(8, _verbose, "%{element}::process_cca_idle: restart trigger defer", this);
      start_trigger_defer_timer(_trigger_packet);
      break;
    }
    case RESPONSE_DEFER:
    case TRIGGER_TRANSMITTING:
    case RESPONSE_TRANSMITTING:
    default:
    {
      hydra_chatter(8, _verbose, "%{element}::process_cca_idle: state = %d", this, _tx_state);
      break;
    }
  }


}

/*
void
DCFTransmitter::check()
{
  // check() should be called *before* most pushes() from element
  // functions, as each push may call back into the element.
}
*/


String
DCFTransmitter::read_param(Element *e, void *vparam)
{
  DCFTransmitter *tp = (DCFTransmitter *)e;
  switch( (intptr_t)vparam )
  {
    case 0:
      return String(tp->_verbose);
    default:
      String();
  }

  return String();
}

int
DCFTransmitter::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  DCFTransmitter *tp = (DCFTransmitter *)e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case 0:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      tp->_verbose = verbose;
      break; 
    }
  }
  return 0;
}

void
DCFTransmitter::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
  add_task_handlers(&_task);
}

//#include <click/dequeue.cc>
//template class DEQueue<ACKRetrySender2::tx_result_t>;

CLICK_ENDDECLS
EXPORT_ELEMENT(DCFTransmitter)
