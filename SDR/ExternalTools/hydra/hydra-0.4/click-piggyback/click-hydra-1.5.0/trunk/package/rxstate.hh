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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/rxstate.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_RXSTATE_HH
#define CLICK_RXSTATE_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/notifier.hh>

CLICK_DECLS


class RXState : public Element { 

public:
  
  RXState();
  ~RXState();
  
  const char *class_name() const	{ return "RXState"; }
  const char *port_count() const	{ return "1/0"; }
  const char *processing() const	{ return AGNOSTIC; }
  const char *flow_code() const		{ return "x/y"; }

  int configure(Vector<String>&, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();

  void push(int, Packet *);
  bool& state() { return _state; }

private:
  bool _state; 
  unsigned _verbose;

  NotifierSignal _nonempty_signal;
  Task _task;
  
};

CLICK_ENDDECLS
#endif
