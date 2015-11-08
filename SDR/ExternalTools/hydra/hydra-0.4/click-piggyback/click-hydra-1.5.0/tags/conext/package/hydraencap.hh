/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/hydraencap.hh,v $ 
 *  
 ***********************************************************/
#ifndef CLICK_HYDRAENCAP_HH
#define CLICK_HYDRAENCAP_HH
#include <click/element.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>

CLICK_DECLS

class MACFrame;

class HydraEncap : public Element { 
public:
  
  HydraEncap();
  ~HydraEncap();

  const char *class_name() const	{ return "HydraEncap"; }
  const char *port_count() const	{ return "1/1"; }
  const char *processing() const	{ return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);
  bool can_live_reconfigure() const	{ return true; }

  Packet *simple_action(Packet *);

  void add_handlers();

protected:
  MACFrame* _mac_frame;
  unsigned _verbose;

  static String read_param(Element*, void*);
  static int write_param(const String&, Element*, void*, ErrorHandler*);
  
private:
  Packet* prepare_llc(Packet* p, uint16_t ethtype);

  EtherAddress _daddr;
  uint16_t _ethtype;
  bool _predefined;
};

CLICK_ENDDECLS
#endif
