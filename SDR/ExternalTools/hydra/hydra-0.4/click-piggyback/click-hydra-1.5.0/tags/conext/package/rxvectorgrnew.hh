/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 

#ifndef CLICK_RXVECTORGRNEW_HH
#define CLICK_RXVECTORGRNEW_HH
#include <click/element.hh>
#include <click/atomic.hh>

#include "mpivector.hh"

CLICK_DECLS

class RXVectorGrNew : public MPIVector {
public:
  RXVectorGrNew();
  ~RXVectorGrNew();

  const char *class_name() const		{ return "RXVectorGrNew"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

  void add_handlers();

  //WritablePacket* prepare_packet(Packet* p);
  Packet* prepare_packet(Packet* p);
  //int header_size() {return _hdr_size;}

protected:
  unsigned _hdr_size;
  //unsigned _verbose;

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

private:
  uint16_t convert_rate(uint32_t rxv_rate);
  uint8_t convert_type(uint32_t rxv_type);
};

CLICK_ENDDECLS
#endif
