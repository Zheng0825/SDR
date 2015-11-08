/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_FREQSELIL_HH
#define CLICK_FREQSELIL_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/timer.hh>
#include <click/dequeue.hh>

#include "loadmonitor.hh"

CLICK_DECLS

class FreqSelIL : public LoadMonitor {
public:
  FreqSelIL();
  ~FreqSelIL();

  const char *class_name() const { return "FreqSelIL"; }
  const char *port_count() const { return "2/2"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  void push(int, Packet *);

  void add_handlers();

protected:
  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

  //void change_state(unsigned char state, struct timeval tv);
  //float calc_load();
  void process_load(int port, Packet* pkt);
  void process_freq(int port, Packet* pkt);
  float check_load();
  unsigned char unmarshall_freq(Packet* pkt, float* freq);
  Packet* marshall_freq(unsigned char req_type, float* freq);

protected:
  //enum {CS_BUSY, CS_IDLE};
  //enum {RISING = CS_BUSY, FALLING = CS_IDLE};
  enum {LOAD_PORT = 0, FREQ_PORT = 1};
  enum {GET_FREQ = 1, SET_FREQ = 2};
  
  //struct edge 
  //{
  //  unsigned char _type;
  //  struct timeval _tv;
  //};

  //DEQueue<struct edge> _edges;

  // May not have if we don't consider CLAW
  //int _cur_state;
  //float _load;
  //struct timeval _window_tv;

  //int _verbose;

  float _load_thresh;
  float _cur_freq;

private: 

};

CLICK_ENDDECLS
#endif
