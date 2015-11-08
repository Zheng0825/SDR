/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_SETPOWERSCEN_HH
#define CLICK_SETPOWERSCEN_HH
#include <click/element.hh>
#include <click/glue.hh>

CLICK_DECLS

class PowerControl;

class SetPowerScen : public Element { public:
  
  SetPowerScen();
  ~SetPowerScen();
  
  const char *class_name() const		{ return "SetPowerScen"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *);

  void add_handlers();

protected:
  PowerControl* _power_control;
  unsigned _verbose;

  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
private:
    unsigned _packet_count;
    unsigned _cur_power;
    float _cur_target_snr;
    float _c_map_power;
    unsigned _scen_num;
    float  _tx_amp; 
    float _tx_pow; 
    float _tx_loss;
    bool _scen1_down;

  unsigned scen_stairstep();
};

CLICK_ENDDECLS
#endif
