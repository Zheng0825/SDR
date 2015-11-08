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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/fromlabviewphy1.cc,v $ 
 *  
 ***********************************************************/

/*
 * mpirx.{cc,hh} --  Classifies packets from PHY
 *             across ethernet interface
 *
 * Ketan Mandke
 */

#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/error.hh>

#include <clicknet/mpi.h>
#include <click/hydra_anno.hh>
#include "fromlabviewphy1.hh"

CLICK_DECLS

FromLabviewPhy1::FromLabviewPhy1()
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

FromLabviewPhy1::~FromLabviewPhy1()
{
}


int
FromLabviewPhy1::configure(Vector<String> &conf, ErrorHandler *errh)
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
FromLabviewPhy1::push(int, Packet *p) 
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
   
  switch(type)
  {
    case MPITYPE_RX_DATA:
    {
      hydra_chatter(8, _verbose, "%{element}::push: recv macframe", this);
      push_control_packet(1, MPITYPE_RX_ACK);
      output(0).push(p); 
      break;
    }
    case MPITYPE_CCA_BUSY:
    {
      hydra_chatter(8, _verbose, "%{element}::push: recv cca_busy", this);
      output(0).push(p); 
      break;
    }
    case MPITYPE_CCA_IDLE:
    {
      hydra_chatter(8, _verbose, "%{element}::push: recv cca_idle", this);
      output(0).push(p); 
      break;
    }
    case MPITYPE_TX_ACK:
    {
      output(1).push(p); 
      break;
    }
    case MPITYPE_TX_DATA:
    //case MPITYPE_TX_NODATA:
    //case MPITYPE_TX_REQ:
    default:
    {
       hydra_chatter(0, _verbose, "%{element}::push: never this case", this); 
       p->kill();
    }    
  }

}

void
FromLabviewPhy1::push_control_packet(int port, uint8_t type)
{
  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, 
                                       (unsigned char *)&type, 
                                       sizeof(type), 
                                       HYDRA_TAILROOM);
   
  output(port).push(p_out);
}

enum {H_VERBOSE};

String
FromLabviewPhy1::read_param(Element *e, void *vparam)
{
  FromLabviewPhy1 *fp = (FromLabviewPhy1 *)e;
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
FromLabviewPhy1::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  FromLabviewPhy1 *fp = (FromLabviewPhy1 *) e;
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
FromLabviewPhy1::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(FromLabviewPhy1)
