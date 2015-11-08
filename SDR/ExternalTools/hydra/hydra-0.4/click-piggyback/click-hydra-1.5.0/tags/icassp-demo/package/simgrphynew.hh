/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_SIMGRPHYNEW_HH
#define CLICK_SIMGRPHYNEW_HH
#include <click/element.hh>
#include <click/timer.hh>
#include "phyprocesstime.hh"

CLICK_DECLS

class SimGrPhyNew : public Element {

public:
  
  SimGrPhyNew();
  ~SimGrPhyNew();

  enum { PHY_IDLE, PHY_RECEIVE, PHY_TRANSMIT};
  enum { PORT_MAC = 0, PORT_CHANNEL = 1 };

  const char *class_name() const { return "SimGrPhyNew"; }
  const char *port_count() const { return "2/2"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler* errh);

  int initialize(ErrorHandler* errh);

  void push(int port, Packet *p);
  void run_timer(Timer* );

private:
  Timer _timer;
  int _state;
  Packet* _pkt_buf;
  PHYProcessTime* _phy_time;
  unsigned _len_csi;
  unsigned _verbose;
  struct fake_rxvector{
     u_int32_t   type;
     u_int32_t   len_metadata;
     u_int32_t   len_broadcast;
     u_int32_t   len_unicast;
     u_int32_t   rate;
     float       snr;
     char        metadata[10];
  };
  
  void start_tx_timer(Packet* pkt);
  void start_rx_timer(Packet* pkt);

  void process_mac(Packet* p);
  void process_channel(Packet* p);

  void push_type(int port, uint32_t type);
  void push_wrong_packet(int port, uint32_t type);

  //void push_packet(int port, Packet* p);
};

CLICK_ENDDECLS
#endif
