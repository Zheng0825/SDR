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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframedata.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/hydra_anno.hh>

#include "macframedata.hh"
CLICK_DECLS

MACFrameData::MACFrameData() 
  : _mode(FC1_DIR_NODS), _data_hdr_size(sizeof(struct data_80211))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrameData::~MACFrameData()
{
}


void*
MACFrameData::cast(const char *n)
{
  if( strcmp(n, "MACFrameData") == 0 )
    return (MACFrameData *) this;

  return MACFrame80211::cast(n);
}

int
MACFrameData::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
			cpEtherAddress, "local MAC address", &_mac_addr,
			cpOptional,
			cpKeywords,
 			"MODE", cpUnsigned, "mode", &_mode,
			"BSSID", cpEtherAddress, "local MAC address", &_bssid,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);

  if (res < 0)
    return res;

  if(!_mac_addr || IS_BROADCAST(_mac_addr.data()) )
    return errh->error("mac address must be set to unicast address");
  if(_mode && _mode > 3)
    return errh->error("bssid must be defined < 4");
  if(_mode && !_bssid)
    return errh->error("bssid must be defined when mode is not ad hoc");

  // setting the basic Data information for future use 
  memset((void *)&_data_hdr, 0, _data_hdr_size);
  _data_hdr.i_fc.protocol_version = FC0_VERSION_0;
  _data_hdr.i_fc.type = FC0_TYPE_DATA;
  _data_hdr.i_fc.to_from_ds = _mode;
  switch (_mode) 
  {
    case FC1_DIR_NODS:
      memcpy(_data_hdr.i_addr2, _mac_addr.data(), ETHER_ADDR_LEN);
      memcpy(_data_hdr.i_addr3, _bssid.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_TODS:
      memcpy(_data_hdr.i_addr1, _bssid.data(), ETHER_ADDR_LEN);
      memcpy(_data_hdr.i_addr2, _mac_addr.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_FROMDS:
      memcpy(_data_hdr.i_addr2, _bssid.data(), ETHER_ADDR_LEN);
      memcpy(_data_hdr.i_addr3, _mac_addr.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_DSTODS:
      /* XXX this is wrong */
      memcpy(_data_hdr.i_addr2, _mac_addr.data(), ETHER_ADDR_LEN);
      memcpy(_data_hdr.i_addr3, _bssid.data(), ETHER_ADDR_LEN);
      break;
    default:
     hydra_chatter(0, _verbose, "%{element}::configure: never this case");
     break;
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assume it receives complete data packet and generate cts frame
// just filtering the data packet
Packet *
MACFrameData::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!p || !p->length())
  {
    if(p) p->kill();
    return 0;
  }

  return p;
}

WritablePacket*
MACFrameData::prepare_packet(Packet* )
{
  return 0;
}

WritablePacket*
MACFrameData::prepare_packet(Packet* p, EtherAddress& dst)
{
  assert(p);

  struct data_80211 data_hdr_out;
  // setting up the rts packet
  memcpy(&data_hdr_out, &_data_hdr, _data_hdr_size);

  switch (_mode) 
  {
    case FC1_DIR_NODS:
      memcpy(data_hdr_out.i_addr1, dst.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_TODS:
      memcpy(data_hdr_out.i_addr3, dst.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_FROMDS:
      memcpy(data_hdr_out.i_addr1, dst.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_DSTODS:
      memcpy(data_hdr_out.i_addr1, dst.data(), ETHER_ADDR_LEN);
      break;
  }

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, p->data(), p->length(), HYDRA_TAILROOM);
  p_out->push(_data_hdr_size);
  memcpy((void *)p_out->data(), &data_hdr_out, _data_hdr_size);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type, &data_hdr_out.i_fc, 2);
  memcpy(cha->dst, dst.data(), 6);
  memcpy(cha->src, _mac_addr.data(), 6);

  cha->type = HYDRA_TX_DATA;

  agg_anno_allocator(p_out, 0);

  hydra_chatter(8, _verbose, "%{element}::prepare_packet: src = %s dst = %s", this, _mac_addr.s().c_str(), dst.s().c_str());
  return p_out;
}


const unsigned char*
MACFrameData::dst(Packet* p)
{
  assert(p);

  struct data_80211* data_in = (struct data_80211 *) p->data();
  if(data_in->i_fc.type == FC0_TYPE_DATA)
  {
    switch (_mode) 
    {
      case FC1_DIR_NODS:
	return data_in->i_addr1;
      case FC1_DIR_TODS:
	return data_in->i_addr3;
      case FC1_DIR_FROMDS:
	return data_in->i_addr1;
      case FC1_DIR_DSTODS:
	return data_in->i_addr1;
    }
  }

  return 0;
}

const unsigned char*
MACFrameData::src(Packet* p)
{
  struct data_80211* data_in = (struct data_80211 *) p->data();
  if(data_in->i_fc.type == FC0_TYPE_DATA)
  {
    switch (_mode) 
    {
      case FC1_DIR_NODS:
	return data_in->i_addr2;
      case FC1_DIR_TODS:
	return data_in->i_addr2;
      case FC1_DIR_FROMDS:
	return data_in->i_addr3;
      case FC1_DIR_DSTODS:
	return data_in->i_addr2;
    }
  }

  return 0;
}

// set default header
WritablePacket* 
MACFrameData::set_header(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  p_out->push(_data_hdr_size);
  memcpy(p_out->data(), &_data_hdr, _data_hdr_size);

  return p_out;
}

int 
MACFrameData::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _data_hdr_size);
  return _data_hdr_size;
}

String
MACFrameData::read_param(Element *e, void *vparam)
{
  return MACFrame80211::read_param( e, vparam);
}

int
MACFrameData::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  return MACFrame80211::write_param(in_s, e, vparam, errh);
}

void
MACFrameData::add_handlers()
{
  MACFrame80211::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrameData)
