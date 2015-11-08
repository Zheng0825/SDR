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

#include <clicknet/mpi.h>
#include "txvectorgr.hh"
CLICK_DECLS

TXVectorGr::TXVectorGr()
  : _hdr_size(sizeof(struct gr_txvector))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

TXVectorGr::~TXVectorGr()
{
}

void*
TXVectorGr::cast(const char *n)
{
    if (strcmp(n, "TXVectorGr") == 0)
	return (TXVectorGr *) this;
    
    return MPIVector::cast(n);
}

int
TXVectorGr::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
 			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}


Packet *
TXVectorGr::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);
  //struct click_hydra_anno* debug = (struct click_hydra_anno*) p->all_user_anno();
  //printf("txvector start: 0x%x\n", debug->mac_type);

  if(!p || !p->length())
  {
    hydra_chatter(0, _verbose, "%{element}::simple_action: wrong packet", this);
    if(p) p->kill();
    return 0;
  }

  //WritablePacket* p_out = prepare_packet(p);
  Packet* p_out = prepare_packet(p);
  // never use manipulate or free a packet after calling its uniqueify 
  //p->kill();

  //printf("txvector end: 0x%x\n", debug->mac_type);

  return p_out;
}

//WritablePacket*
Packet*
TXVectorGr::prepare_packet(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::prepare_packet:", this);

  //WritablePacket* p_out = p->uniqueify();
  Packet* p_out = p;
  
  p_out->push(_hdr_size);
  struct gr_txvector* txv = (struct gr_txvector *) p_out->data();

  struct click_hydra_anno* cha = 
		(struct click_hydra_anno *) p_out->all_user_anno();

  txv->type = convert_type(cha->type);
  txv->length = p_out->length() - _hdr_size;
  txv->rate = convert_rate(cha->rate_mbps); 
  //txv->txpwr = 0.0;
  txv->txpwr = (float) cha->power;

  return p_out;
}

uint8_t
TXVectorGr::convert_rate(uint16_t anno_rate)
{
  uint16_t rate = 0;

  switch(anno_rate)
  {
    case 6: rate = MPIRATE_6M; break;
    case 9: rate = MPIRATE_9M; break;
    case 12: rate = MPIRATE_12M; break;
    case 18: rate = MPIRATE_18M; break;
    case 24: rate = MPIRATE_24M; break;
    case 36: rate = MPIRATE_36M; break;
    case 48: rate = MPIRATE_48M; break;
    case 54: rate = MPIRATE_54M; break;
  }
  return rate;
}

uint8_t
TXVectorGr::convert_type(uint8_t anno_type)
{
  uint8_t txv_type = 0;
  switch(anno_type)
  {
    // FIXME: some element must change RX_DATA to TX_DATA
    case HYDRA_RX_DATA: 
    case HYDRA_TX_DATA: 
	txv_type = MPIMSG_TX_DATA; 
	break;
    default:
      hydra_chatter(0, _verbose, "%{element}::convert_type: never this case %d", this, anno_type);
      break;
  }
  return txv_type;
}

String
TXVectorGr::read_param(Element *e, void *vparam)
{
  TXVectorGr *mf = (TXVectorGr *)e;
  switch( (intptr_t)vparam )
  {
    case 0:
      return String(mf->_verbose);
    default:
      return String();
  }
  return String();
}

int
TXVectorGr::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  TXVectorGr *mf = (TXVectorGr *)e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case 0:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      mf->_verbose = verbose;
      break; 
    }
  }

  return 0;
}

void
TXVectorGr::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TXVectorGr)
