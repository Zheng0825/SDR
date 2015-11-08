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
 
// Extension of DCFScheduler
// To deal with rate control and power control

#ifndef CLICK_DCFSCHEDULER2_HH
#define CLICK_DCFSCHEDULER2_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/timer.hh>
#include <click/notifier.hh>
#include <click/etheraddress.hh>
//#include <click/ipaddress.hh>
//#include <click/dequeue.hh>

#include "macnotifier.hh"
#include "dcfscheduler1.hh"

CLICK_DECLS

class MACFrame;
class RateControl; 
class SetDuration;
class PowerControl;

class DCFScheduler2 : public DCFScheduler1 {

public:
  DCFScheduler2();
  ~DCFScheduler2();

  const char *class_name() const { return "DCFScheduler2"; }
  //const char *processing() const { return "la/hh"; }
  //const char *flow_code()  const { return "xy/xx"; }

  void* cast(const char *n);

  int configure(Vector<String> &conf, ErrorHandler *errh);

  //bool run_task();
  //void run_timer(Timer* );
  //void push(int port, Packet *p);

  //void notified(uint32_t num, MACEvent e, const Packet* p);

protected: // for main functions

  // From MACTXScheduler
  //EtherAddress _mac_addr;
  //CollisionPolicy* _cp;

  //DEQueue<Packet *> _waiting_packets;
  //Packet* _trigger_packet;
  //Packet* _response_packet;
  //Packet* _control_packet;
  //Packet* _waiting_packet;

  //MACNotifier* _tx_notifier;
  //Timer _timer;
  //Task _task;

  //struct timeval _timeout_val; 

  //ActiveNotifier _nonfull_notifier; 
  //NotifierSignal _nonfull_signal;
  //NotifierSignal _nonempty_signal;

  //unsigned int _timeout; 
  //unsigned _verbose;

  // From DCFScheduler
  //void check();

  //Packet* generate_trigger(Packet *p);
  //Packet* generate_response(Packet *p);

  //void calculate_timeout(const Packet* p);

  //void push_packet(int port, Packet* p);
  //void push_type(int port, int type);

  virtual void prep_rts(Packet* pkt); 
  virtual void prep_unicast(Packet* pkt); 
  virtual void prep_broadcast(Packet* pkt); 
  //virtual void prep_cts(Packet* ) { return 0;}
  virtual void prep_response(Packet* pkt); 
  //virtual void prep_ack(Packet* ) { return 0;}
  //virtual void prep_done(Packet* ) { return 0;}

protected:
  // From DCFScheduler
  //MACFrame* _rts_frame;
  //RateControl* _rts_rate;
  //SetDuration* _duration;
  //RateControl* _data_rate;
  PowerControl* _rts_power;
  PowerControl* _data_power;


  //enum DCFState {DCF_IDLE, DCF_WAITTX_TRIGGER, DCF_WAITTX_RESPONSE, 
  //               DCF_WAITTX_CTS, DCF_WAITTX_ACK, 
  //               DCF_TX_TRIGGER, DCF_TX_RESPONSE, 
  //               DCF_TX_CTS, DCF_TX_ACK};
  //DCFState _state;
  // FIXME: for previous state when RTS/DATA is arrived
  //DCFState _pstate;

  //enum NextTXPacket {NTXP_NOTHING, NTXP_DONE,
  //                   NTXP_RESPONSE, NTXP_CTS, NTXP_ACK};
  //NextTXPacket process_control(Packet *p);

  //bool _regard_it_lost;

  //void set_state(DCFState state);
  //bool is_state(DCFState state);

  //void transmitter_timer();
  //void receiver_timer();

};

CLICK_ENDDECLS
#endif
