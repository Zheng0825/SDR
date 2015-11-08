/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_LOADMONITOR_HH
#define CLICK_LOADMONITOR_HH
#include <click/element.hh>
#include <click/packet.hh>
#include <click/timer.hh>
#include <click/dequeue.hh>

CLICK_DECLS

class LoadMonitor : public Element {
public:
  LoadMonitor();
  ~LoadMonitor();

  const char *class_name() const { return "LoadMonitor"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  void push(int, Packet *);

  void add_handlers();

protected:
  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

  void change_state(unsigned char state, struct timeval tv);
  float calc_load();

protected:
  enum {CS_BUSY, CS_IDLE};
  enum {RISING = CS_BUSY, FALLING = CS_IDLE};
  
  struct edge 
  {
    unsigned char _type;
    struct timeval _tv;
  };

  DEQueue<struct edge> _edges;

  // May not have if we don't consider CLAW
  int _cur_state;
  float _load;
  struct timeval _window_tv;

  int _verbose;
private: 
};

CLICK_ENDDECLS
#endif
