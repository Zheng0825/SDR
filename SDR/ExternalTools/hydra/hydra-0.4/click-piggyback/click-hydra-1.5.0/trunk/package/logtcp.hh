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
 
#ifndef CLICK_LOGTCP_HH
#define CLICK_LOGTCP_HH
#include <click/element.hh>
#include <click/atomic.hh>

CLICK_DECLS

class LogTCP: public Element{
public:
  LogTCP();
  ~LogTCP();

  const char *class_name() const		{ return "LogTCP"; }
  const char *port_count() const    { return "1/1"; }
  const char *processing() const		{ return PUSH; }

  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);

  //Packet *simple_action(Packet *);
  void push(int, Packet*); 

protected:
  String _logfilename; 
  FILE* _fptr_log;
  int _verbose;
  int _logtype; //0: tcpwin 1: tcp ack 

  void process_data(Packet* pkt, int log_type);
};

CLICK_ENDDECLS
#endif
