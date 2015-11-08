/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframects.hh,v $
 *  
 ***********************************************************/


#ifndef CLICK_MACFRAMECTS_HH
#define CLICK_MACFRAMECTS_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <clicknet/80211.h>

#include "macframe80211.hh"

CLICK_DECLS

class MACFrameCTS : public MACFrame80211 {
public:
  MACFrameCTS();
  ~MACFrameCTS();

  const char *class_name() const		{ return "MACFrameCTS"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

  void add_handlers();

  WritablePacket* prepare_packet(Packet* p);
  WritablePacket* prepare_packet(Packet* p, EtherAddress& dst);
  const unsigned char* dst(Packet* p);  
  const unsigned char* src(Packet* p);  
  WritablePacket* set_header(Packet* p);
  int get_header(Packet* p, void* hdr);
  int header_size() {return _cts_hdr_size;}

protected:

  static String read_param(Element *e, void *vparam);
  int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

  struct cts_80211 _cts_hdr;
  size_t _cts_hdr_size;
private:
};

CLICK_ENDDECLS
#endif
