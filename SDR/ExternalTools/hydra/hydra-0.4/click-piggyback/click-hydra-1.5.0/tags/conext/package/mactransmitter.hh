/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/mactransmitter.hh,v $
 *  
 ***********************************************************/
#ifndef CLICK_MACTRANSMITTER_HH
#define CLICK_MACTRANSMITTER_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/timer.hh>
#include <click/notifier.hh>
//#include <click/ipaddress.hh>
//#include <click/etheraddress.hh>
//#include <click/dequeue.hh>

#include "mactimer.hh"
#include "macnotifier.hh"

CLICK_DECLS

/*
 * =c
 * MACTransmitter(I<KEYWORDS>)
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

class MACTransmitter : public Element, public MACListener {
public:
  MACTransmitter();
  ~MACTransmitter();

  const char *class_name() const { return "MACTransmitter"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return "lhh/h"; }
  const char *flow_code()  const { return "xxy/x"; }

  void* cast(const char *n);

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();
  void run_timer(Timer* );
  void push(int, Packet *);

  void notified(uint32_t, MACListener::MACEvent e, const Packet* p);
  
  void add_handlers();

protected:
  enum TXState { IDLE, BUSY };

  virtual void push_packet(int port, Packet* p);

  virtual bool is_control(int port, Packet* p);
  virtual void process_control(int port, Packet* p);
  virtual void process_response(int port, Packet* p);

  virtual inline void set_txstate(TXState ts) { _tx_state = ts; }
  virtual inline bool is_txstate(TXState ts) { return (_tx_state == ts); }

  static String read_param(Element *e, void *vparam);
  static int write_param(const String&, Element*, void*, ErrorHandler*);

  TXState _tx_state;
  //typedef DEQueue<Packet *> PacketQ;
  Packet* _trigger_packet;
  Packet* _response_packet;
  Packet* _waiting_trigger_packet;
  Packet* _waiting_response_packet;

  unsigned int _delay;   // usecs
  struct timeval _delay_val; 
  struct timeval _start_val; 
  struct timeval _remain_val;

  MACNotifier* _tx_notifier;
  MACTimer _timer;
  Task _task;

  ActiveNotifier _nonfull_notifier; 
  NotifierSignal _nonfull_signal;
  NotifierSignal _nonempty_signal;

  unsigned _verbose;

};

CLICK_ENDDECLS
#endif
