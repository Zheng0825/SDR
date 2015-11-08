/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_SETPOWER_HH
#define CLICK_SETPOWER_HH
#include <click/element.hh>
#include <click/glue.hh>

CLICK_DECLS

class PowerControl;

class SetPower : public Element { public:
  
  SetPower();
  ~SetPower();
  
  const char *class_name() const		{ return "SetPower"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *);

  void add_handlers();

protected:
  PowerControl* _power_control;
  unsigned _verbose;

  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
private:
    int _packet_count;
    int _power_probe;
};

CLICK_ENDDECLS
#endif
