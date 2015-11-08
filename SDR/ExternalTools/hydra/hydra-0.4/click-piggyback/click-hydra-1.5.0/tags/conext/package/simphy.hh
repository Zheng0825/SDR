/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/simphy.hh,v $ 
 *  
 ***********************************************************/
#ifndef CLICK_SIMPHY_HH
#define CLICK_SIMPHY_HH
#include <click/element.hh>
#include <click/timer.hh>

CLICK_DECLS

/*
 * SimPhy
 *
 * Simulate reliable interface to MAC across etherenet interface
 * 
 */

class SimPhy : public Element {

public:
  
  SimPhy();
  ~SimPhy();

  enum { PHY_IDLE, PHY_RECEIVE, PHY_TRANSMIT };
  enum { PORT_MAC = 0, PORT_CHANNEL = 1 };

  const char *class_name() const { return "SimPhy"; }
  const char *port_count() const { return "2/2"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);

  int initialize(ErrorHandler* errh);

  void push(int port, Packet *p);
  void run_timer(Timer* );

private:
  Timer _timer;
  int _state;
  unsigned _tx_wait;
  unsigned _rx_wait;
  Packet* _rcv_buf;

  struct timeval _tx_tv;
  struct timeval _rx_tv;

  void process_mac(Packet* p);
  void process_channel(Packet* p);
  void push_type(int port, uint8_t type);
  void push_packet(int port, Packet* p);
};

CLICK_ENDDECLS
#endif
