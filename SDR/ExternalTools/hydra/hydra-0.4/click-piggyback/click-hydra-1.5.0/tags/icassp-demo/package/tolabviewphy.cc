/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/tolabviewphy.cc,v $
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
#include "tolabviewphy.hh"

CLICK_DECLS

// FIXME: how to make this element work, even though there is no second input port connected?
ToLabviewPhy::ToLabviewPhy() 
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ToLabviewPhy::~ToLabviewPhy()
{
}

void*
ToLabviewPhy::cast(const char *n)
{
  if( strcmp(n, "ToLabviewPhy") == 0 )
  {
    return (ToLabviewPhy *)this;
  }
  return ToPhy::cast(n);
}

int
ToLabviewPhy::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
 			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
ToLabviewPhy::initialize(ErrorHandler *errh) 
{
  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  ScheduleInfo::join_scheduler(this, &_task, errh);
  _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);
 
  //check();
  return 0;
}

void
ToLabviewPhy::push(int port, Packet *p)
{
  hydra_chatter(9, _verbose, "%{element}::push: ", this);

  // input(0) is pull port for MAC frame
  // input(1) is push port for feedback information
  assert(port != 0);

  struct mpitype* mt = (struct mpitype *)p->data();
  uint8_t type = mt->type;

  // control feedbacks never go out.
  p->kill();

  struct timeval tv;
  click_gettimeofday(&tv);
  switch(type)
  {
    case MPITYPE_TX_REQ:
    {
      hydra_chatter(9, _verbose, "%{element}::push: TX_REQ %d.%06d", this, tv.tv_sec, tv.tv_usec);
      if(_waiting_packet)
      {
        hydra_chatter(8, _verbose, "%{element}::push: push packet %d.%06d", this, tv.tv_sec, tv.tv_usec);
        push_waiting_packet(0, _waiting_packet);
        _waiting_packet = 0;
      }
      else
        push_control_packet(0, MPITYPE_TX_NODATA); 
       
       break; 
    }
    default:
    {
       hydra_chatter(0, _verbose, "%{element}::push: never this case", this);
       break;
    }
  } 

}

bool
ToLabviewPhy::run_task()
{
  hydra_chatter(8, _verbose, "%{element}::run_task: ", this);
/*
  if(_waiting_packet)
  {
    // sleep until waiting packet is sent by push/run_timer function 
    hydra_chatter(4, _verbose, "%{element}::run_task: sleep due to waiting packet", this);
    return false;
  }
*/ 
  Packet *p = input(0).pull();

  if(p)
  {

    struct mpitype* mt = (struct mpitype *)p->data();
    hydra_chatter(8, _verbose, "%{element}::run_task: data pulled", this);
    if(mt->type != MPITYPE_TX_DATA)
    {
      hydra_chatter(4, _verbose, "%{element}::run_task: strange type", this);
      p->kill();
      _task.fast_reschedule();
      return false;
    }

    struct timeval tv;
    click_gettimeofday(&tv);
    hydra_chatter(8, _verbose, "%{element}::run_task: got tx packet %d.%06d", this, tv.tv_sec, tv.tv_usec);

    if(_waiting_packet)
    {
      notify(0, MACListener::TX_CHANGE, _waiting_packet);
      _waiting_packet->kill();
    }
    _waiting_packet = p;
  }
  else if (!_nonempty_signal)
  {
    // sleep until new packet is arrived in port 0
    hydra_chatter(4, _verbose, "%{element}::run_task: sleep due to no nonempty_signal", this);
    return false;
  }

  _task.fast_reschedule();
  return (p!=0);
}

void
ToLabviewPhy::push_waiting_packet(int port, Packet* p)
{

  notify(0, MACListener::TX_DONE, p);

  output(port).push(p);

  hydra_chatter(8, _verbose, "%{element}:push_waiting_packet:", this);


  if(!_task.scheduled())
     _task.reschedule();
}

void
ToLabviewPhy::push_control_packet(int port, uint8_t type)
{
  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, 
                                       (unsigned char *)&type, 
                                       sizeof(type), 
                                       HYDRA_TAILROOM);
   
  output(port).push(p_out);
}

void
ToLabviewPhy::push_packet(int port, Packet* p)
{
  output(port).push(p);
}


String
ToLabviewPhy::read_param(Element *e, void *vparam)
{
  ToLabviewPhy *tp = (ToLabviewPhy *)e;
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
ToLabviewPhy::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  ToLabviewPhy *tp = (ToLabviewPhy *)e;
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
ToLabviewPhy::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
  add_task_handlers(&_task);
}



//#include <click/dequeue.cc>
//template class DEQueue<Packet*>;

CLICK_ENDDECLS
ELEMENT_REQUIRES(MACNotifier)
EXPORT_ELEMENT(ToLabviewPhy)

