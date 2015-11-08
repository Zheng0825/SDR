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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/fromphy.cc,v $
 *  
 ***********************************************************/

/*
 * mpirx.{cc,hh} --  Classifies packets from PHY
 *             across ethernet interface
 *
 * output(0) - carrier sense traffic
 * output(1) - mac traffic
 * output(2) - handshaking traffic
 * Ketan Mandke
 */

#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/error.hh>

#include <click/hydra_anno.hh>
#include "fromphy.hh"

CLICK_DECLS

FromPhy::FromPhy() : _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

FromPhy::~FromPhy()
{
}


int
FromPhy::configure(Vector<String> &conf, ErrorHandler *errh)
{

  int res =  cp_va_parse(conf, this, errh,
		     cpOptional,
		     cpKeywords,
		     "VERBOSE", cpUnsigned, "noisy?", &_verbose,
                     cpEnd);

  if(res < 0)
    return -1;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

void
FromPhy::push(int, Packet *p) 
{ 
  hydra_chatter(8, _verbose, "%{element}::push: ", this);

  if (!p || !p->length()) 
  {
    hydra_chatter(1, _verbose, "%{element} :push invalid packet", this);
    if (p) p->kill(); 

    return;  
  }
 
  if( is_control(p) )
  {
    if(ninputs() > 1)
      output(1).push(p);
    else
      p->kill();

    return;
  }

  output(0).push(p); 

}

bool
FromPhy::is_control(Packet*)
{
  return false;
}

enum {H_VERBOSE};

String
FromPhy::read_param(Element *e, void *vparam)
{
  FromPhy *fp = (FromPhy *)e;
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
      return String(fp->_verbose);
    default:
      return String();
  }

  return String();
}

int
FromPhy::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  FromPhy *fp = (FromPhy *) e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      fp->_verbose = verbose;
      break; 
    }
  }

  return 0;
}

void
FromPhy::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FromPhy)
