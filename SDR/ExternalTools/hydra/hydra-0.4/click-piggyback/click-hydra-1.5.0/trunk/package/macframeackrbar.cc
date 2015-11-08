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

#include "macframeackrbar.hh"

CLICK_DECLS

MACFrameACKRbar::MACFrameACKRbar() : _ack_hdr_size(sizeof(struct ack_rbar))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrameACKRbar::~MACFrameACKRbar()
{
}

void*
MACFrameACKRbar::cast(const char *n)
{
  if( strcmp(n, "MACFrameACKRbar") == 0 )
    return (MACFrameACKRbar *) this;

  return MACFrame80211::cast(n);
}

int
MACFrameACKRbar::configure(Vector<String> &conf, ErrorHandler *errh)
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
  memset((void *)&_ack_hdr, 0, _ack_hdr_size);
  _ack_hdr.i_fc.protocol_version = FC0_VERSION_0;
  _ack_hdr.i_fc.type = FC0_TYPE_CTL;
  _ack_hdr.i_fc.subtype = FC0_SUBTYPE_ACK;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assume it receives complete data packet and generate ack frame
Packet *
MACFrameACKRbar::simple_action(Packet *p)
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
MACFrameACKRbar::prepare_packet(Packet* p)
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
  struct data_rbar* data_hdr_in = (struct data_rbar *)p->data();
  EtherAddress data_dst((const unsigned char *)data_hdr_in->i_addr1);
  if( _mac_addr != data_dst )
  {
    hydra_chatter(8, _verbose, "%{element}::prepare_packet: drop due to addr %s, %s", this, _mac_addr.s().c_str(), data_dst.s().c_str());
    return 0;
  }
  struct ack_rbar ack_out;

  // setting up the ack packet
  memcpy(&ack_out, &_ack_hdr, _ack_hdr_size);

  ack_out.i_fc.to_from_ds = data_hdr_in->i_fc.to_from_ds;
  // The RA of ACK frame is copied from the address2 field of DATA.
  memcpy(ack_out.i_ra, data_hdr_in->i_addr2, ETHER_ADDR_LEN);

  // It is just tentative, setduration will take care of it.
  ack_out.i_rate = data_hdr_in->i_rate;  
  ack_out.i_len = data_hdr_in->i_len; 

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)&ack_out, _ack_hdr_size, HYDRA_TAILROOM);

  //struct click_hydra_anno* cha_in = (struct click_hydra_anno *) p->all_user_anno();
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type, &ack_out.i_fc, 2);
  memcpy(cha->dst, ack_out.i_ra, 6);
  memcpy(cha->src, _mac_addr.data(), 6);

  cha->type = HYDRA_TX_DATA;

  // FIXME: unconvert rate, but nobody uses this
  cha->rate_data_frame = ack_out.i_rate; 
  cha->len_data_frame = ack_out.i_len;

  return p_out;
}

WritablePacket*
MACFrameACKRbar::prepare_packet(Packet* p, EtherAddress& dst)
{
  assert(!p);

  struct ack_rbar ack_out;
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
MACFrameACKRbar::dst(Packet* p)
{
  assert(p);

  struct ack_rbar* ack_in = (struct ack_rbar *) p->data();
  if(ack_in->i_fc.type == FC0_TYPE_CTL &&
     ack_in->i_fc.subtype == FC0_SUBTYPE_ACK)
  {
    return ack_in->i_ra;
  }

  return 0;
}

const unsigned char*
MACFrameACKRbar::src(Packet* )
{
  return 0;
}

// set default header
WritablePacket* 
MACFrameACKRbar::set_header(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  p_out->push(_ack_hdr_size);
  memcpy(p_out->data(), &_ack_hdr, _ack_hdr_size);

  return p_out;
}

int 
MACFrameACKRbar::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _ack_hdr_size);
  return _ack_hdr_size;
}

String
MACFrameACKRbar::read_param(Element *e, void *vparam)
{
  return MACFrame80211::read_param(e, vparam);
}

int
MACFrameACKRbar::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  return MACFrame80211::write_param(in_s, e, vparam, errh);
}

void
MACFrameACKRbar::add_handlers()
{
  MACFrame80211::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrameACKRbar)
