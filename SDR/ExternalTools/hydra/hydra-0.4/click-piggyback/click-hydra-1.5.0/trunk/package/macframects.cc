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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframects.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/hydra_anno.hh>

#include "macframects.hh"
CLICK_DECLS

MACFrameCTS::MACFrameCTS() : _cts_hdr_size(sizeof(struct cts_80211))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrameCTS::~MACFrameCTS()
{
}

void*
MACFrameCTS::cast(const char *n)
{
  if( strcmp(n, "MACFrameCTS") == 0 )
    return (MACFrameCTS *) this;

  return MACFrame80211::cast(n);
}

int
MACFrameCTS::configure(Vector<String> &conf, ErrorHandler *errh)
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

  // setting the basic CTS information for future use 
  memset((void *)&_cts_hdr, 0, _cts_hdr_size);
  _cts_hdr.i_fc.protocol_version = FC0_VERSION_0;
  _cts_hdr.i_fc.type = FC0_TYPE_CTL;
  _cts_hdr.i_fc.subtype = FC0_SUBTYPE_CTS;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assume it receives complete data packet and generate cts frame
Packet *
MACFrameCTS::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!p || !p->length())
  {
    if(p) p->kill();
    return 0;
  }

  WritablePacket* p_out = prepare_packet(p);
  
  p->kill();

  return p_out;
}

// expect DATA packet with full header
WritablePacket*
MACFrameCTS::prepare_packet(Packet* p)
{
  // Got RTS?
  struct fc_80211* fc_in = (struct fc_80211 *)p->data();
  if( fc_in->type != FC0_TYPE_CTL || fc_in->subtype != FC0_SUBTYPE_RTS)
    return 0;

  // Is RTS for me?
  struct rts_80211* rts_in = (struct rts_80211 *)p->data();
  EtherAddress rts_dst((const unsigned char *)rts_in->i_ra);
  if( _mac_addr != rts_dst )
    return 0;

  struct cts_80211 cts_out;

  // setting up the cts packet
  memcpy(&cts_out, &_cts_hdr, _cts_hdr_size);

  cts_out.i_fc.to_from_ds = rts_in->i_fc.to_from_ds;
  // The RA of CTS frame is copied from the TA field of RTS.
  memcpy(cts_out.i_ra, rts_in->i_ta, ETHER_ADDR_LEN);

  // just copy nav, then SetDuration takes care of it.
#ifndef IEEE80211_LONG_TIMER
  memcpy(cts_out.i_dur, rts_in->i_dur, 2);
#else
  memcpy(cts_out.i_dur, rts_in->i_dur, 4);
#endif

  struct click_hydra_anno *rts_cha =
                            (struct click_hydra_anno *)p->all_user_anno();

  // Create the outgoing packet.  We use the metadata obtained from the RTS
  // annotation field as the data.
  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM,
                      (unsigned char *)rts_cha->agg_anno->metadata,
                      rts_cha->agg_anno->len_metadata, HYDRA_TAILROOM);

  // Add the CTS header
  p_out->push(_cts_hdr_size);
  memcpy((void *)p_out->data(), &cts_out, _cts_hdr_size);

  // Set up the annotation field for the outgoing CTS.
  struct click_hydra_anno* cha = 
                        (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type, &cts_out.i_fc, 2);
  memcpy(cha->dst, cts_out.i_ra, 6);
  memcpy(cha->src, _mac_addr.data(), 6);
  cha->type = HYDRA_TX_DATA;

  cha->agg_anno = 0;

  return p_out;
}

WritablePacket*
MACFrameCTS::prepare_packet(Packet* p, EtherAddress& dst)
{
  assert(!p);

  struct cts_80211 cts_out;
  // setting up the rts packet
  memcpy(&cts_out, &_cts_hdr, _cts_hdr_size);

  memcpy(cts_out.i_ra, dst.data(), ETHER_ADDR_LEN);

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM,
                    (unsigned char *)&cts_out, _cts_hdr_size, HYDRA_TAILROOM);

  struct click_hydra_anno* cha =
                    (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy( &cha->mac_type, &cts_out.i_fc, 2);
  memcpy(cha->dst, dst.data(), 6);
  memcpy(cha->src, _mac_addr.data(), 6);
  return p_out;
}


const unsigned char*
MACFrameCTS::dst(Packet* p)
{
  assert(p);

  struct cts_80211* cts_in = (struct cts_80211 *) p->data();
  if(cts_in->i_fc.type == FC0_TYPE_CTL &&
     cts_in->i_fc.subtype == FC0_SUBTYPE_CTS)
  {
    return cts_in->i_ra;
  }

  return 0;
}

const unsigned char*
MACFrameCTS::src(Packet*)
{
  return 0;
}

// set default header
WritablePacket* 
MACFrameCTS::set_header(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  p_out->push(_cts_hdr_size);
  memcpy(p_out->data(), &_cts_hdr, _cts_hdr_size);
  
  return p_out;
}

int 
MACFrameCTS::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _cts_hdr_size);
  return _cts_hdr_size;
}

String
MACFrameCTS::read_param(Element *e, void *vparam)
{
  return MACFrame80211::read_param(e, vparam);
}

int
MACFrameCTS::write_param(const String &in_s, Element *e, void *vparam,
                         ErrorHandler *errh)
{
  return MACFrame80211::write_param(in_s, e, vparam, errh);
}

void
MACFrameCTS::add_handlers()
{
  MACFrame::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrameCTS)
