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
 
#ifndef CLICK_SIMGRPHY_HH
#define CLICK_SIMGRPHY_HH
#include <click/element.hh>
#include <click/timer.hh>
#include "phyprocesstime.hh"

CLICK_DECLS

class SimGrPhy : public Element {

public:
  
  SimGrPhy();
  ~SimGrPhy();

  enum { PHY_IDLE, PHY_RECEIVE, PHY_TRANSMIT};
  enum { PORT_MAC = 0, PORT_CHANNEL = 1 };

  const char *class_name() const { return "SimGrPhy"; }
  const char *port_count() const { return "2/2"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler* errh);

  int initialize(ErrorHandler* errh);

  void push(int port, Packet *p);
  void run_timer(Timer* );

private:
  Timer _timer;
  int _state;
  Packet* _pkt_buf;
  PHYProcessTime* _phy_time;
  unsigned _verbose;

  void start_tx_timer(Packet* pkt);
  void start_rx_timer(Packet* pkt);

  void process_mac(Packet* p);
  void process_channel(Packet* p);

  void push_type(int port, uint8_t type);
  void push_wrong_packet(int port, uint8_t type);

  //void push_packet(int port, Packet* p);
};

CLICK_ENDDECLS
#endif
