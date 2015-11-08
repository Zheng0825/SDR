/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/rxstate.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_RXSTATE_HH
#define CLICK_RXSTATE_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/task.hh>
#include <click/notifier.hh>

CLICK_DECLS


class RXState : public Element { 

public:
  
  RXState();
  ~RXState();
  
  const char *class_name() const	{ return "RXState"; }
  const char *port_count() const	{ return "1/0"; }
  const char *processing() const	{ return AGNOSTIC; }
  const char *flow_code() const		{ return "x/y"; }

  int configure(Vector<String>&, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  bool run_task();

  void push(int, Packet *);
  bool& state() { return _state; }

private:
  bool _state; 
  unsigned _verbose;

  NotifierSignal _nonempty_signal;
  Task _task;
  
};

CLICK_ENDDECLS
#endif
