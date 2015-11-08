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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/calculaterssi.cc,v $ 
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>

#include "calculaterssi.hh"

CLICK_DECLS

CalculateRSSI::CalculateRSSI() : _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

CalculateRSSI::~CalculateRSSI()
{
}

int
CalculateRSSI::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
 		  "VERBOSE", cpUnsigned, "verbose noise?", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
CalculateRSSI::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if (p) 
  {
    struct click_hydra_anno *cha = (struct click_hydra_anno *) p->all_user_anno();
    //cha->magic = HYDRA_MAGIC;

    cha->avg_snr = calculate_rssi(cha->avg_snr);
  }
  return p;
}

double
CalculateRSSI::calculate_rssi(double snr)
{
    // if we have different matic, do that here
    return snr; 
}

enum {H_VERBOSE, H_NUMLEVEL};

String
CalculateRSSI::read_param(Element *e, void *thunk)
{
  CalculateRSSI *td = (CalculateRSSI *)e;
  switch ((uintptr_t) thunk) 
  {
    case H_VERBOSE:
      return String(td->_verbose) + "\n";
    default:
      return String();
  }

  return String();

}

int 
CalculateRSSI::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  CalculateRSSI *f = (CalculateRSSI *)e;
  String s = cp_uncomment(in_s);
  switch((int)vparam) 
  {
    case H_VERBOSE: 
    {
      unsigned m;
      if (!cp_unsigned(s, &m)) 
        return errh->error("verbose parameter must be unsigned");
      f->_verbose = m;
      break;
    }
  }

  return 0;

}
void
CalculateRSSI::add_handlers()
{
  add_read_handler("verbose?", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose?", write_param, (void *) H_VERBOSE);
  add_read_handler("threshold", read_param, (void *) H_NUMLEVEL);
  add_write_handler("threshold", write_param, (void *) H_NUMLEVEL);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CalculateRSSI)

