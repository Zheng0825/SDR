/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 

#ifndef CLICK_SETDURATION1_HH
#define CLICK_SETDURATION1_HH
#include <click/element.hh>
#include <click/glue.hh>
#include "setduration.hh"
CLICK_DECLS

/*
=c

SetDuration1(Unsigned)

// change for gnuradio 

=s Hydra

set defer

=d

=h defer_time read/write

=a 
*/

//class MACFrame80211;
class PHYProcessTime1;
//class SetDefer;

class SetDuration1 : public SetDuration { public:
  
  SetDuration1();
  ~SetDuration1();
  
  const char *class_name() const		{ return "SetDuration1"; }
  const char *processing() const		{ return AGNOSTIC; }
  const char *flow_code()  const 		{ return "#/#"; }
  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  const Packet* adjust_duration(const Packet* p);
  const Packet* calculate_duration(const Packet* p, const Packet* p_aux);
  unsigned calculate_timeout(const Packet* p, bool next);

  //void add_handlers();

protected:
  //SetDefer* _sifs_defer;
  PHYProcessTime1* _phy_time;
  //unsigned _verbose;

  uint32_t real_txtime(int len, uint8_t rate);
  uint32_t real_rxtime(int len, uint8_t rate);
  uint32_t real_airtime(int len, uint8_t rate);
  uint32_t real_reachtime(int len, uint8_t rate);

  //static String read_param(Element *e, void *);  
  //static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
