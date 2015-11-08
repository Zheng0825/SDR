/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframe80211.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_MACFRAME80211_HH
#define CLICK_MACFRAME80211_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <clicknet/80211.h>

#include "macframe.hh"

CLICK_DECLS


class MACFrame80211 : public MACFrame {
public:
  MACFrame80211();
  ~MACFrame80211();

  const char *class_name() const		{ return "MACFrame80211"; }
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
  int header_size() { return sizeof(struct common_80211);}

  const unsigned char* duration(Packet* p);

protected:

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

private:
  struct common_80211 _common_hdr;
  size_t _common_hdr_size;
};

CLICK_ENDDECLS
#endif
