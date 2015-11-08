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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setratecontrol.cc,v $ 
 *  
 ***********************************************************/
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/packet_anno.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>

#include "ratecontrol.hh"
#include "setratecontrol.hh"

CLICK_DECLS

SetRateControl::SetRateControl()
  : _rate_control(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

SetRateControl::~SetRateControl()
{
}

int
SetRateControl::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* e = 0;
  if (cp_va_parse(conf, this, errh,
  		  cpElement, "rate control algorithm", &e,
		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }

  if (e) { 
    _rate_control = (RateControl *) e->cast("RateControl");
    if(!_rate_control) return errh->error(" Ratecontrol must be set using RateControl");
  } else {
    return errh->error("RateControl must be defined");
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
SetRateControl::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  //FIXME: Is WritablePacket need for modification of annotations?

  WritablePacket* p_out = _rate_control->get_txrate(p_in);

  hydra_chatter(9, _verbose, "%{element}::simple_action p_in=0x%x, p_out=0x%x", this, p_in, p_out);

  return p_out;
}

enum {H_VERBOSE};

String
SetRateControl::read_handler(Element *e, void *thunk)
{
  SetRateControl *src = (SetRateControl *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
SetRateControl::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  SetRateControl *src = (SetRateControl *) e;
  String s = cp_uncomment(arg);
  switch((int)vparam) {
  case H_VERBOSE: {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("verbose parameter must be unsigned");
    src->_verbose = m;
    break;
  }
  }
  return 0;
}

void
SetRateControl::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetRateControl)

