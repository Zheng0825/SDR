/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 * 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
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

#include "macframe.hh"
#include "ratecontrol.hh"

#include <vector>

CLICK_DECLS


class RateControlRSSI : public RateControl { 

public:
  
  RateControlRSSI();
  ~RateControlRSSI();
  
  const char *class_name() const		{ return "RateControlRSSI"; }
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
