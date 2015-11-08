/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/mactxscheduler.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_MACTXSCHEDULER_HH
#define CLICK_MACTXSCHEDULER_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/timer.hh>
#include <click/notifier.hh>
#include <click/etheraddress.hh>
//#include <click/ipaddress.hh>
//#include <click/dequeue.hh>

#include "macnotifier.hh"

CLICK_DECLS

/*
 * =c
 * MACTXScheduler(I<KEYWORDS>)
 *
 * =s Hydra
 * Transmission scheduler
 *
 * =d
 *
 * Description goes here
 * 
 * Keyword arguments are:
 *
 * =over 8
 *
 * =item MACADDR
 *
 * This node's MAC address, Required argument. 
 *
 * =item COLLISIONREDUCING
 *
 * CollisionReducing element which determines collision reducing scheme 
 * such as backoff scheme and persistence transmission
 * Required argument. 
 *
 * =item NOTIFIER
 *
 * Notifier element which notifies the transmission complete.
 * If not presented, start restransmission timer when it push the packet.
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

/* packet formats:

Data/ACK packet formats:

(pushed by later EtherEncap) ether dest [6]
(pushed by later EtherEncap) ether src  [6]
(pushed by later EtherEncap) ether type [2] (0x7ffb for data, 0x7ffc for ACK)
src IP 
dst IP 
<encapsulated data packet> (not for ACK)

*/

class CollisionPolicy;

class MACTXScheduler : public Element, public MACListener {
public:
  MACTXScheduler();
  ~MACTXScheduler();

  const char *class_name() const { return "MACTXScheduler"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return "la/hh"; }
  const char *flow_code()  const { return "xy/xx"; }

  void* cast(const char *n);

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();
  void run_timer(Timer* );
  void push(int port, Packet *p);

  void notified(uint32_t num, MACEvent e, const Packet* p);

  void add_handlers();

protected: // for main functions

  EtherAddress _mac_addr;
  CollisionPolicy* _cp;

  //DEQueue<Packet *> _waiting_packets;
  Packet* _trigger_packet;
  Packet* _response_packet;
  Packet* _control_packet;
  Packet* _waiting_packet;

  MACNotifier* _tx_notifier;
  Timer _timer;
  Task _task;

  struct timeval _timeout_val; 

  ActiveNotifier _nonfull_notifier; 
  NotifierSignal _nonfull_signal;
  NotifierSignal _nonempty_signal;
 
  unsigned int _timeout; 
  unsigned _verbose;

  virtual void check();

  virtual Packet* generate_trigger(Packet *p);
  virtual Packet* generate_response(Packet *p);
  virtual void calculate_timeout(const Packet* p); 

  virtual void push_packet(int port, Packet* p);

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

private:
  enum NextTXPacket {NTXP_NOTHING, NTXP_DONE, NTXP_RESPONSE};
  NextTXPacket process_control(Packet *p);

};


CLICK_ENDDECLS
#endif
