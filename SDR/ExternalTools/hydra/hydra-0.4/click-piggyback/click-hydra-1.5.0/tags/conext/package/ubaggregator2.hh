#ifndef HYDRA_UBAGGREGATOR2_HH
#define HYDRA_UBAGGREGATOR2_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "prioritizer.hh"

CLICK_DECLS

/*
=c

UBAggregator()

=a 
*/

class UBAggregator2 : public Element { public:
  
  UBAggregator2();
  ~UBAggregator2();
 
  const char *class_name() const		{ return "UBAggregator2"; }
  const char *port_count() const		{ return "2/1"; }
  const char *processing() const		{ return "ll/l"; }
  
  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *);

  void add_handlers();

  // Override the pull method to support aggregation
  Packet *pull(int port);

private:
  unsigned _max_broadcast;
  unsigned _max_unicast;
  unsigned _max_aggsize;
  unsigned _max_num;
  unsigned total_size; 
  unsigned _verbose;
  unsigned _unicast_len;
  unsigned _broadcast_len;
  Prioritizer *_prioritizer;

  Packet **_broadcast_buffer, **_unicast_buffer;

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);

  static Packet *make_mpdu_with_pad(Packet *, bool more_data);
  unsigned aggregate(unsigned char **, u_int32_t *, u_int32_t *);
};

CLICK_ENDDECLS
#endif
