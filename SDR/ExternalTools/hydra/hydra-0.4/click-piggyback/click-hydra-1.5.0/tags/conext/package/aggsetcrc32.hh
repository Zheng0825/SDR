#ifndef CLICK_AGGSETCRC32_HH
#define CLICK_AGGSETCRC32_HH
#include <click/element.hh>

CLICK_DECLS

class AggSetCRC32 : public Element { public:
  
  AggSetCRC32();
  ~AggSetCRC32();

  const char *class_name() const	{ return "AggSetCRC32"; }
  const char *port_count() const	{ return PORTS_1_1; }
  const char *processing() const	{ return AGNOSTIC; }
 
  Packet *simple_action(Packet *);

};

CLICK_ENDDECLS
#endif
