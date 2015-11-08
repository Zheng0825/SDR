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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/dcftransmitter.hh,v $
 *  
 ***********************************************************/
#ifndef CLICK_DCFTRANSMITTER_HH
#define CLICK_DCFTRANSMITTER_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/timer.hh>
#include <click/notifier.hh>
//#include <click/ipaddress.hh>
//#include <click/etheraddress.hh>
//#include <click/dequeue.hh>

#include "mactransmitter.hh"

CLICK_DECLS

/*
 * =c
 * DCFTransmitter(I<KEYWORDS>)
 *
 * =s Hydra
 * Actual transmitting using multiple access scheme  
 *
 * =d
 *
 * Description goes here
 * 
 * Keyword arguments are:
 *
 * =over 8
 *
 *
 * =item NOTIFIER
 *
 * Notifier element which notifies the transmission complete.
 * If not presented, start restransmission timer when it push the packet.
 *
 *
 *
 * =item VERBOSE
 *
 * Boolean.  Be noisy.  True by default.
 *
 * =back
 *
 * =h summary read-only
 * Print summary of packet retry statistics
 *
 * =h history read-only
 * Print packet retry history.
 *
 * =h clear write-only
 * Clear out packet retry history.
 *
 * =h reset write-only
 * Reset packet retry statistics.
 * =a 
 * ACKRetrySender2, ACKResponder2, ACKRetrySender, ACKResponder, EtherEncap */

/* packet formats:

Data/ACK packet formats:

(pushed by later EtherEncap) ether dest [6]
(pushed by later EtherEncap) ether src  [6]
(pushed by later EtherEncap) ether type [2] (0x7ffb for data, 0x7ffc for ACK)
src IP 
dst IP 
<encapsulated data packet> (not for ACK)

*/

class DCFTransmitter : public MACTransmitter {
public:
  DCFTransmitter();
  ~DCFTransmitter();

  const char *class_name() const { return "DCFTransmitter"; }
  const char *port_count() const { return "3/1"; }
  const char *processing() const { return "lhh/h"; }
  const char *flow_code()  const { return "xxy/x"; }

  void* cast(const char *n);

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();
  //void run_timer(Timer* );
  void push(int, Packet *);

  void notified(uint32_t, MACListener::MACEvent e, const Packet* p);
  
  void add_handlers();

protected:
  void push_packet(int port, Packet* p);

  bool is_control(int port, Packet* p);
  void process_control(int port, Packet* p);
  void process_response(int port, Packet* p);

  static String read_param(Element *e, void *vparam);
  static int write_param(const String&, Element*, void*, ErrorHandler*);

  enum TXState{ IDLE, TRIGGER_TRANSMITTING, RESPONSE_TRANSMITTING,
                TRIGGER_DEFER, RESPONSE_DEFER, BACKOFF_DEFER };

  TXState _tx_state;
  inline void set_txstate(TXState ts) { _tx_state = ts; }
  inline bool is_txstate(TXState ts) { return (_tx_state == ts); }

  //typedef DEQueue<Packet *> PacketQ;
  //Packet* _trigger_packet;
  //Packet* _response_packet;
  //Packet* _waiting_trigger_packet;
  //Packet* _waiting_response_packet;

  // FIXME: change it to _waiting_trigger or _waiting_response
  Packet* _waiting_packet;
  //unsigned int _delay;   // usecs
  //struct timeval _delay_val; 
  //struct timeval _start_val; 
  //struct timeval _remain_val;

  //MACNotifier* _tx_notifier;
  //MACTimer _timer;
  //Task _task;

  //ActiveNotifier _nonfull_notifier; 
  //NotifierSignal _nonfull_signal;
  //NotifierSignal _nonempty_signal;

  //unsigned _verbose;


private:
  MACTimer _trigger_defer_timer;
  MACTimer _response_defer_timer;
  MACTimer _backoff_timer;

  unsigned _slot_time;

  enum ControlState { CS_IDLE, CS_BUSY, TX, RX};
  ControlState _control_state;
  inline void set_ctrlstate(ControlState cs) { _control_state = cs; }
  inline bool is_ctrlstate(ControlState cs) { return (_control_state == cs); }

  bool start_trigger_defer_timer(Packet* p);
  bool start_response_defer_timer(Packet* p);
  bool start_backoff_timer(Packet* p);
  static void run_trigger_defer_timer(Timer* timer, void* thunk);
  static void run_response_defer_timer(Timer* timer, void* thunk);
  static void run_backoff_timer(Timer* timer, void* thunk);

  void process_cca_busy(Packet* p);
  void process_cca_idle(Packet* p);
  


};

CLICK_ENDDECLS
#endif
