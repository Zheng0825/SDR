/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/tolabviewphy1.hh,v $
 *  
 ***********************************************************/
#ifndef CLICK_TOLABVIEWPHY1_HH
#define CLICK_TOLABVIEWPHY1_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/notifier.hh>

#include "tophy.hh"
#include "mactimer.hh"

CLICK_DECLS

class ToLabviewPhy1 : public ToPhy {
public:
  ToLabviewPhy1();
  ~ToLabviewPhy1();

  const char *class_name() const { return "ToLabviewPhy1"; }
  const char *port_count() const { return "2/1"; }
  const char *processing() const { return "lh/h"; }
  const char *flow_code()  const { return "xy/x"; }

  void* cast(const char *n);

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();
  void run_timer(Timer* );
  void push(int, Packet *);

  void add_handlers();

protected:

  //Packet *_waiting_packet;
  //NotifierSignal _nonempty_signal;
  //Task _task;
  //unsigned _verbose;

   MACTimer _timer;
   unsigned _timeout;
  
  void push_packet(int port, Packet* p);
  //void check();

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

private:
  void recv_txack(Packet* p);

};

CLICK_ENDDECLS
#endif
