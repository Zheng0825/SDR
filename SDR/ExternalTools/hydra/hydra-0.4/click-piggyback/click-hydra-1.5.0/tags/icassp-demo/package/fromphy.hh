/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/fromphy.hh,v $
 *  
 ***********************************************************/

#ifndef FROMPHY_HH
#define FROMPHY_HH
#include <click/element.hh>
CLICK_DECLS


class FromPhy : public Element {

public:

  FromPhy();
  ~FromPhy();

  const char *class_name() const { return "FromPhy"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);

  void push(int, Packet *);

  void add_handlers();

protected:
  unsigned _verbose;

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

private:
  bool is_control(Packet* p);
};

CLICK_ENDDECLS
#endif
