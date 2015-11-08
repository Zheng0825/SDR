/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_RATECONTROLTBAR_HH
#define CLICK_RATECONTROLTBAR_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "ratecontrol.hh"

#include <vector>

CLICK_DECLS


class RateControlTbar : public RateControl { 

public:
  
  RateControlTbar();
  ~RateControlTbar();
  
  const char *class_name() const		{ return "RateControlTbar"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *p);

  void select_txrate(Packet* p_in);
  WritablePacket* get_txrate(Packet *p_in);

  //void add_handlers();

protected:
  //static String read_handler(Element *e, void *);  
  //static int write_handler(const String &arg, Element *e,
  //			   void *, ErrorHandler *errh);
  unsigned short calculate_datarate(double snr);

protected:
  enum {TBAR_6M = 0, TBAR_9M, TBAR_12M, TBAR_18M, 
        TBAR_24M, TBAR_36M, TBAR_48M, TBAR_54M, TBAR_Rate_Size};

  float _snr;
  uint16_t _rate;

  Vector<float> _snr_vector;

};

CLICK_ENDDECLS
#endif
