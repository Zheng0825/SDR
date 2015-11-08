/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/activemactx.cc,v $ 
 *  
 ***********************************************************/
#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>
#include "activemactx.hh"

CLICK_DECLS

// FIXME: how to make this element work, even though there is no second input port connected?
ActiveMACTX::ActiveMACTX() : _task(this), _timer(this), 
    _tx_notifier(0), _packet(0), _waiting_packet(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ActiveMACTX::~ActiveMACTX()
{
}

void*
ActiveMACTX::cast(const char *n)
{
  if( strcmp(n, "ActiveMACTX") == 0 )
    return (ActiveMACTX *) (this);
  else if( strcmp(n, "MACNotifier") == 0 )
    return (MACNotifier*) (this);
  else if( strcmp(n, "MACListener") == 0 )
    return (MACListener*) (this);
  //else if( strcmp(n, Notifier::EMPTY_NOTIFIER) == 0 )
  //  return static_cast<Notifier*> (&_nonempty_notifier);
  else if( strcmp(n, Notifier::FULL_NOTIFIER) == 0 )
    return static_cast<Notifier*> (&_full_notifier);

  return Element::cast(n);
}


int
ActiveMACTX::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* e = 0;
  int res = cp_va_parse(conf, this, errh,
 			cpOptional,
			cpKeywords,
			"TXNOTIFIER", cpElement, "notifier for tx done", &e,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  if(e)
  {
    _tx_notifier = (MACNotifier *) e->cast("MACNotifier");
    if(!_tx_notifier)
      return errh->error("TXNOTIFIER must be set using MACNotifier");
  }


  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
ActiveMACTX::initialize(ErrorHandler *errh) 
{
  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  ScheduleInfo::join_scheduler(this, &_task, errh);
  _timer.initialize(this);

  _full_notifier.initialize(router());
  _full_notifier.set_active(true);
  //_nonempty_notifier.initialize(router());

  _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);
  _full_signal = Notifier::downstream_full_signal(this, 0, &_task);
  if(_tx_notifier)
  {
    _tx_notifier->add_listener( (MACListener *) this);
  }

  return 0;
}

void
ActiveMACTX::push(int port, Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::push: ", this);

  // input(0) is pull port for MAC frame
  // input(1) is push port for feedback information
  assert(port != 0);
  assert(_waiting_packet == 0);
  assert(_packet);

  p->kill();

  _packet->kill();
  _packet = 0;

  _full_notifier.wake();

  _timer.stop_timer();
  _task.reschedule();
}

bool
ActiveMACTX::run_task()
{
  hydra_chatter(8, _verbose, "%{element}::run_task: ", this);

  // if there is nothing to do, go to sleep
  if(!_nonempty_signal && !_full_signal)
  {
    hydra_chatter(4, _verbose, "%{element}: sleep due to no signal", this);
    return false;
  }

  if(_waiting_packet)
  {
    // sleep until waiting packet is sent by push/run_timer function 
    if(_full_signal)
    {
      output(0).push(_waiting_packet);
      _waiting_packet = 0;

      unsigned timeout = 1000000; // 1s
      struct timeval now; 
      click_gettimeofday(&now);
      _timer.start_timer(timeout, now);

      return true;
    }
    hydra_chatter(4, _verbose, "%{element}: sleep due to waiting packet", this);
    return false;
  }
  
  Packet *p = input(0).pull();

  if(p)
  {
    _full_notifier.sleep();

    _packet = p;
    if(_full_signal)
    {
      output(0).push(_packet->clone());

      unsigned timeout = 1000000; // 1s
      struct timeval now; 
      click_gettimeofday(&now);
      _timer.start_timer(timeout, now);
      return true;
    }
    else 
    {
       _waiting_packet = _packet->clone();
    }
  }
  else if(!_nonempty_signal)
  {
    // sleep until new packet is arrived in port 0
    hydra_chatter(4, _verbose, "%{element}: sleep due to no packet and no signal", this);
    return false;
  }

  _task.fast_reschedule();
  return (p!=0);
}

void
ActiveMACTX::run_timer(Timer* )
{
  assert(_packet);

  if(_full_signal)
  {
    output(0).push(_packet->clone());


    unsigned timeout = 1000000; // 1s
    struct timeval now; 
    click_gettimeofday(&now);
    _timer.start_timer(timeout, now);
    
  }
  else
  {
    _waiting_packet = _packet->clone();
  }
}

void 
ActiveMACTX::notified(uint32_t, MACEvent, const Packet *)
{
  notify(0, MACListener::TX_DONE, _packet);
}

String
ActiveMACTX::read_param(Element *e, void *vparam)
{
  ActiveMACTX *tp = (ActiveMACTX *)e;
  switch( (intptr_t)vparam )
  {
    case 0:
      return String(tp->_verbose);
    default:
      break;
  }
  return String();
}

int
ActiveMACTX::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  ActiveMACTX *tp = (ActiveMACTX *)e;
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
ActiveMACTX::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
  add_task_handlers(&_task);
}



CLICK_ENDDECLS
ELEMENT_REQUIRES(MACNotifier)
ELEMENT_REQUIRES(MACListener)
EXPORT_ELEMENT(ActiveMACTX)

