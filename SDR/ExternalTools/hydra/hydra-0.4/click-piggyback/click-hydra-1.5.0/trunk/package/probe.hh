/* 
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
 */

#ifndef PROBE
#define PROBE

/*
 * =c
 * LinkStat([I<KEYWORDS>])
 * =s Grid
 * Track broadcast loss rates. 
 *
 * =d
 *
 * Expects Link probe packets as input.  Records the last WINDOW unique
 * (not neccessarily sequential) sequence numbers of link probes from
 * each host, and calculates loss rates over the last TAU milliseconds
 * for each host.  If the output is connected, sends probe
 * packets every PERIOD milliseconds.  The source Ethernet 
 * address ETH must be specified if the second output is
 * connected.
 *
 * Keyword arguments are:
 *
 * =over 8
 *
 * =item ETH
 *
 * Ethernet address of this node; required if output is connected.
 *
 * =item PERIOD
 *
 * Unsigned integer.  Millisecond period between sending link probes
 * if second output is connected.  Defaults to 1000 (1 second).
 *
 * =item WINDOW
 *
 * Unsigned integer.  Number of most recent sequence numbers to remember
 * for each host.  Defaults to 100.
 *
 * =item TAU
 *
 * Unsigned integer.  Millisecond period over which to calculate loss
 * rate for each host.  Defaults to 10,000 (10 seconds).
 *
 * =item SIZE
 *
 * Unsigned integer.  Total number of bytes in probe packet, including
 * ethernet header and above.  Defaults to 1000.
 *
 * =item USE_SECOND_PROTO
 *
 * Boolean.  If true, use the alternate LinkStat ethernet protocol
 * number.  The normal protocol number is 0x7ffe; the alternate number
 * is 0x7ffd.  Defaults to false.
 *
 * =back */

#include <click/element.hh>
#include <click/glue.hh>
#include <click/etheraddress.hh>
#include <elements/grid/timeutils.hh>

CLICK_DECLS

class Timer;

class Probe : public Element {

public:  
  Probe();
  ~Probe();
  
  const char *class_name() const		{ return "Probe"; }
  const char *port_count() const		{ return "0/1"; }
  const char *processing() const		{ return PUSH; }
  

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *);

  //void push(int, Packet*);
  
  static const unsigned short ETHERTYPE_PROBE = 0x7ffe;

private:
  unsigned int _period; // msecs
  unsigned int _probe_size; // bytes
  EtherAddress _eth;

  static void static_send_hook(Timer *, void *e) { ((Probe *) e)->send_hook(); }
  void send_hook();

  Timer *_send_timer;
  Timestamp _next_bcast;

};

CLICK_ENDDECLS
#endif
