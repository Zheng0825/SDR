/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/fromlabviewphy.hh,v $
 *  
 ***********************************************************/

#ifndef FROMLABVIEWPHY_HH
#define FROMLABVIEWPHY_HH
#include <click/element.hh>

#include "fromphy.hh"
CLICK_DECLS

class FromLabviewPhy : public FromPhy {

public:

  FromLabviewPhy();
  ~FromLabviewPhy();

  const char *class_name() const { return "FromLabviewPhy"; }
  const char *port_count() const { return "1/2"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);

  void push(int, Packet *);

  void add_handlers();

protected:
  //unsigned _verbose;
  struct timeval _rx_start;

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
