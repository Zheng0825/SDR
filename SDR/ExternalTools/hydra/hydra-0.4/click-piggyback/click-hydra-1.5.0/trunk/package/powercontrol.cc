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
 

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/packet_anno.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>

#include "powercontrol.hh"

CLICK_DECLS

PowerControl::PowerControl()
  : _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

PowerControl::~PowerControl()
{
}

void*
PowerControl::cast(const char *n)
{
  if( strcmp(n, "PowerControl") == 0 )
    return (PowerControl *) this;
  else
    return 0;
}

int
PowerControl::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }


  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
PowerControl::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  return p_in;
}

WritablePacket*
PowerControl::get_power(Packet* p_in)
{
   
  WritablePacket* p_out = p_in->uniqueify();

  return p_out;
}

void PowerControl::set_power(unsigned )
{
  // set power 
}

enum {H_VERBOSE};

String
PowerControl::read_handler(Element *e, void *thunk)
{
  PowerControl *src = (PowerControl *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
PowerControl::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  PowerControl *src = (PowerControl *) e;
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
PowerControl::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PowerControl)

