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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/ratecontrol.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_RATECONTROL_HH
#define CLICK_RATECONTROL_HH
#include <click/element.hh>
#include <click/glue.hh>

//#include <click/etheraddress.hh>
//#include <click/bighashmap.hh>
//#include <click/hydra_anno.hh>

CLICK_DECLS

/*
=c
RateControl([, I<KEYWORDS>])

=s Hydra

base class for bit-rate selection algorithm

=d 
Description goes here

=a SetRateControl
*/


class RateControl : public Element { 

public:
  
  RateControl();
  ~RateControl();
  
  const char *class_name() const		{ return "RateControl"; }
  const char *port_count() const		{ return "-/-"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *p);

  void add_handlers();
  
  virtual WritablePacket* get_txrate(Packet *p_in);


protected:
  unsigned _verbose;

  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
  
};

CLICK_ENDDECLS
#endif