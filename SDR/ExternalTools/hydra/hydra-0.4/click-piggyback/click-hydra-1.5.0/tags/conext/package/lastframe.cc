/*
 * checkcrc32.{cc,hh} -- element checks CRC 32 checksum
 * Robert Morris
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
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
 */

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/crc32.h>
#include <click/hydra_anno.hh>
#include "lastframe.hh"
CLICK_DECLS

LastFrame::LastFrame()
{
  hydra_chatter(9,8, "%{element}::constructor", this);
}

LastFrame::~LastFrame()
{
}

int LastFrame::configure(Vector<String> &conf, ErrorHandler *errh)
{

  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
 		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int LastFrame::initialize(ErrorHandler *errh)
{
   // fill out if necessary
   return 0;
}

Packet *
LastFrame::simple_action(Packet *p)
{

  // parse annotation for deaggregation
  struct click_hydra_anno* cha=
	  (struct click_hydra_anno *) p->all_user_anno();

  if(cha->agg_anno->lastframe!=1)
  {  
    p->kill();	  
    return 0;
  }
  
  return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(LastFrame)
//ELEMENT_MT_SAFE(LastFrame)
