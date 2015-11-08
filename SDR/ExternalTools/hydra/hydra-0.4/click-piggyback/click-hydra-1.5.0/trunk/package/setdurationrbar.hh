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
 

#ifndef CLICK_SETDURATIONRBAR_HH
#define CLICK_SETDURATIONRBAR_HH
#include <click/element.hh>
#include <click/glue.hh>
#include "setduration1.hh"

CLICK_DECLS


//class MACFrame80211;
//class PHYProcessTime1;
//class SetDefer;

class SetDurationRbar : public SetDuration1 { public:
  
  SetDurationRbar();
  ~SetDurationRbar();
  
  const char *class_name() const		{ return "SetDurationRbar"; }
  //const char *processing() const		{ return AGNOSTIC; }
  //const char *flow_code()  const 		{ return "#/#"; }
  void* cast(const char *n);
  
  //int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  enum {RBAR_6M = 0, RBAR_9M, RBAR_12M, RBAR_18M, 
        RBAR_24M, RBAR_36M, RBAR_48M, RBAR_54M};

  const Packet* adjust_duration(const Packet* p);
  const Packet* calculate_duration(const Packet* p, const Packet* p_aux);
  uint32_t calculate_duration(const Packet* p);
  unsigned calculate_timeout(const Packet* p, bool next);

  //void add_handlers();

      
protected:
  //SetDefer* _sifs_defer;
  //PHYProcessTime1* _phy_time;
  //unsigned _verbose;

  //inline uint32_t real_txtime(int len, uint8_t rate);
  //inline uint32_t real_rxtime(int len, uint8_t rate);
  //inline uint32_t real_airtime(int len, uint8_t rate);
  //inline uint32_t real_reachtime(int len, uint8_t rate);

  //static String read_param(Element *e, void *);  
  //static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
  uint8_t convert_rate(uint16_t anno_rate);
  uint16_t unconvert_rate(uint8_t rbar_rate);
};

CLICK_ENDDECLS
#endif
