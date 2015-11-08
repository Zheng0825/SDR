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
//#include <click/packet_anno.hh>
//#include <clicknet/ether.h>
//#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "powercontrolconst.hh"

CLICK_DECLS

PowerControlConst::PowerControlConst()
  : _power(0) 
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

PowerControlConst::~PowerControlConst()
{
}

void*
PowerControlConst::cast(const char *n)
{
  if( strcmp(n, "PowerControlConst") == 0 )
    return (PowerControlConst *) this;
  else
    return PowerControl::cast(n);
}

int
PowerControlConst::configure(Vector<String> &conf, ErrorHandler *errh)
{

  if (cp_va_parse(conf, this, errh,
		  cpUnsigned, "power for transmission", &_power, 
		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }

  if(!_power)
    return errh->error("power must be defined");


  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
PowerControlConst::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  return p_in;
}

WritablePacket*
PowerControlConst::get_power(Packet* p_in)
{
  hydra_chatter(8, _verbose, "%{element}::get_txrate: ", this);

  WritablePacket* p_out = p_in->uniqueify(); 

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();

  cha->power = (u_int16_t) _power;

  return p_out;

}

void PowerControlConst::set_power(unsigned power)
{
  _power = power;
}

enum {H_VERBOSE, H_TXPOWER};

String
PowerControlConst::read_handler(Element *e, void *thunk)
{
  PowerControlConst *src = (PowerControlConst *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    case H_TXPOWER: 
      return String(src->_power) + "\n";
    default:   return "\n";
  }
  
}

int
PowerControlConst::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  PowerControlConst *src = (PowerControlConst *) e;
  String s = cp_uncomment(arg);
  switch((int)vparam) 
  {
    case H_VERBOSE: 
    {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("verbose parameter must be unsigned");
    src->_verbose = m;
    break;
    }
    case H_TXPOWER: 
    {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("txpower parameter must be unsigned");
    src->_power = m;
    break;
    }
   
  }
  return 0;
}

void
PowerControlConst::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
  add_read_handler("txpower", read_handler, (void *) H_TXPOWER);
  add_write_handler("txpower", write_handler, (void *) H_TXPOWER);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PowerControlConst)

