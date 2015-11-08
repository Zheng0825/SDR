/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/hydradecap.hh,v $ 
 *  
 ***********************************************************/
#ifndef CLICK_HYDRADECAP_HH
#define CLICK_HYDRADECAP_HH
#include <click/element.hh>
#include <clicknet/ether.h>

CLICK_DECLS

/*
=c

HydraDecap

=s Hydra

Turns 802.11 packets into ethernet packets 

=d

=e


  wifi_cl :: Classifier (0/00%0c, 
                         0/04%0c,
                         0/08%0c);

  wifi_cl [0] -> Discard; //mgt 
  wifi_cl [1] -> Discard; //ctl
  wifi_cl [2] -> wifi_decap :: HydraDecap() -> ...

=a HydraDencap
 */

class MACFrame;

class HydraDecap : public Element { 

public:
  
  HydraDecap();
  ~HydraDecap();

  const char *class_name() const	{ return "HydraDecap"; }
  const char *port_count() const	{ return "1/1"; }
  const char *processing() const	{ return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);
  bool can_live_reconfigure() const	{ return true; }

  Packet *simple_action(Packet *);

  void add_handlers();

protected:
  static String read_param(Element *e, void *thunk);
  static int write_param(const String &, Element *, void *, ErrorHandler *);

private:
  MACFrame* _mac_frame;

  bool _strict;
  unsigned _verbose;
};

CLICK_ENDDECLS
#endif
