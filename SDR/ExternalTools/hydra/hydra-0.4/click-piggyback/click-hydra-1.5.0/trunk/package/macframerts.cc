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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframerts.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/hydra_anno.hh>

#include "macframerts.hh"
CLICK_DECLS

MACFrameRTS::MACFrameRTS() : _rts_hdr_size(sizeof(struct rts_80211))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrameRTS::~MACFrameRTS()
{
}


void*
MACFrameRTS::cast(const char *n)
{
  if( strcmp(n, "MACFrameRTS") == 0 )
    return (MACFrameRTS *) this;

  return MACFrame80211::cast(n);
}

int
MACFrameRTS::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
			cpEtherAddress, "local MAC address", &_mac_addr,
			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);

  if (res < 0)
    return res;

  if(!_mac_addr || IS_BROADCAST(_mac_addr.data()) )
    return errh->error(" mac address must be set to unicast address");

  // setting the basic RTS information for future use 
  memset((void *)&_rts_hdr, 0, _rts_hdr_size);
  _rts_hdr.i_fc.protocol_version = FC0_VERSION_0;
  _rts_hdr.i_fc.type = FC0_TYPE_CTL;
  _rts_hdr.i_fc.subtype = FC0_SUBTYPE_RTS;

  //TA is the address of the STA transmitting RTS frame
  memcpy(_rts_hdr.i_ta, _mac_addr.data(), ETHER_ADDR_LEN);;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assume it receives complete data packet and generate rts frame
Packet *
MACFrameRTS::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!p || !p->length())
  {
    if(p) p->kill();
    return 0;
  }

  struct common_80211* cmm = (struct common_80211*)p->data();
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();
  if( IS_BROADCAST(cmm->i_ra) ||  !(cha->flags & HYDRA_DO_RTS) ) 
    return p;
  
  WritablePacket* p_out = prepare_packet(p);
  
  p->kill();

  return p_out;
}

// expect DATA packet with full header
WritablePacket*
MACFrameRTS::prepare_packet(Packet* p)
{
  // Got DATA?
  struct fc_80211* fc_in = (struct fc_80211 *)p->data();
  if( !is_data(*fc_in))
    return 0;

  // wonsoo +7
  //RTS uses unicast dst address in aggregated data (unicast+broadcast)
  struct click_hydra_anno* anno= 
	        (struct click_hydra_anno *) p->all_user_anno();
  uint32_t broad_len=0;
  if(anno->agg_anno)
    broad_len = anno->agg_anno->len_broadcast;
    
  struct data_80211* data_hdr_in = (struct data_80211 *)(p->data()+broad_len);
  struct rts_80211 rts_out;
 
  // setting up the rts packet
  memcpy(&rts_out, &_rts_hdr, _rts_hdr_size);

  rts_out.i_fc.to_from_ds = data_hdr_in->i_fc.to_from_ds;
  switch(data_hdr_in->i_fc.to_from_ds)
  {
    case FC1_DIR_NODS:
        memcpy(rts_out.i_ra, data_hdr_in->i_addr1, ETHER_ADDR_LEN);
        break;
    // FIXME: it only consiers the ad hoc mode. (from_ds & to_ds == 0)
    //        Do we need RTS for PCF mode?
    //        I think addr1 always contains the address of the next hop receiver.
    case FC1_DIR_TODS:
    case FC1_DIR_FROMDS:
    case FC1_DIR_DSTODS:
        memcpy(rts_out.i_ra, data_hdr_in->i_addr1, ETHER_ADDR_LEN);
    default:
    	break;

  }

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)&rts_out, _rts_hdr_size, HYDRA_TAILROOM);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type, &rts_out.i_fc, 2);
  hydra_chatter(8, _verbose, "%{element}::prepare_packet: 0x%x", this, cha->mac_type);
  memcpy(cha->dst, rts_out.i_ra, 6);
  memcpy(cha->src, _mac_addr.data(), 6);
  cha->type = HYDRA_TX_DATA;
  cha->agg_anno = 0;

  return p_out;
}

WritablePacket*
MACFrameRTS::prepare_packet(Packet* p, EtherAddress& dst)
{
  assert(!p);

  struct rts_80211 rts_out;
  // setting up the rts packet
  memcpy(&rts_out, &_rts_hdr, _rts_hdr_size);

  memcpy(rts_out.i_ra, dst.data(), ETHER_ADDR_LEN);

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)&rts_out, _rts_hdr_size, HYDRA_TAILROOM);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type,  &rts_out.i_fc, 2);
  memcpy(cha->dst, dst.data(), 6);
  memcpy(cha->src, _mac_addr.data(), 6);
  return p_out;
}


const unsigned char*
MACFrameRTS::dst(Packet* p)
{
  assert(p);

  struct rts_80211* rts_in = (struct rts_80211 *) p->data();
  if(rts_in->i_fc.type == FC0_TYPE_CTL &&
     rts_in->i_fc.subtype == FC0_SUBTYPE_RTS)
  {
    return rts_in->i_ra;
  }

  return 0;
}

const unsigned char*
MACFrameRTS::src(Packet* p)
{
  struct rts_80211* rts_in = (struct rts_80211 *) p->data();
  if(rts_in->i_fc.type == FC0_TYPE_CTL &&
     rts_in->i_fc.subtype == FC0_SUBTYPE_RTS)
  {
    return rts_in->i_ta;
  }

  return 0;
}

// set default header
WritablePacket* 
MACFrameRTS::set_header(Packet* p)
{

  WritablePacket* p_out = p->uniqueify();

  p_out->push(_rts_hdr_size);
  memcpy(p_out->data(), &_rts_hdr, _rts_hdr_size);

  return p_out;
}

int 
MACFrameRTS::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _rts_hdr_size);
  return _rts_hdr_size;
}

String
MACFrameRTS::read_param(Element *e, void *vparam)
{
  return MACFrame80211::read_param( e, vparam);
}

int
MACFrameRTS::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  return MACFrame80211::write_param(in_s, e, vparam, errh);
}

void
MACFrameRTS::add_handlers()
{
  MACFrame80211::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrameRTS)
//ELEMENT_MT_SAFE(MACFrame)
