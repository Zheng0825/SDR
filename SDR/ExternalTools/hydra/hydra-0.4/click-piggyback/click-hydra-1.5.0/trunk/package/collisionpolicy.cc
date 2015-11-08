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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/collisionpolicy.cc,v $
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
#include "collisionpolicy.hh"
CLICK_DECLS

CollisionPolicy::CollisionPolicy()
  : _max_tries(8), _num_tries(0), 
    _num_success(0), _num_fail(0), _num_tx(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

CollisionPolicy::~CollisionPolicy()
{
}


void*
CollisionPolicy::cast(const char *n)
{
    if (strcmp(n, "CollisionPolicy") == 0)
	return (CollisionPolicy *) this;

    return 0;
}

int
CollisionPolicy::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
 		  "MAX_RETRIES", cpUnsigned, "max retry number", &_max_tries,
		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  if(_max_tries == 0)
    return errh->error("MAX_TRIES must be greater than 0");

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
CollisionPolicy::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  Packet* pkt_to_send = 0;

  if (p) 
  {
    struct click_hydra_anno *cha = (struct click_hydra_anno *) p->all_user_anno();
    //cha->magic = HYDRA_MAGIC;

    _num_tx++;
    pkt_to_send = set_control(p);
    
  }

  return pkt_to_send;
}

void 
CollisionPolicy::success(const Packet* )
{
  _num_success++;
  _num_tries = 1;

}

void 
CollisionPolicy::fail(const Packet* )
{
  _num_fail++;
  _num_tries++;
}

bool 
CollisionPolicy::retry(const Packet* )
{
  return (_num_tries <= _max_tries);
}

void 
CollisionPolicy::reset_retry(const Packet* )
{
  _num_tries = 1;
}


Packet* 
CollisionPolicy::process_control(Packet*)
{
  Packet* pkt_to_send = 0;

  return pkt_to_send; 
}

Packet* 
CollisionPolicy::set_control(Packet* p)
{
  Packet* pkt_to_send = 0;

  if(_num_tries <= _max_tries)
    pkt_to_send = p;

  return pkt_to_send; 
}

String
CollisionPolicy::read_param(Element *e, void *thunk)
{
  CollisionPolicy *cp = (CollisionPolicy *)e;
  switch ((uintptr_t) thunk) {
  case 0:
    return String(cp->_verbose) + "\n";
  default:
    return String();
  }
  
  return String();
}

int 
CollisionPolicy::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  CollisionPolicy *cp = (CollisionPolicy *)e;
  String s = cp_uncomment(in_s);
  switch((int)vparam) {
  case 0: {
    unsigned verbose;
    if (!cp_unsigned(s, &verbose)) 
      return errh->error("verbose parameter must be boolean");
    cp->_verbose = verbose;
    break;
  }
  }
    return 0;

}
void
CollisionPolicy::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CollisionPolicy)

