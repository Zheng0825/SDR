/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 

#ifndef CLICK_RATECONTROLARF_HH
#define CLICK_RATECONTROLARF_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <click/timer.hh>

#include "ratecontrol.hh"
#include <click/hydra_anno.hh>

CLICK_DECLS

class RateControlArf : public RateControl { 

public:
  
  RateControlArf();
  ~RateControlArf();
  
  const char *class_name() const		{ return "RateControlArf"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler*);

  Packet *simple_action(Packet *p);
  void run_timer(Timer* );

  WritablePacket* get_txrate(Packet *p_in);

  void add_handlers();

  void success();
  void fail();

protected:
  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
protected:
  Timer _timer;
  struct timeval _timeout_val;

  unsigned _rate;
  unsigned _num_successes;
  unsigned _num_fails;
  unsigned _cur_successes;
  unsigned _cur_fails;
  bool _first_try;

private:
  enum {ARF_6M=0, ARF_9M, ARF_12M, ARF_18M, 
        ARF_24M, ARF_36M, ARF_48M, ARF_54M, 
        NUM_ARF_RATE}; 
  uint8_t rate_to_index(uint16_t anno_rate);
  uint16_t index_to_rate(uint8_t index);
  void increase_rate();
  void decrease_rate();

};

CLICK_ENDDECLS
#endif
