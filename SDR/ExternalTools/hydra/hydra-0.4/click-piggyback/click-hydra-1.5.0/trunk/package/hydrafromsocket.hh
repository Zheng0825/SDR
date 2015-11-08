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

#ifndef CLICK_HYDRAFROMSOCKET_HH
#define CLICK_HYDRAFROMSOCKET_HH
#include <click/element.hh>
CLICK_DECLS

class HydraFromSocket : public Element { public:

  HydraFromSocket();
  ~HydraFromSocket();

  const char *class_name() const	{ return "HydraFromSocket"; }
  const char *port_count() const	{ return "0/1"; }

  int configure(Vector<String> &conf, ErrorHandler *);
  int initialize(ErrorHandler *);
  void cleanup(CleanupStage);

  void * handle(int);
  void selected(int);

 private:

  bool _verbose;
  int _fd;
  int _family;
  int _socktype;
  int _protocol;
  IPAddress _ip;
  unsigned short _port;
  String _pathname;
  unsigned _snaplen;
  bool _frame;

  Vector<WritablePacket *> _packets;
  Vector<int> _active;

  int initialize_socket_error(ErrorHandler *, const char *);

};

CLICK_ENDDECLS
#endif
