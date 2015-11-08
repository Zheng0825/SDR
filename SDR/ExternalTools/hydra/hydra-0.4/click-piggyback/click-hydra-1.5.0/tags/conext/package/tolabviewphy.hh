/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/tolabviewphy.hh,v $
 *  
 ***********************************************************/
#ifndef CLICK_TOLABVIEWPHY_HH
#define CLICK_TOLABVIEWPHY_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/notifier.hh>

#include "tophy.hh"

CLICK_DECLS

class ToLabviewPhy : public ToPhy {
public:
  ToLabviewPhy();
  ~ToLabviewPhy();

  const char *class_name() const { return "ToLabviewPhy"; }
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

  //Packet *_waiting_packet;
  //NotifierSignal _nonempty_signal;
  //Task _task;

  //unsigned _verbose;

  void push_packet(int port, Packet* p);
  //void check();

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

private:
  void push_waiting_packet(int port, Packet* p);
  void push_control_packet(int port, uint8_t type);

};

CLICK_ENDDECLS
#endif
