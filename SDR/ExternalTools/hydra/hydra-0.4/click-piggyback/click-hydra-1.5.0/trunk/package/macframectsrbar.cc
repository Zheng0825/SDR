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
#include <click/hydra_anno.hh>

#include "macframectsrbar.hh"
CLICK_DECLS

MACFrameCTSRbar::MACFrameCTSRbar() : _cts_hdr_size(sizeof(struct cts_rbar))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrameCTSRbar::~MACFrameCTSRbar()
{
}

void*
MACFrameCTSRbar::cast(const char *n)
{
  if( strcmp(n, "MACFrameCTSRbar") == 0 )
    return (MACFrameCTSRbar *) this;

  return MACFrame80211::cast(n);
}

int
MACFrameCTSRbar::configure(Vector<String> &conf, ErrorHandler *errh)
{
  //hydra_chatter(9, 9, "%{element}::configure start", this);
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
MACFrameCTSRbar::simple_action(Packet *p)
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
MACFrameCTSRbar::prepare_packet(Packet* p)
{
  // Got RTS?
  struct fc_80211* fc_in = (struct fc_80211 *)p->data();
  if( fc_in->type != FC0_TYPE_CTL || fc_in->subtype != FC0_SUBTYPE_RTS)
    return 0;

  // Is RTS for me?
  struct rts_rbar* rts_in = (struct rts_rbar *)p->data();
  EtherAddress rts_dst((const unsigned char *)rts_in->i_ra);
  if( _mac_addr != rts_dst )
    return 0;

  struct cts_rbar cts_out;

  // setting up the cts packet
  memcpy(&cts_out, &_cts_hdr, _cts_hdr_size);

  cts_out.i_fc.to_from_ds = rts_in->i_fc.to_from_ds;
  // The RA of CTS frame is copied from the TA field of RTS.
  memcpy(cts_out.i_ra, rts_in->i_ta, ETHER_ADDR_LEN);

  // It is just tentative, setduration will take care of it.
  cts_out.i_rate = rts_in->i_rate;  
  cts_out.i_len = rts_in->i_len; 

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)&cts_out, _cts_hdr_size, HYDRA_TAILROOM);

  struct click_hydra_anno* cha_in = (struct click_hydra_anno *) p->all_user_anno();
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type, &cts_out.i_fc, 2);
  memcpy(cha->dst, cts_out.i_ra, 6);
  memcpy(cha->src, _mac_addr.data(), 6);
  cha->type = HYDRA_TX_DATA;

  cha->rate_data_frame = cha_in->rate_data_frame; 
  cha->len_data_frame = cha_in->len_data_frame;

  return p_out;
}

WritablePacket*
MACFrameCTSRbar::prepare_packet(Packet* p, EtherAddress& dst)
{
  assert(!p);

  struct cts_rbar cts_out;
  // setting up the rts packet
  memcpy(&cts_out, &_cts_hdr, _cts_hdr_size);

  memcpy(cts_out.i_ra, dst.data(), ETHER_ADDR_LEN);

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)&cts_out, _cts_hdr_size, HYDRA_TAILROOM);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy( &cha->mac_type, &cts_out.i_fc, 2);
  memcpy(cha->dst, dst.data(), 6);
  memcpy(cha->src, _mac_addr.data(), 6);
  return p_out;
}


const unsigned char*
MACFrameCTSRbar::dst(Packet* p)
{
  assert(p);

  struct cts_rbar* cts_in = (struct cts_rbar *) p->data();
  if(cts_in->i_fc.type == FC0_TYPE_CTL &&
     cts_in->i_fc.subtype == FC0_SUBTYPE_CTS)
  {
    return cts_in->i_ra;
  }

  return 0;
}

const unsigned char*
MACFrameCTSRbar::src(Packet*)
{
  return 0;
}

// set default header
WritablePacket* 
MACFrameCTSRbar::set_header(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  p_out->push(_cts_hdr_size);
  memcpy(p_out->data(), &_cts_hdr, _cts_hdr_size);
  
  return p_out;
}

int 
MACFrameCTSRbar::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _cts_hdr_size);
  return _cts_hdr_size;
}

int 
MACFrameCTSRbar::get_feedback(Packet* p, void* feedback)
{
  if(!p || !feedback)
    return 0;

  struct cts_rbar* cts_in = (struct cts_rbar *) p->data();

  uint8_t rate = cts_in->i_rate; 
  hydra_chatter(8, _verbose, "%{element}::get_feedback recved rate = %d\n", this, rate);
  *(uint8_t *)feedback =  rate;

  return sizeof(rate);
}

String
MACFrameCTSRbar::read_param(Element *e, void *vparam)
{
  return MACFrame80211::read_param(e, vparam);
}

int
MACFrameCTSRbar::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  return MACFrame80211::write_param(in_s, e, vparam, errh);
}

void
MACFrameCTSRbar::add_handlers()
{
  MACFrame::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrameCTSRbar)
