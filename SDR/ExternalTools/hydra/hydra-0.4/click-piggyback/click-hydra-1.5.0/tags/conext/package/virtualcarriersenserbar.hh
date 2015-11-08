/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_VIRTUALCARRIERSENSERBAR_HH
#define CLICK_VIRTUALCARRIERSENSERBAR_HH

#include <click/element.hh>

#include "virtualcarriersense.hh"

CLICK_DECLS

class SetDurationRbar;
class VirtualCarrierSenseRbar : public VirtualCarrierSense { 
public:
  
  VirtualCarrierSenseRbar();
  ~VirtualCarrierSenseRbar();

  const char *class_name() const	{ return "VirtualCarrierSenseRbar"; }

  int configure(Vector<String> &, ErrorHandler *);

  void push(int, Packet *);

protected:
  //enum CSState {CS_IDLE, CS_BUSY};
  //CSState _cs_state;

  //MACTimer _timer;
  //unsigned _verbose;
  SetDurationRbar* _duration;

  //Packet* prepare_packet(uint8_t type);
  //static String read_param(Element *e, void *vparam);
  //static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);
private:

};

CLICK_ENDDECLS
#endif
