/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/virtualcarriersense.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_VIRTUALCARRIERSENSE_HH
#define CLICK_VIRTUALCARRIERSENSE_HH

#include <click/element.hh>

#include "mactimer.hh"
CLICK_DECLS

/*
=c

VirtualCarrierSense

=s Hydra

Virtual carrier sensing using 802.11 NAV

=d 

Description goes here.

=a CarrierSense
 */

class VirtualCarrierSense : public Element { public:
  
  VirtualCarrierSense();
  ~VirtualCarrierSense();

  const char *class_name() const	{ return "VirtualCarrierSense"; }
  const char *port_count() const	{ return "1/2"; }
  const char *processing() const	{ return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);

  void run_timer(Timer* );
  void push(int, Packet *);

  void add_handlers();

protected:
  enum CSState {CS_IDLE, CS_BUSY};
  CSState _cs_state;

  MACTimer _timer;
  //struct timeval _remain_val;
  //struct timeval _start_val;
  unsigned _verbose;

  Packet* prepare_packet(uint8_t type);
  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);
private:

};

CLICK_ENDDECLS
#endif
