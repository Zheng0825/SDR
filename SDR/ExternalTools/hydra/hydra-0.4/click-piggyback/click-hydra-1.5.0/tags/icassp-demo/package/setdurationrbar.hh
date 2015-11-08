/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 

#ifndef CLICK_SETDURATIONRBAR_HH
#define CLICK_SETDURATIONRBAR_HH
#include <click/element.hh>
#include <click/glue.hh>
#include "setduration1.hh"

CLICK_DECLS


//class MACFrame80211;
//class PHYProcessTime1;
//class SetDefer;

class SetDurationRbar : public SetDuration1 { public:
  
  SetDurationRbar();
  ~SetDurationRbar();
  
  const char *class_name() const		{ return "SetDurationRbar"; }
  //const char *processing() const		{ return AGNOSTIC; }
  //const char *flow_code()  const 		{ return "#/#"; }
  void* cast(const char *n);
  
  //int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  enum {RBAR_6M = 0, RBAR_9M, RBAR_12M, RBAR_18M, 
        RBAR_24M, RBAR_36M, RBAR_48M, RBAR_54M};

  const Packet* adjust_duration(const Packet* p);
  const Packet* calculate_duration(const Packet* p, const Packet* p_aux);
  uint32_t calculate_duration(const Packet* p);
  unsigned calculate_timeout(const Packet* p, bool next);

  //void add_handlers();

      
protected:
  //SetDefer* _sifs_defer;
  //PHYProcessTime1* _phy_time;
  //unsigned _verbose;

  //inline uint32_t real_txtime(int len, uint8_t rate);
  //inline uint32_t real_rxtime(int len, uint8_t rate);
  //inline uint32_t real_airtime(int len, uint8_t rate);
  //inline uint32_t real_reachtime(int len, uint8_t rate);

  //static String read_param(Element *e, void *);  
  //static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
  uint8_t convert_rate(uint16_t anno_rate);
  uint16_t unconvert_rate(uint8_t rbar_rate);
};

CLICK_ENDDECLS
#endif
