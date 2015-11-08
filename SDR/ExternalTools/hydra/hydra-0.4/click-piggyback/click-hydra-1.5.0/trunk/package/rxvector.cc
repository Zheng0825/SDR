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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/rxvector.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>

#include <clicknet/mpi.h>
#include "rxvector.hh"

CLICK_DECLS

RXVector::RXVector() :_hdr_size( sizeof(struct rxvector) )
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RXVector::~RXVector()
{
}

void*
RXVector::cast(const char *n)
{
    if (strcmp(n, "RXVector") == 0)
	return (RXVector *) this;
    
    return MPIVector::cast(n);
}

int
RXVector::configure(Vector<String> &conf, ErrorHandler *errh)
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
RXVector::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!p || !p->length())
  {
    if(p) p->kill();
    return 0;
  }
  
  //WritablePacket* p_out = prepare_packet(p);
  Packet* p_out = prepare_packet(p);

  return p_out;
}

//WritablePacket*
Packet*
RXVector::prepare_packet(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::prepare_packet:", this);
  //WritablePacket* p_out = p->uniqueify();
  Packet* p_out = p;
  
  struct rxvector* rxv = (struct rxvector *) p_out->data();

  //FIXME: I change the bit alignment based on MPI vis in LABVIEW.
  //       For baseband test just use this, 
  //       but eventually LABVIEW must be changed to above alignment.
  //       Or, just use 8bit boundary for simplicity.
/*
  uint8_t tmp[3];
  memcpy(tmp, rxv, 3);
  rxv->length = ((tmp[1] * (1 << 4) ) & 0xff0) | ((tmp[2] >> 4) & 0x0f);
  rxv->rate = tmp[2] & 0x0f;
*/
  struct click_hydra_anno* cha = 
		(struct click_hydra_anno *) p_out->all_user_anno();


  if( rxv->length != p_out->length() - _hdr_size)
  {
    hydra_chatter(8, _verbose, "%{element}::prepare_packet: rxv_len=%d, pkt_len=%d, hdr_size=%d", this, rxv->length, p_out->length(), _hdr_size);
    //assert( rxv->length == p_out->length() - _hdr_size)
    p_out->kill();
    return 0;
  }
   
  cha->type = convert_type(rxv->type); 
  cha->rate_mbps = convert_rate(rxv->rate) ; 
  //cha->rrate_mbps = convert_rate(rxv->rate) ; 
  cha->rssi = rxv->rssi;

  cha->avg_snr = ((double)rxv->avg_snr) / (-1000.0);
  hydra_chatter(8, _verbose, "%{element}::prepare_packet: rxv_snr = %d cha_snr = %f", this, rxv->avg_snr, cha->avg_snr);

  // FIXME: change belows according to the drexel implementation.
/*
  cha->channel_vector.clear();
  struct click_hydra_channel chc;
  chc.real = 0.1; 
  chc.imaginary = 0.2;  
  cha->channel_vector.push_back(chc);
  hydra_chatter(8, _verbose, "%{element}::prepare_packet: real=%f imag=%f", this, chc.real, chc.imaginary);
*/
/*
  if(rxv->info_size > MAX_CHANNEL_INFO)
  {
    p_out->pull(_hdr_size);
    return p_out;
  }
  
  for(int i=0; i<rxv->info_size; i++)
  {
    cha->channel_vector[i].real = rxv->ch_info[i].real;
    cha->channel_vector[i].imaginary = rxv->ch_info[i].imaginary;
  }
*/
  p_out->pull(_hdr_size);

  return p_out;
}

uint16_t
RXVector::convert_rate(uint8_t rxv_rate)
{
  hydra_chatter(8, _verbose, "%{element}::convert_rate:", this);
  uint16_t anno_rate = 0;

  switch(rxv_rate)
  {
    case MPIRATE_6M: anno_rate = 6; break;
    case MPIRATE_9M: anno_rate = 9; break;
    case MPIRATE_12M: anno_rate = 12; break;
    case MPIRATE_18M: anno_rate = 18; break;
    case MPIRATE_24M: anno_rate = 24; break;
    case MPIRATE_36M: anno_rate = 36; break;
    case MPIRATE_48M: anno_rate = 48; break;
    case MPIRATE_54M: anno_rate = 54; break;
  }
  return anno_rate;
}

uint8_t
RXVector::convert_type(uint8_t rxv_type)
{
  hydra_chatter(8, _verbose, "%{element}::convert_type:", this);
  uint8_t anno_type = 0;
  switch(rxv_type)
  {
    case MPITYPE_RX_DATA: 
    {
      hydra_chatter(9, _verbose, "%{element}::convert_type: RX_DATA", this);
      anno_type = HYDRA_RX_DATA; 
      break;
    }
    case MPITYPE_CCA_IDLE: anno_type = HYDRA_CCA_IDLE; break;
    case MPITYPE_CCA_BUSY: anno_type = HYDRA_CCA_BUSY; break;
    case MPITYPE_TX_REQ:
    case MPITYPE_TX_DATA:
    case MPITYPE_TX_NODATA:
    default:
      hydra_chatter(0, _verbose, "%{element}::convert_type: never this case", this);
      break;
  }

  return anno_type;
}

String
RXVector::read_param(Element *e, void *vparam)
{
  RXVector *mf = (RXVector *)e;
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
RXVector::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  RXVector *mf = (RXVector *)e;
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
RXVector::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RXVector)
