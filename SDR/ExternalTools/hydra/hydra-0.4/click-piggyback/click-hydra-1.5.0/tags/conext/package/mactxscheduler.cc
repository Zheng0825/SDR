/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/mactxscheduler.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/glue.hh>
#include <clicknet/ether.h>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>
#include "collisionpolicy.hh"
#include "mactxscheduler.hh"

CLICK_DECLS

MACTXScheduler::MACTXScheduler() 
  : _cp(0), 
    _trigger_packet(0), _response_packet(0), _control_packet(0), _waiting_packet(0),
    _tx_notifier(0), _timer(this), _task(this), 
    _timeout(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACTXScheduler::~MACTXScheduler()
{
}

void*
MACTXScheduler::cast(const char *n)
{
  if( strcmp(n, "MACTXScheduler") == 0 )
    return (MACTXScheduler *) this;
  else if( strcmp(n, "MACListener") == 0 )
    return (MACListener *) (this);
  else if( strcmp(n, Notifier::FULL_NOTIFIER) == 0 )
    return static_cast<Notifier*> (&_nonfull_notifier);

  return 0; 
}


int
MACTXScheduler::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element *cp_e, *tx_notifier_e = 0;

  int res = cp_va_parse(conf, this, errh,
			cpEtherAddress, "local MAC address", &_mac_addr,
 			cpElement, "collision reducing polocy", &cp_e,
			cpOptional,
			cpKeywords,
			"NOTIFIER", cpElement, "mac notifier", &tx_notifier_e,
		        "TIMEOUT", cpSecondsAsMicro, "time between retried us)", &_timeout,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  if(!_mac_addr) 
    return errh->error("MAC address must be specified");


  if(cp_e) {
    _cp = (CollisionPolicy *) cp_e->cast("CollisionPolicy");
    if(!_cp) return errh->error("CollisionReducing policy must be set using CollisionPolicy");
  } else {
    return errh->error("CollisionReducing Policy must be specified");
  }

 
  if(tx_notifier_e) {
    _tx_notifier = (MACNotifier *) tx_notifier_e->cast("MACNotifier");
    if(!_tx_notifier) return errh->error("NOTIFIER must be set using MACNotifier");
  }


  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
MACTXScheduler::initialize(ErrorHandler *errh) 
{

  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  _timer.initialize(this);
  ScheduleInfo::join_scheduler(this, &_task, errh);

  _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);
  _nonfull_signal = Notifier::downstream_full_signal(this, 0, &_task);

  if(_tx_notifier)
    _tx_notifier->add_listener( (MACListener *) this);

  // Why this initialization occurrs in configure() of fullnoteqeueue.cc?
  _nonfull_notifier.initialize(router());
  _nonfull_notifier.set_active(true);

  //check();
  return 0;
}

void
MACTXScheduler::push(int port, Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::push: ", this);

  // control port for feedback or scheduling info
  assert(port != 0);
  //check();

  if (!_trigger_packet) 
  {
    // we aren't waiting for ACK
    hydra_chatter(0, _verbose, "%{element}: got unexpected control packet", this);

    p->kill();
    return;
  }

  NextTXPacket ntxp = process_control(p);
  switch(ntxp)
  {
    case NTXP_RESPONSE:
    {
      _timer.unschedule();
      _cp->success(_trigger_packet);
      push_packet(1, _response_packet);
      break;
    }
    case NTXP_DONE:
    {
      _timer.unschedule();

      _cp->success(_response_packet ? _response_packet : _trigger_packet);
      _trigger_packet->kill(); _trigger_packet = 0;
      if(_response_packet)
      {
        _response_packet->kill(); _response_packet = 0;
      }

      _nonfull_notifier.wake();
      if(!_task.scheduled()) _task.reschedule();

      break;
    }

    case NTXP_NOTHING:
    default:
      break;

  }
  
  p->kill();

  //check();
}

bool
MACTXScheduler::run_task()
{
  hydra_chatter(8, _verbose, "%{element}::run_task: ", this);

  // if there is nothing to do, go to sleep
  if(!_nonempty_signal && !_nonfull_signal)
  {
    hydra_chatter(4, _verbose, "%{element}: sleep due to no signal", this);
    return false;
  }

  // first check if there is packet to push, 
  // assuming that only one packet can be sent
  if(_waiting_packet)
  {
    if(_nonfull_signal) 
    {
      push_packet(0, _waiting_packet); 
      _waiting_packet = 0;
      _task.fast_reschedule();
      return true;
    }
    else
    {
      // sleep until waiting packet is sent by task 
      hydra_chatter(4, _verbose, "%{element}: sleep due to trigger packet waited", this);
      return false;
    }  
  }


  // second, check if there is outstanding packet
  if(_trigger_packet)
  {
    // task should be waken up by timer when no more retransmission
    hydra_chatter(4, _verbose, "%{element}: sleep due to outstanding packet", this);
    return false;
  }

  
  Packet *p = input(0).pull();

  if(p)
  {
    // FIXME: sleep only listeners connected to input(0)
    _nonfull_notifier.sleep();
    // For unicast data of DCF mode, generate RTS and link RTS to _trigger_packet
    // For multicast data , just link Data to _trigger packet
    _trigger_packet = generate_trigger(p);
    if(!_trigger_packet)
    {
      p->kill();
      _task.fast_reschedule();
      return false;
    }

    _response_packet = generate_response(p);
    p->kill();

    // send trigger packet 
    // RTS for DCF with RTS/CTS
    // DATA for broadcast packet or TDMA packets
    _cp->reset_retry(_trigger_packet);
    _waiting_packet = _trigger_packet->clone();
    _cp->set_control(_waiting_packet);
    if(_waiting_packet) 
    {
      if(_nonfull_signal)
      {
        push_packet(0, _waiting_packet); 
        _waiting_packet = 0;
      }
      // else 
      // if downstream element is full, wait until nonfull_signal is set in run_task
    }
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


void
MACTXScheduler::run_timer(Timer* )
{
  assert(!_waiting_packet && _trigger_packet && !_timer.scheduled());

  // prepare_packet generate new waiting packet by make or clone 
  
  if(_cp->retry(0) && ninputs() > 1) // retransmission
  {
    _cp->fail(_trigger_packet);
    _waiting_packet = _trigger_packet->clone();
    _cp->set_control(_waiting_packet);
    if(_nonfull_signal)
    {
     // push_packet kills packet
      push_packet(0, _waiting_packet); 
      _waiting_packet = 0;
    }
    // else 
    // if downstream element is full, wait until nonfull_signal is set in run_task
  }
  else  // no retransmission, just drop
  {
    _cp->reset_retry(_trigger_packet);
    if(_waiting_packet)
    {
      _waiting_packet->kill(); _waiting_packet = 0;   
    }

    if(_response_packet) 
    {
      _response_packet->kill(); _response_packet = 0;
    }
    
    _trigger_packet->kill(); _trigger_packet = 0;   


    _task.reschedule();
  }

  //check();
}


void 
MACTXScheduler::notified(uint32_t, MACEvent, const Packet *)
{
  calculate_timeout(0);
  // FIXME: if remain time is too short just busy waiting
  _timer.schedule_after(_timeout_val);
}

Packet*
MACTXScheduler::generate_trigger(Packet *p)
{
  Packet* p_out = p->clone();
  return p_out;
}

Packet*
MACTXScheduler::generate_response(Packet*)
{
  Packet* p_out = 0;

  return p_out;
}

MACTXScheduler::NextTXPacket
MACTXScheduler::process_control(Packet *)
{
  return NTXP_DONE;
}

void
MACTXScheduler::push_packet(int port, Packet* p)
{
  MACListener::MACEvent e = MACListener::TX_DONE;
  // if there is notifier start timer at notified()
  if(_tx_notifier == 0 )
    notified(0, e, p); 

  output(port).push(p);
}

void 
MACTXScheduler::calculate_timeout(const Packet*)
{
  if(_timeout != 0)
  {
    _timeout_val.tv_sec =  _timeout/(1000000);
    _timeout_val.tv_usec = _timeout%(1000000);
  }
  else
  {
    // dummy timeout = 1ms
    _timeout_val.tv_sec = 0;
    _timeout_val.tv_usec = 1000;
  }
}

void
MACTXScheduler::check()
{
  // check() should be called *before* most pushes() from element
  // functions, as each push may call back into the element.
  
}



String
MACTXScheduler::read_param(Element *e, void *vparam)
{
  MACTXScheduler *tp = (MACTXScheduler *)e;
  switch( (intptr_t)vparam )
  {
    case 0:
      return String(tp->_verbose);
    default:
      return String();
  }

  return String();
}

int
MACTXScheduler::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  MACTXScheduler *tp = (MACTXScheduler *)e;
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
MACTXScheduler::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
  add_task_handlers(&_task);
}


CLICK_ENDDECLS
EXPORT_ELEMENT(MACTXScheduler)
