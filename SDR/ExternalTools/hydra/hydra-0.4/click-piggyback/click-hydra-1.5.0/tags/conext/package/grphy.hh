/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef GRPHY_HH
#define GRPHY_HH
#include <click/element.hh>
#include "macnotifier.hh"

#include <vector>

CLICK_DECLS


class GrPhy : public Element, public MACNotifier {

public:

  GrPhy();
  ~GrPhy();

  void* cast(const char *n);
  const char *class_name() const { return "GrPhy"; }
  const char *port_count() const { return "2/2"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);

  void push(int, Packet *);

  void add_handlers();

protected:

  // in the final version this message types should be included in mpi.h
  //enum MPIMessage{MSG_TX_DATA = 0, MSG_TX_DONE, MSG_RX_DATA, MSG_CS_BUSY, MSG_CS_IDLE};
  enum MPIPort{TO_PHY_PORT, FROM_PHY_PORT};

  // For state machine
  enum MPIState {STATE_IDLE, STATE_TX, STATE_RX, STATE_SIZE};
  typedef void (GrPhy::*run_state_fptr) (int port, Packet* pkt, uint8_t type);

  // For state and executions
  class RunState {
    public:
      GrPhy::MPIState _state;
      GrPhy::run_state_fptr _fptr;
      const RunState& operator=(const RunState& rhs)
      {
        if(this != &rhs)                                       
        {                                                      
          this->_state = rhs._state;   
          this->_fptr = rhs._fptr;                           
        }                                                      
        return *this;                                          
      }            
  };
 
  MPIState _cur_state;
  std::vector<RunState> _states;

  Packet* _tx_pkt; 
   
  // For debugging
  unsigned _verbose;
  //struct timeval _rx_start;

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

  struct timeval _debug_tv;
  int _debug_tx_len;
  int _debug_rx_len;
protected:
  void init_states();
  void add_state(MPIState state, run_state_fptr fptr);
  void run_states(int port, Packet *pkt);

private:
  void run_state_idle(int port, Packet *pkt, uint8_t type);
  void run_state_tx(int port, Packet *pkt, uint8_t type);
  void run_state_rx(int port, Packet *pkt, uint8_t type);

  // FIXME: for temporary
  void legacy_on_to_phy_message(Packet* pkt);
  void legacy_on_from_phy_message(Packet* pkt);
  void push_cs_idle(int port);


};

CLICK_ENDDECLS
#endif
