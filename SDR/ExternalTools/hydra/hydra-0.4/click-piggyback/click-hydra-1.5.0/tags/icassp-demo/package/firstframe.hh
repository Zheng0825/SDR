#ifndef CLICK_FIRSTFRAME_HH
#define CLICK_FIRSTFRAME_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <click/etheraddress.hh>
#include <clicknet/80211.h>
#include <click/vector.hh>

CLICK_DECLS

class FirstFrame : public Element {
public:
  FirstFrame();
  ~FirstFrame();

  const char *class_name() const { return "FirstFrame"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return AGNOSTIC; }
  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

protected:
  
  unsigned _verbose;

};

CLICK_ENDDECLS
#endif
