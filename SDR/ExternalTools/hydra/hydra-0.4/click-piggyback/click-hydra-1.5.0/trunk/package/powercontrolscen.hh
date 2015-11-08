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
 
#ifndef CLICK_POWERCONTROLSCEN_HH
#define CLICK_POWERCONTROLSCEN_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "powercontrol.hh"
#include <click/hydra_anno.hh>

CLICK_DECLS

class PowerControlScen : public PowerControl { 

public:
  
  PowerControlScen();
  ~PowerControlScen();
  
  const char *class_name() const		{ return "PowerControlScen"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }
 
  void* cast(const char* n); 
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *);

  WritablePacket* get_power(Packet *p_in);
  void set_power(unsigned power);

  void add_handlers();

protected:
  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);

protected:
  unsigned _verbose;
  unsigned _power;

private:
  unsigned _scen_num;

  // for scen 1
  unsigned _scen1_pkt_count;
  float _scen1_cur_target_snr;
  float _scen1_map_power;
  float _scen1_tx_amp; 
  float _scen1_tx_pow; 
  float _scen1_tx_loss;
  float _scen1_highest_snr;
  float _scen1_lowest_snr;
  float _scen1_snr_step;
  unsigned _scen1_num_packets;
  bool _scen1_down;

private:
  unsigned scen1_stairstep(unsigned cur_power);
};

CLICK_ENDDECLS
#endif
