/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/rxvector.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_RXVECTOR_HH
#define CLICK_RXVECTOR_HH
#include <click/element.hh>
#include <click/atomic.hh>

#include "mpivector.hh"

CLICK_DECLS

/*
 * =c
 * RXVector()
 * =s 
 * convert or generate MAC frame
 * =d
 * Description goes here.
 * =a RXVector
 */


class RXVector : public MPIVector {
public:
  RXVector();
  ~RXVector();

  const char *class_name() const		{ return "RXVector"; }
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
  uint16_t convert_rate(uint8_t rxv_rate);
  uint8_t convert_type(uint8_t rxv_type);
};

CLICK_ENDDECLS
#endif
