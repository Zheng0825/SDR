/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/mactransmitter.cc,v $
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

#include <click/hydra_anno.hh>

#include "mactransmitter.hh"
CLICK_DECLS

MACTransmitter::MACTransmitter() 
  : _tx_state(IDLE), 
    _trigger_packet(0), _response_packet(0), 
    _waiting_trigger_packet(0), _waiting_response_packet(0),
    _delay(0), _tx_notifier(0),  _timer(this), _task(this), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACTransmitter::~MACTransmitter()
{
}

void*
MACTransmitter::cast(const char *n)
{
  if( strcmp(n, "MACTransmitter") == 0 )
    return (MACTransmitter *) this;
  else if( strcmp(n, "MACListener") == 0 )
    return (MACListener*) (this);
  else if( strcmp(n, Notifier::FULL_NOTIFIER) == 0 )
    return static_cast<Notifier*> (&_nonfull_notifier);

  return 0;
}


int
MACTransmitter::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* e = 0;
  int res = cp_va_parse(conf, this, errh,
			cpOptional,
			cpKeywords,
			"NOTIFIER", cpElement, "mac notifier", &e,
			"DELAY", cpSecondsAsMicro, "timeout for delay", &_delay,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  if(e)
  {
    _tx_notifier = (MACNotifier *) e->cast("MACNotifier");
    if(!_tx_notifier)
      return errh->error("NOTIFIER must be set using MACNotifier");
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
MACTransmitter::initialize(ErrorHandler *errh) 
{
  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  _timer.initialize(this);
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
MACTransmitter::push(int port, Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::push: ", this);

  // control port for feedback or scheduling info
  assert(port != 0);
  //check();

  if (is_control(port, p))
    process_control(port, p);
  else
    process_response(port, p); 
  
  p->kill();

  //check();
}

bool
MACTransmitter::run_task()
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
  if(_waiting_trigger_packet)
  {
    if((_tx_notifier && is_txstate(IDLE)) || _nonfull_signal) 
    {
      push_packet(0, _waiting_trigger_packet);

      _waiting_trigger_packet = 0;
      _trigger_packet = 0;

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


  Packet *p = input(0).pull();

  if(p)
  {
    // FIXME: sleep only listeners connected to input(0)
    _nonfull_notifier.sleep();

    // For unicast data of DCF mode, generate RTS and link RTS to _trigger_packet
    // For multicast data , just link Data to _trigger packet
    _trigger_packet = p;
   
    struct timeval now_tv;
    click_gettimeofday(&now_tv); 
    // send trigger packet at run_timer() after delay
    bool now = _timer.start_timer(_delay, now_tv);
    if(now)
    {
      if((_tx_notifier && is_txstate(IDLE)) || _nonfull_signal) 
      {
        push_packet(0, p);
      	_trigger_packet = 0;
      }
      else
      {
        _waiting_trigger_packet = p;
        hydra_chatter(4, _verbose, "%{element}: sleep due to no nonfull signal", this);
	return false;
      } 
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
MACTransmitter::run_timer(Timer* )
{
  assert(!_timer.scheduled());

  if((_tx_notifier && is_txstate (IDLE)) || _nonfull_signal) 
  {
    push_packet(0, _trigger_packet);   
    _trigger_packet = 0; 
  }
  else
  {
    _waiting_trigger_packet = _trigger_packet;
  } 
    

  //check();
}

void 
MACTransmitter::notified(uint32_t , MACEvent e, const Packet *)
{
  if(e == MACListener::TX_DONE)
    _nonfull_notifier.wake(); 
}


void
MACTransmitter::push_packet(int port, Packet* p)
{
       
  output(port).push(p); 

  if(_tx_notifier)
    set_txstate (BUSY);
  else
    _nonfull_notifier.wake();
}

bool
MACTransmitter::is_control(int port, Packet* p)
{
  assert(p && port != 0);

  return true;
}

void
MACTransmitter::process_control(int port, Packet* p)
{
  assert(p && port != 0);
}

void
MACTransmitter::process_response(int port, Packet* p)
{
  assert(p && port != 0);

  output(0).push(p);
}


/*
void
MACTransmitter::check()
{
  // check() should be called *before* most pushes() from element
  // functions, as each push may call back into the element.
}
*/


String
MACTransmitter::read_param(Element *e, void *vparam)
{
  MACTransmitter *tp = (MACTransmitter *)e;
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
MACTransmitter::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  MACTransmitter *tp = (MACTransmitter *)e;
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
MACTransmitter::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
  add_task_handlers(&_task);
}

//#include <click/dequeue.cc>
//template class DEQueue<ACKRetrySender2::tx_result_t>;

CLICK_ENDDECLS
EXPORT_ELEMENT(MACTransmitter)
