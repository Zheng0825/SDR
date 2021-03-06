#ifndef CLICK_DEAGGREGATOR_HH
#define CLICK_DEAGGREGATOR_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <click/etheraddress.hh>
#include <clicknet/80211.h>
#include <click/vector.hh>

CLICK_DECLS

class MACFrame80211;
class PowerControl;

class DeAggregator : public Element {
public:
  DeAggregator();
  ~DeAggregator();

  const char *class_name() const { return "DeAggregator"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return PUSH; }  //push
  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);
  
  void push(int, Packet*); 
protected:
  void success_rts(Packet* pkt);
  void success_cts(Packet* pkt);
  void success_data(Packet* pkt);
  void success_ack(Packet* pkt);
  //void fail_rts(Packet* pkt);
  //void fail_cts(Packet* pkt);
  //void fail_data(Packet* pkt);
  //void fail_data_for_draw(Packet* pkt);
  //void fail_ack(Packet* pkt);

  void fail(Packet* pkt);
  bool check_crc(Packet* pkt, uint16_t len);

protected:
  EtherAddress _my_mac_addr;

  String _filename;
  String _logfilename;
  FILE* _fptr_log;

  //fix this!!
  struct Unicast{
    WritablePacket* frag_data;
    int     len;
  };
  // Vector<WritablePacket *> _unicast; 
  Vector<Unicast> _unicast;

  MACFrame80211* _mac_rts;
  MACFrame80211* _mac_cts;
  MACFrame80211* _mac_data;
  MACFrame80211* _mac_ack;
  int _mac_data_hdr_size;

  char _send_pattern[IEEE80211_MTU];
  unsigned _verbose;

  PowerControl* _power_control;

private:
  atomic_uint32_t _drops;

};

CLICK_ENDDECLS
#endif
