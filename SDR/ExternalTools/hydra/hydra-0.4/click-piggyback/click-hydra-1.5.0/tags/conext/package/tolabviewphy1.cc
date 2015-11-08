/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/tolabviewphy1.cc,v $
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
#include <clicknet/mpi.h>
#include "tolabviewphy1.hh"

CLICK_DECLS

// FIXME: how to make this element work, even though there is no second input port connected?
ToLabviewPhy1::ToLabviewPhy1() 
  : _timer(this), _timeout(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ToLabviewPhy1::~ToLabviewPhy1()
{
}

void*
ToLabviewPhy1::cast(const char *n)
{
  if( strcmp(n, "ToLabviewPhy1") == 0 )
  {
    return (ToLabviewPhy1 *)this;
  }
  return ToPhy::cast(n);
}


int
ToLabviewPhy1::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
		        cpSecondsAsMicro, "time between retried us)", &_timeout,
 			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  if(!_timeout)
    _timeout = 100000; // 100ms

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
ToLabviewPhy1::initialize(ErrorHandler *errh) 
{
  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);
  _timer.initialize(this);

  ScheduleInfo::join_scheduler(this, &_task, errh);
  _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);
 
  //check();
  return 0;
}

void
ToLabviewPhy1::push(int port, Packet *p)
{
  hydra_chatter(9, _verbose, "%{element}::push: ", this);

  // input(0) is pull port for MAC frame
  // input(1) is push port for feedback information
  assert(port != 0);

  struct mpitype* mt = (struct mpitype *)p->data();
  uint8_t type = mt->type;

  switch(type)
  {
    case MPITYPE_TX_ACK:
    {
      hydra_chatter(9, _verbose, "%{element}::push: TX_ACK", this);
      if(_waiting_packet)
      {
        recv_txack(_waiting_packet);
        _waiting_packet->kill(); _waiting_packet = 0;
      }
      else
      {
        hydra_chatter(5, _verbose, "%{element}::push: TX_ACK but no waiting_packet", this);
      } 
      p->kill();
      break; 
    }
    case MPITYPE_RX_ACK:
    {
      push_packet(0, p);
      break;
    }
    default:
    {
      hydra_chatter(0, _verbose, "%{element}::push: never this case", this);
      p->kill();
      break;
    }
  } 

}

bool
ToLabviewPhy1::run_task()
{
  hydra_chatter(8, _verbose, "%{element}::run_task: ", this);

  if(_waiting_packet)
  {
    // sleep until waiting packet is sent by push/run_timer function 
    hydra_chatter(4, _verbose, "%{element}::run_task: sleep due to waiting packet", this);
    return false;
  }
  
  Packet *p = input(0).pull();

  if(p)
  {
    hydra_chatter(9, _verbose, "%{element}::run_task: data pulled", this);
    struct mpitype* mt = (struct mpitype *)p->data();
    if(mt->type != MPITYPE_TX_DATA)
    {
      hydra_chatter(4, _verbose, "%{element}::run_task: strange type", this);
      p->kill();
      _task.fast_reschedule();
      return false;
    }

    _waiting_packet = p;
    push_packet(0, _waiting_packet->clone()); 

    struct timeval tv;
    tv.tv_sec = _timeout/1000000;
    tv.tv_usec = _timeout%1000000;
    _timer.schedule_after(tv);
    
  }
  else if(!_nonempty_signal)
  {
    // sleep until new packet is arrived in port 0
    hydra_chatter(4, _verbose, "%{element}::run_task: sleep due to no nonempty_signal", this);
    return false;
  }

  _task.fast_reschedule();
  return (p!=0);
}

void
ToLabviewPhy1::run_timer(Timer* )
{
  hydra_chatter(4, _verbose, "%{element}::run_timer:", this);
  assert(_waiting_packet);

  push_packet(0, _waiting_packet->clone());

  struct timeval tv;
  tv.tv_sec = _timeout/1000000;
  tv.tv_usec = _timeout%1000000;
  _timer.schedule_after(tv);
}

void
ToLabviewPhy1::recv_txack(Packet* p)
{
  _timer.unschedule();

  notify(0, MACListener::TX_DONE, p);

  hydra_chatter(8, _verbose, "%{element}:recv_txack:", this);
  
  if(!_task.scheduled())
     _task.reschedule();
}

void
ToLabviewPhy1::push_packet(int port, Packet* p)
{
  output(port).push(p);
}


String
ToLabviewPhy1::read_param(Element *e, void *vparam)
{
  ToLabviewPhy1 *tp = (ToLabviewPhy1 *)e;
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
ToLabviewPhy1::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  ToLabviewPhy1 *tp = (ToLabviewPhy1 *)e;
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
ToLabviewPhy1::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
  add_task_handlers(&_task);
}



//#include <click/dequeue.cc>
//template class DEQueue<Packet*>;

CLICK_ENDDECLS
ELEMENT_REQUIRES(MACNotifier)
EXPORT_ELEMENT(ToLabviewPhy1)

