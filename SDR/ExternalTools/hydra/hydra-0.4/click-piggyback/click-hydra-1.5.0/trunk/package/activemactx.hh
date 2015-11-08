/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/activemactx.hh,v $
 *  
 * 
 *  Copyright (C) 2009  The University of Texas at Austin.
 *  
 *  This file is part of Hydra: A wireless multihop testbed.
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, subject to the conditions
 *  listed in the Click LICENSE file. These conditions include: you must
 *  preserve this copyright notice, and you cannot mention the copyright
 *  holders in advertising related to the Software without their permission.
 *  The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 *  notice is a summary of the Click LICENSE file; the license in that file is
 *  legally binding.
 * 
 ***********************************************************/
#ifndef CLICK_ACTIVEMACTX_HH
#define CLICK_ACTIVEMACTX_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include "mactimer.hh"
#include <click/notifier.hh>
#include "macnotifier.hh"

CLICK_DECLS



class ActiveMACTX : public Element, public MACNotifier, public MACListener {
public:
  ActiveMACTX();
  ~ActiveMACTX();

  const char *class_name() const { return "ActiveMACTX"; }
  const char *port_count() const { return "2/1"; }
  const char *processing() const { return "lh/h"; }
  const char *flow_code()  const { return "xy/x"; }

  void* cast(const char *n);

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();
  void run_timer(Timer* );
  void push(int, Packet *);

  void notified(uint32_t, MACEvent, const Packet *);

  void add_handlers();

protected:
  Task _task;
  MACTimer _timer;

  NotifierSignal _nonempty_signal;
  NotifierSignal _full_signal;

  ActiveNotifier _full_notifier; 
  //ActiveNotifier _nonempty_notifier; 

  MACNotifier* _tx_notifier;

  Packet *_packet;
  Packet *_waiting_packet;

  unsigned _verbose;

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

};

CLICK_ENDDECLS
#endif
