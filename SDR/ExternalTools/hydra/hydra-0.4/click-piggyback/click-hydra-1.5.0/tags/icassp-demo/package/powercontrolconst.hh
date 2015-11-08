/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 

#ifndef CLICK_POWERCONTROLCONST_HH
#define CLICK_POWERCONTROLCONST_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "powercontrol.hh"
#include <click/hydra_anno.hh>

CLICK_DECLS

class PowerControlConst : public PowerControl { 

public:
  
  PowerControlConst();
  ~PowerControlConst();
  
  const char *class_name() const		{ return "PowerControlConst"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *p);

  WritablePacket* get_power(Packet *p_in);
  void set_power(unsigned power);

  void add_handlers();

protected:
  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
protected:
  unsigned _power;

};

CLICK_ENDDECLS
#endif
