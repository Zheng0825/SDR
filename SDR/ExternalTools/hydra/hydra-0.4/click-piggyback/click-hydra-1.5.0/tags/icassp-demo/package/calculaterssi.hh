/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/calculaterssi.hh,v $ 
 *  
 ***********************************************************/

#ifndef CLICK_CALCULATERSSI_HH
#define CLICK_CALCULATERSSI_HH
#include <click/element.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>

CLICK_DECLS

class CalculateRSSI : public Element { public:
  
  CalculateRSSI();
  ~CalculateRSSI();
  
  const char *class_name() const { return "CalculateRSSI"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  void add_handlers();

private:
  unsigned _verbose;

  double calculate_rssi(double snr);

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
