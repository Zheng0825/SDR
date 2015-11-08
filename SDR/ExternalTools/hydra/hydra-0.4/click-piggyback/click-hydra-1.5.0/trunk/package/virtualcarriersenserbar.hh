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
