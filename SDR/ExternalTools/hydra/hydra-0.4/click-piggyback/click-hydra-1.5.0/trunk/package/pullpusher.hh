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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setdefer.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_PULLPUSHER_HH
#define CLICK_PULLPUSHER_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <click/notifier.hh>
CLICK_DECLS

/*
PullPusher()
*/

class PullPusher : public Element { public:
  
  PullPusher();
  ~PullPusher();
  
  const char *class_name() const		{ return "PullPusher"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return PULL_TO_PUSH; }

  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();

  void add_handlers();

private:
  unsigned _verbose;
  NotifierSignal _nonempty_signal;
  Task _task;

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
