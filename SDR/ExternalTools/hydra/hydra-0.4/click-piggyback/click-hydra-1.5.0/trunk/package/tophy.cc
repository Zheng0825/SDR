/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 * 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 * 
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/tophy.cc,v $
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
#include "tophy.hh"
CLICK_DECLS

// FIXME: how to make this element work, even though there is no second input port connected?
ToPhy::ToPhy() 
  : _waiting_packet(0), _task(this), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ToPhy::~ToPhy()
{
}

void*
ToPhy::cast(const char *n)
{
  if( strcmp(n, "ToPhy") == 0 )
    return (ToPhy *)this;
  else if( strcmp(n, "MACNotifier") == 0 )
    return (MACNotifier*) (this);

  return 0;
}

int
ToPhy::configure(Vector<String> &conf, ErrorHandler *errh)
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
ToPhy::initialize(ErrorHandler *errh) 
{
  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  ScheduleInfo::join_scheduler(this, &_task, errh);
  _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);
  
  hydra_chatter(9, _verbose, "%{element}::initialize: listeners size %d", this,  _listeners.size());
  //check();
  return 0;
}

void
ToPhy::push(int port, Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::push: ", this);

  // input(0) is pull port for MAC frame
  // input(1) is push port for feedback information
  assert(port != 0);

  // add checking control feedbacks such as TXREQ, TXACK.

  // control feedbacks never go out.
  p->kill();

  if (_waiting_packet) 
  {
    push_packet(0, _waiting_packet);
    _waiting_packet = 0;
    if(!_task.scheduled())
      _task.reschedule();
     
    hydra_chatter(5, _verbose, "%{element}: got feedback", this);
    return;
  }

}

bool
ToPhy::run_task()
{
  hydra_chatter(8, _verbose, "%{element}::run_task: ", this);

  if(_waiting_packet)
  {
    // sleep until waiting packet is sent by push/run_timer function 
    hydra_chatter(4, _verbose, "%{element}: sleep due to waiting packet", this);
    return false;
  }
  
  Packet *p = input(0).pull();

  if(p)
  {
    _waiting_packet = p;
    if(ninputs() == 1)
    {
      // dummy sender
      push_packet(0, _waiting_packet);
      _waiting_packet = 0;
    }
  }
  else if (!_nonempty_signal)
  {
    // sleep until new packet is arrived in port 0
    hydra_chatter(4, _verbose, "%{element}: sleep due to no signal", this);
    return false;
  }

  _task.fast_reschedule();
  return (p!=0);
}

void
ToPhy::push_packet(int port, Packet* p)
{
  output(port).push(p);

  notify(0, MACListener::TX_DONE, _waiting_packet);
}

void
ToPhy::check()
{
  // check() should be called *before* most pushes() from element
  // functions, as each push may call back into the element.
  
}


String
ToPhy::read_param(Element *e, void *vparam)
{
  ToPhy *tp = (ToPhy *)e;
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
ToPhy::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  ToPhy *tp = (ToPhy *)e;
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
ToPhy::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
  add_task_handlers(&_task);
}



//#include <click/dequeue.cc>
//template class DEQueue<Packet*>;

CLICK_ENDDECLS
ELEMENT_REQUIRES(MACNotifier)
EXPORT_ELEMENT(ToPhy)

