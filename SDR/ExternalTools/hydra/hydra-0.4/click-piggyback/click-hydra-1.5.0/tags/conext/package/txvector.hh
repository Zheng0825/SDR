/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/txvector.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_TXVECTOR_HH
#define CLICK_TXVECTOR_HH
#include <click/element.hh>
#include <click/atomic.hh>

#include "mpivector.hh"

CLICK_DECLS

/*
 * =c
 * TXVector()
 * =s 
 * convert or generate MAC frame
 * =d
 * Description goes here.
 * =a TXVector
 */


class TXVector : public MPIVector {
public:
  TXVector();
  ~TXVector();

  const char *class_name() const		{ return "TXVector"; }
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
  uint8_t convert_rate(uint16_t anno_rate);
  uint8_t convert_type(uint8_t anno_type);
};

CLICK_ENDDECLS
#endif
