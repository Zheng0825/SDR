/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/fromlabviewphy1.hh,v $
 *  
 ***********************************************************/

#ifndef FROMLABVIEWPHY1_HH
#define FROMLABVIEWPHY1_HH
#include <click/element.hh>

#include "fromphy.hh"
CLICK_DECLS

/*
 * FromLabviewPhy1
 *
 * 
 */

class FromLabviewPhy1 : public FromPhy {

public:

  FromLabviewPhy1();
  ~FromLabviewPhy1();

  const char *class_name() const { return "FromLabviewPhy1"; }
  const char *port_count() const { return "1/2"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);

  void push(int, Packet *);

  void add_handlers();

protected:
  //unsigned _verbose;
  void push_control_packet(int port, uint8_t type);

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
