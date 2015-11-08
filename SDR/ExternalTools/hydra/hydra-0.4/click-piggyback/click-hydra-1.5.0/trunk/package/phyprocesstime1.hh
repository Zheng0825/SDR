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
 

#ifndef CLICK_PHYPROCESSTIME1_HH
#define CLICK_PHYPROCESSTIME1_HH
#include <click/element.hh>
#include <click/glue.hh>
#include "phyprocesstime.hh"

CLICK_DECLS


class PHYProcessTime1 : public PHYProcessTime { public:
  
  PHYProcessTime1();
  ~PHYProcessTime1();
  
  const char *class_name() const		{ return "PHYProcessTime1"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);
  //Packet *simple_action(Packet *);

  //inline unsigned tx_delay() {return _tx_delay;}
  //inline unsigned rx_delay() {return _rx_delay;}
  //inline unsigned mac_delay() {return _mac_delay;}
  //inline unsigned tx_encoding(int len){return _txe_slope * len + _txe_inter;}
  //inline unsigned rx_decoding(int len) {return _rxd_slope * len + _rxd_inter;}
  inline float symbol_extension() {return _symbol_extension;}

  //void add_handlers();

protected:
  //unsigned _tx_delay; 
  //unsigned _rx_delay; 
  //unsigned _mac_delay; 
  //unsigned _txe_slope;
  //unsigned _txe_inter;
  //unsigned _rxd_slope;
  //unsigned _rxd_inter;
  float    _symbol_extension;
  //unsigned _verbose;

  //static String read_param(Element *e, void *);  
  //static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
