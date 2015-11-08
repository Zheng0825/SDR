/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/tophy.hh,v $
 *  
 ***********************************************************/
#ifndef CLICK_TOPHY_HH
#define CLICK_TOPHY_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/notifier.hh>
//#include <click/timer.hh>
//#include <click/ipaddress.hh>
//#include <click/etheraddress.hh>
//#include <click/dequeue.hh>

#include "macnotifier.hh"

CLICK_DECLS

/*
 * =c
 * ToPhy(I<KEYWORDS>)
 *
 * =s Hydra
 * To physical layer devices 
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
 * =item TIMEOUT
 *
 * Unsigned integer, > 0.  Milliseconds.  Wait this long before
 * resending the packet. If not presented, calculate timeout 
 * based on packet type.
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


class ToPhy : public Element, public MACNotifier {
public:
  ToPhy();
  ~ToPhy();

  const char *class_name() const { return "ToPhy"; }
  const char *port_count() const { return "2/1"; }
  const char *processing() const { return "lh/h"; }
  const char *flow_code()  const { return "xy/x"; }

  void* cast(const char *n);

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();
  void push(int, Packet *);

  void add_handlers();

protected:

  Packet *_waiting_packet;
  NotifierSignal _nonempty_signal;
  Task _task;

  unsigned _verbose;

  void push_packet(int port, Packet* p);
  void check();

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

};

CLICK_ENDDECLS
#endif
