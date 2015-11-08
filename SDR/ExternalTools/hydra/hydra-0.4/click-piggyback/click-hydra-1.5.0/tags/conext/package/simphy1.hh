/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/simphy1.hh,v $ 
 *  
 ***********************************************************/
#ifndef CLICK_SIMPHY1_HH
#define CLICK_SIMPHY1_HH
#include <click/element.hh>
#include <click/timer.hh>
#include <click/task.hh>
#include <click/notifier.hh>

CLICK_DECLS

/*
 * SimPhy1
 *
 * Simulate reliable interface to MAC across etherenet interface
 * 
 */

class SimPhy1 : public Element {

public:
  
  SimPhy1();
  ~SimPhy1();

  enum { PORT_MAC = 0, PORT_CHANNEL = 1 };

  const char *class_name() const { return "SimPhy1"; }
  const char *port_count() const { return "2/2"; }
  const char *processing() const { return "lh/hh"; }

  int configure(Vector<String> &, ErrorHandler *);

  int initialize(ErrorHandler* errh);

  void push(int port, Packet *p);
  bool run_task();
  void run_timer(Timer* );

private:
  Task _task;
  Timer _timer;
  unsigned _timeout;
  NotifierSignal _nonempty_signal;

  Packet* _waiting_packet;
  void push_type(int port, uint8_t type);
};

CLICK_ENDDECLS
#endif
