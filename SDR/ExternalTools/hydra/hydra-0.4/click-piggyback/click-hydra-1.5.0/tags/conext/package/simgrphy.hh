/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_SIMGRPHY_HH
#define CLICK_SIMGRPHY_HH
#include <click/element.hh>
#include <click/timer.hh>
#include "phyprocesstime.hh"

CLICK_DECLS

class SimGrPhy : public Element {

public:
  
  SimGrPhy();
  ~SimGrPhy();

  enum { PHY_IDLE, PHY_RECEIVE, PHY_TRANSMIT};
  enum { PORT_MAC = 0, PORT_CHANNEL = 1 };

  const char *class_name() const { return "SimGrPhy"; }
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
  unsigned _verbose;

  void start_tx_timer(Packet* pkt);
  void start_rx_timer(Packet* pkt);

  void process_mac(Packet* p);
  void process_channel(Packet* p);

  void push_type(int port, uint8_t type);
  void push_wrong_packet(int port, uint8_t type);

  //void push_packet(int port, Packet* p);
};

CLICK_ENDDECLS
#endif
