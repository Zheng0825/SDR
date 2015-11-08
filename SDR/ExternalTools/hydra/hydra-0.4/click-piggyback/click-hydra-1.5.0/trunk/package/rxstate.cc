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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/rxstate.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>

#include <click/hydra_anno.hh>
#include "rxstate.hh"
CLICK_DECLS

RXState::RXState() 
  : _state(false),_verbose(0), _task(this)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RXState::~RXState()
{
}

int
RXState::configure(Vector<String> &conf, ErrorHandler* errh)
{
  int res = cp_va_parse(conf, this, errh,
			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return res;
}

int 
RXState::initialize(ErrorHandler* errh)
{
  hydra_chatter(9, _verbose, "%{element}::initialize: ", this);

  if(input_is_pull(0))
  {
    ScheduleInfo::join_scheduler(this, &_task, errh);
    _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);
  }

  return 0;
}

void
RXState::push(int, Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::push: ", this);


  _state = true;
  p->kill();
}

bool
RXState::run_task()
{
  hydra_chatter(8, _verbose, "%{element}::run_task: ", this);

  Packet* p = input(0).pull();

  if(p)
  {
    _state = true;
    p->kill();
  }
  else if(!_nonempty_signal)
     return false;

  _task.fast_reschedule();
  return (p != 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RXState)
