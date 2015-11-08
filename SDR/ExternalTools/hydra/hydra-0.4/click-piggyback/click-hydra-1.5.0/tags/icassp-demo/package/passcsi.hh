#ifndef HYDRA_PASSCSI_HH
#define HYDRA_PASSCSI_HH
#include <click/element.hh>
#include <click/glue.hh>
CLICK_DECLS

/*
=c

PassCSI()

=a 
*/

class PassCSI : public Element { public:
  
  PassCSI();
  ~PassCSI();
  
  const char *class_name() const		{ return "PassCSI"; }
  const char *port_count() const		{ return "2/1"; }
  const char *processing() const		{ return "al/a"; }
  
  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  void add_handlers();

private:
  unsigned _verbose;

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
