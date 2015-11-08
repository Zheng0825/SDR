// -*- mode: c++; c-basic-offset: 2 -*-
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

#ifndef CLICK_HYDRATOSOCKET_HH
#define CLICK_HYDRATOSOCKET_HH
#include <click/element.hh>
#include <click/string.hh>
#include <click/task.hh>
#include <click/notifier.hh>
#include <sys/un.h>
CLICK_DECLS


class HydraToSocket : public Element { public:

  HydraToSocket();
  ~HydraToSocket();

  const char *class_name() const	{ return "HydraToSocket"; }
  const char *port_count() const	{ return "1/0"; }
  const char *processing() const	{ return AGNOSTIC; }
  
  int configure(Vector<String> &conf, ErrorHandler *);
  int initialize(ErrorHandler *);
  void cleanup(CleanupStage);
  void add_handlers();

  void selected(int);

  void push(int port, Packet *);
  bool run_task();

protected:
  Task _task;
  void send_packet(Packet *);

 private:

  bool _verbose;
  int _fd;
  int _active;
  int _family;
  int _socktype;
  int _protocol;
  IPAddress _ip;
  int _port;
  String _pathname;
  int _snaplen;
  union { struct sockaddr_in in; struct sockaddr_un un; } sa;
  socklen_t sa_len;
  NotifierSignal _signal;
  int _nodelay;
  bool _frame;

  int initialize_socket_error(ErrorHandler *, const char *);

};

CLICK_ENDDECLS
#endif
