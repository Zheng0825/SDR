/*********************************************************
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
 * 
 *  
 ***********************************************************/

#ifndef CLICK_BROADQUEUE_HH
#define CLICK_BROADQUEUE_HH
#include <list>
#include <vector>
#include <click/element.hh>
#include <click/glue.hh>
#include <click/notifier.hh>
CLICK_DECLS

class Broadqueue: public Element { public:
  
  Broadqueue();
  ~Broadqueue();
  
  const char *class_name() const		{ return "Broadqueue"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return PUSH_TO_PULL; }

  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);

  void add_handlers();

  void push(int port, Packet *p);
  // Override the pull method to support prioritizing
  Packet *pull(int port);
  int queue_status(int max_size);

private:

  unsigned _verbose;

  std::vector<int> _qlist; // packet size information 
  std::list<Packet *> _plist;

  ActiveNotifier _empty_note;

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif