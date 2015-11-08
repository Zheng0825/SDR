/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/ratecontrolconst.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_RATECONTROLCONST_HH
#define CLICK_RATECONTROLCONST_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "ratecontrol.hh"
#include <click/hydra_anno.hh>

CLICK_DECLS

/*
=c
RateControlConst([, I<KEYWORDS>])

=s Hydra

base class for bit-rate selection algorithm

=d 
Description goes here

=a SetRateControl
*/


class RateControlConst : public RateControl { 

public:
  
  RateControlConst();
  ~RateControlConst();
  
  const char *class_name() const		{ return "RateControlConst"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *p);

  WritablePacket* get_txrate(Packet *p_in);

  void add_handlers();

protected:
  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
private:
  unsigned _rate;
  struct click_hydra_rate _rate_struct;

  bool set_rate_struct();
  
};

CLICK_ENDDECLS
#endif
