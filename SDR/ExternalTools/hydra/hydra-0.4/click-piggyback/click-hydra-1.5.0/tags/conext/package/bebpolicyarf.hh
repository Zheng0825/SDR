/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_BEBPOLICYARF_HH
#define CLICK_BEBPOLICYARF_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "bebpolicy.hh"

CLICK_DECLS

class RateControlArf;

class BEBPolicyArf : public BEBPolicy { 
public:
  
  BEBPolicyArf();
  ~BEBPolicyArf();
  
  const char *class_name() const		{ return "BEBPolicyArf"; }
  //const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);
  //Packet *simple_action(Packet *);

  //void add_handlers();

  // for retransmission of MACTXScheduler
  void success(const Packet* p);
  void fail(const Packet* p);
  //bool retry(const Packet* p);
  //void reset_retry(const Packet* p);

  // for control message such as backoff slot number or time slot
  // return packet must be uniquefied.
  //Packet* process_control(Packet* p);
  //Packet* set_control(Packet* p);

protected:
  //static String read_param(Element*, void*);
  //static int write_param(const String&, Element*, void*, ErrorHandler*);
  //unsigned int _max_tries;
  //unsigned int _num_tries;

  //int _num_success;
  //int _num_fail;
  //int _num_tx;

  //unsigned _verbose;

protected:
  //unsigned int _max_cw;
  //unsigned int _min_cw;
  //unsigned int _cw;

private:
  RateControlArf* _ratecontrol;
};

CLICK_ENDDECLS
#endif
