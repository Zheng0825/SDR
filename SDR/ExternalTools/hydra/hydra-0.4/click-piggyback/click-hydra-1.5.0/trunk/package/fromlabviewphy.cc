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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/fromlabviewphy.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/error.hh>

#include <clicknet/mpi.h>
#include <click/hydra_anno.hh>
#include "fromlabviewphy.hh"

CLICK_DECLS

FromLabviewPhy::FromLabviewPhy()
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

FromLabviewPhy::~FromLabviewPhy()
{
}

int
FromLabviewPhy::configure(Vector<String> &conf, ErrorHandler *errh)
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
FromLabviewPhy::push(int, Packet *p) 
{ 
  hydra_chatter(9, _verbose, "%{element}::push: ", this);

  if (!p || !p->length()) 
  {
    hydra_chatter(1, _verbose, "%{element} :push invalid packet", this);
    if (p) p->kill(); 

    return;  
  }

  struct mpitype* mt = (struct mpitype *) p->data();
  uint8_t type = mt->type;
   
  struct timeval tv;
  click_gettimeofday(&tv);

  switch(type)
  {
    case MPITYPE_RX_DATA:
    {
      hydra_chatter(8, _verbose, "%{element}::push: recv RX_START %d.%06d", this, _rx_start.tv_sec, _rx_start.tv_usec);
      hydra_chatter(8, _verbose, "%{element}::push: recv RX_DATA %d.%06d", this, tv.tv_sec, tv.tv_usec);
      output(0).push(p); 
      break;
    }
    case MPITYPE_CCA_BUSY:
    {
      hydra_chatter(8, _verbose, "%{element}::push: recv CCA_BUSY %d.%06d", this, tv.tv_sec, tv.tv_usec);
      output(0).push(p); 
      break;
    }
    case MPITYPE_CCA_IDLE:
    {
      hydra_chatter(8, _verbose, "%{element}::push: recv CCA_IDLE %d.%06d", this, tv.tv_sec, tv.tv_usec);
      output(0).push(p); 
      break;
    }
    case MPITYPE_TX_REQ:
    {
      hydra_chatter(9, _verbose, "%{element}::push: recv TX_REQ %d.%06d", this, tv.tv_sec, tv.tv_usec);
      output(1).push(p); 
      break;
    }
    case MPITYPE_TX_DONE:
    {
      hydra_chatter(8, _verbose, "%{element}::push: recv TX_DONE %d.%06d", this, tv.tv_sec, tv.tv_usec);
      break;
    }
    case MPITYPE_RX_START:
    {
      _rx_start.tv_sec = tv.tv_sec;
      _rx_start.tv_usec = tv.tv_usec;
      break;
    }
    case MPITYPE_TX_DATA:
    case MPITYPE_TX_NODATA:
    default:
    {
       hydra_chatter(0, _verbose, "%{element}::push: never this case", this); 
       p->kill();
    }    
  }

}


enum {H_VERBOSE};

String
FromLabviewPhy::read_param(Element *e, void *vparam)
{
  FromLabviewPhy *fp = (FromLabviewPhy *)e;
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
FromLabviewPhy::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  FromLabviewPhy *fp = (FromLabviewPhy *) e;
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
FromLabviewPhy::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FromLabviewPhy)
