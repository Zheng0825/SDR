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

#ifndef CLICK_RATECONTROLRBAR_HH
#define CLICK_RATECONTROLRBAR_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "ratecontrol.hh"

#include <vector>

CLICK_DECLS

class MACFrameRTSRbar;
class MACFrameCTSRbar;

class RateControlRbar : public RateControl { 

public:
  
  RateControlRbar();
  ~RateControlRbar();
  
  const char *class_name() const		{ return "RateControlRbar"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *p);

  void select_txrate(Packet* p_in);
  void set_txrate(Packet* p_in);
  WritablePacket* get_txrate(Packet *p_in);

  //void add_handlers();

protected:
  //static String read_handler(Element *e, void *);  
  //static int write_handler(const String &arg, Element *e,
  //			   void *, ErrorHandler *errh);
  unsigned short calculate_datarate(double snr);
  uint8_t convert_rate(uint16_t anno_rate);
  uint16_t unconvert_rate(uint8_t rbar_rate);

protected:
  enum {RBAR_6M = 0, RBAR_9M, RBAR_12M, RBAR_18M, 
        RBAR_24M, RBAR_36M, RBAR_48M, RBAR_54M, RBAR_Rate_Size};

  float _snr;
  uint16_t _rate;

  MACFrameRTSRbar* _rts_frame;
  MACFrameCTSRbar* _cts_frame;
  Vector<float> _snr_vector;

};

CLICK_ENDDECLS
#endif
