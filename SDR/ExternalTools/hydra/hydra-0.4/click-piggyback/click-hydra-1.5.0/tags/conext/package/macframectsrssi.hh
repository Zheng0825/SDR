/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframectsrssi.hh,v $ 
 *  
 ***********************************************************/


#ifndef CLICK_MACFRAMECTSRSSI_HH
#define CLICK_MACFRAMECTSRSSI_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <clicknet/80211.h>

#include "macframects.hh"

CLICK_DECLS

class MACFrameCTSRSSI : public MACFrameCTS {
public:
  MACFrameCTSRSSI();
  ~MACFrameCTSRSSI();

  const char *class_name() const		{ return "MACFrameCTSRSSI"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

  void add_handlers();

  WritablePacket* prepare_packet(Packet* p);
  WritablePacket* prepare_packet(Packet* p, EtherAddress& dst, double snr);

  WritablePacket* set_header(Packet* p);
  int get_header(Packet* p, void* hdr);
  int header_size() {return sizeof(struct cts_80211_rssi);}
  int get_feedback(Packet* p, void* feedback);

protected:

  static String read_param(Element *e, void *vparam);
  int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

  struct cts_80211_rssi _cts_rssi_hdr;
  size_t _cts_rssi_hdr_size;
  size_t _rssi_len;
private:
};

CLICK_ENDDECLS
#endif
