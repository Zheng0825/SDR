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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setratecontrol.hh,v $ 
 *  
 ***********************************************************/
#ifndef CLICK_SETRATECONTROL_HH
#define CLICK_SETRATECONTROL_HH
#include <click/element.hh>
#include <click/glue.hh>
CLICK_DECLS

/*
=c
SetRateControl([I<KEYWORDS>])

=s Hydra

Sets the bit-rate for a packet.

=d

Sets the hydra rate annotation on a packet.

Regular Arguments:
=over 8

=item RateControl
Element. 

=back 8

=h rate read/write

=a AutoRateFallback, MadwifiRate, ProbeRate, ExtraEncap
*/

class RateControl;

class SetRateControl : public Element { public:
  
  SetRateControl();
  ~SetRateControl();
  
  const char *class_name() const		{ return "SetRateControl"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *);

  void add_handlers();

protected:
  RateControl* _rate_control;
  unsigned _verbose;

  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
private:
};

CLICK_ENDDECLS
#endif
