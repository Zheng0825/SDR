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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframeack.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/hydra_anno.hh>

#include "macframeack.hh"
CLICK_DECLS

MACFrameACK::MACFrameACK() : _ack_hdr_size(sizeof(struct ack_80211))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrameACK::~MACFrameACK()
{
}

void*
MACFrameACK::cast(const char *n)
{
  if( strcmp(n, "MACFrameACK") == 0 )
    return (MACFrameACK *) this;

  return MACFrame80211::cast(n);
}

int
MACFrameACK::configure(Vector<String> &conf, ErrorHandler *errh)
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
  memset((void *)&_ack_hdr, 0, _ack_hdr_size);
  _ack_hdr.i_fc.protocol_version = FC0_VERSION_0;
  _ack_hdr.i_fc.type = FC0_TYPE_CTL;
  _ack_hdr.i_fc.subtype = FC0_SUBTYPE_ACK;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assume it receives complete data packet and generate ack frame
Packet *
MACFrameACK::simple_action(Packet *p)
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
MACFrameACK::prepare_packet(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::prepare_packet", this);
  // Got DATA?
  struct fc_80211* fc_in = (struct fc_80211 *)p->data();
  if( fc_in->subtype != FC0_SUBTYPE_DATA)
  {
    hydra_chatter(8, _verbose, "%{element}::prepare_packet: drop due to type", this);
    return 0;
  }

  // Is DATA for me?
  // FIXME: addr1 of data is immidiate recipient?
  struct data_80211* data_hdr_in = (struct data_80211 *)p->data();
  EtherAddress data_dst((const unsigned char *)data_hdr_in->i_addr1);
  if( _mac_addr != data_dst )
  {
    hydra_chatter(8, _verbose, "%{element}::prepare_packet: drop due to addr %s, %s", this, _mac_addr.s().c_str(), data_dst.s().c_str());
    return 0;
  }
  struct ack_80211 ack_out;

  // setting up the ack packet
  memcpy(&ack_out, &_ack_hdr, _ack_hdr_size);

  ack_out.i_fc.to_from_ds = data_hdr_in->i_fc.to_from_ds;
  // The RA of ACK frame is copied from the address2 field of DATA.
  memcpy(ack_out.i_ra, data_hdr_in->i_addr2, ETHER_ADDR_LEN);
  // just copy nav, then SetDuration takes care of it.
#ifndef IEEE80211_LONG_TIMER
  memcpy(ack_out.i_dur, data_hdr_in->i_dur, 2);
#else
  memcpy(ack_out.i_dur, data_hdr_in->i_dur, 4);
#endif

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)&ack_out, _ack_hdr_size, HYDRA_TAILROOM);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type, &ack_out.i_fc, 2);
  memcpy(cha->dst, ack_out.i_ra, 6);
  memcpy(cha->src, _mac_addr.data(), 6);

  cha->type = HYDRA_TX_DATA;

  cha->agg_anno = 0;

  return p_out;
}

WritablePacket*
MACFrameACK::prepare_packet(Packet* p, EtherAddress& dst)
{
  assert(!p);

  struct ack_80211 ack_out;
  // setting up the rts packet
  memcpy(&ack_out, &_ack_hdr, _ack_hdr_size);

  memcpy(ack_out.i_ra, dst.data(), ETHER_ADDR_LEN);

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)&ack_out, _ack_hdr_size, HYDRA_TAILROOM);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type, &ack_out.i_fc, 2);
  memcpy(cha->dst, dst.data(), 6);
  memcpy(cha->src, _mac_addr.data(), 6);
  return p_out;
}


const unsigned char*
MACFrameACK::dst(Packet* p)
{
  assert(p);

  struct ack_80211* ack_in = (struct ack_80211 *) p->data();
  if(ack_in->i_fc.type == FC0_TYPE_CTL &&
     ack_in->i_fc.subtype == FC0_SUBTYPE_ACK)
  {
    return ack_in->i_ra;
  }

  return 0;
}

const unsigned char*
MACFrameACK::src(Packet* )
{
  return 0;
}

// set default header
WritablePacket* 
MACFrameACK::set_header(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  p_out->push(_ack_hdr_size);
  memcpy(p_out->data(), &_ack_hdr, _ack_hdr_size);

  return p_out;
}

int 
MACFrameACK::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _ack_hdr_size);
  return _ack_hdr_size;
}

String
MACFrameACK::read_param(Element *e, void *vparam)
{
  return MACFrame80211::read_param(e, vparam);
}

int
MACFrameACK::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  return MACFrame80211::write_param(in_s, e, vparam, errh);
}

void
MACFrameACK::add_handlers()
{
  MACFrame80211::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrameACK)
