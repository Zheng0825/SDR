/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/ratecontrolrssi.hh,v $ 
 *  
 ***********************************************************/

#ifndef CLICK_RATECONTROLRSSI_HH
#define CLICK_RATECONTROLRSSI_HH
#include <click/element.hh>
#include <click/glue.hh>

#include <hydra/macframe.hh>
#include <hydra/ratecontrol.hh>

#include <vector>

CLICK_DECLS


class RateControlIN : public RateControl { 

public:
  
  RateControlIN();
  ~RateControlIN();
  
  const char *class_name() const		{ return "RateControlIN"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *p);

  void set_txrate(Packet* p_in);
  WritablePacket* get_txrate(Packet *p_in);

  void add_handlers();

protected:
  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
private:

  void init_tables();
  void interpolate_bers(double snr, std::vector<double>& bers);  
  //unsigned short calculate_pers(unsigned int len, std::vector<double>& error_rates, double cutoff);

  unsigned short calculate_bers(unsigned int len, std::vector<double>& bers);
  unsigned short calculate_pers(unsigned int len, std::vector<double>& bers);

  unsigned short calculate_datarate_per(double snr, unsigned int data_length);
  unsigned short calculate_datarate_ber(double snr, unsigned int data_length);
                            
  std::vector<int> _rate_index;
  std::vector<int> _snr_index;

  std::vector<double> _ber;

  double _cutoff;                                                  


private:
  double _snr;
  unsigned _rate;

  MACFrame* _mac_frame;

  // not used
  Vector<unsigned short> _rates;
  Vector<unsigned short> _max_rssi;
  
};

CLICK_ENDDECLS
#endif
