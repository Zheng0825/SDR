/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setduration.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_SETDURATION_HH
#define CLICK_SETDURATION_HH
#include <click/element.hh>
#include <click/glue.hh>

CLICK_DECLS

/*
=c

SetDuration(Unsigned)

=s Hydra

set defer

=d

=h defer_time read/write

=a 
*/

class MACFrame80211;
class SetDefer;
class PHYProcessTime;

class SetDuration : public Element { public:
  
  SetDuration();
  ~SetDuration();
  
  const char *class_name() const		{ return "SetDuration"; }
  const char *port_count()  const 		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }
  const char *flow_code()  const 		{ return "#/#"; }
  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  virtual const Packet* adjust_duration(const Packet* p);
  virtual const Packet* calculate_duration(const Packet* p, const Packet* p_aux);
  virtual unsigned calculate_timeout(const Packet* p, bool next);

  void add_handlers();

protected:
  SetDefer* _sifs_defer;
  PHYProcessTime* _phy_time;
  unsigned _verbose;

  inline uint32_t real_reachtime(int len, uint8_t rate);

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
