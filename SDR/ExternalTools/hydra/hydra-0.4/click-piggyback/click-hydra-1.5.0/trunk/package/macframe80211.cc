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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframe80211.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/hydra_anno.hh>

#include "macframe80211.hh"
CLICK_DECLS

MACFrame80211::MACFrame80211() 
  : _common_hdr_size(sizeof(struct common_80211))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrame80211::~MACFrame80211()
{
}


void*
MACFrame80211::cast(const char *n)
{
  if( strcmp(n, "MACFrame80211") == 0 )
    return (MACFrame80211 *) this;

  return MACFrame::cast(n);
}

int
MACFrame80211::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
			cpOptional,
			cpKeywords,
			"VERBOSE", cpBool, "noisy?", &_verbose,
			cpEnd);

  if (res < 0)
    return res;

  // setting the basic RTS information for future use 
  memset((void *)&_common_hdr, 0, _common_hdr_size);
  _common_hdr.i_fc.protocol_version = FC0_VERSION_0;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assume it receives complete data packet and generate common frame
Packet *
MACFrame80211::simple_action(Packet *p)
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
MACFrame80211::prepare_packet(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  return p_out;
}

WritablePacket*
MACFrame80211::prepare_packet(Packet* p, EtherAddress& dst)
{
  assert(!p);

  struct common_80211 common_out;
  // setting up the common packet
  memcpy(&common_out, &_common_hdr, _common_hdr_size);

  memcpy(common_out.i_ra, dst.data(), ETHER_ADDR_LEN);

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)&common_out, _common_hdr_size, HYDRA_TAILROOM);

  return p_out;
}


const unsigned char*
MACFrame80211::dst(Packet* p)
{
  assert(p);

  struct common_80211* common_in = (struct common_80211 *) p->data();

  return common_in->i_ra;
}

const unsigned char*
MACFrame80211::src(Packet*)
{
  return 0;
}

const unsigned char*
MACFrame80211::duration(Packet* p)
{
  assert(p);

  struct common_80211* common_in = (struct common_80211 *) p->data();

  return common_in->i_dur;

}

// set default header
WritablePacket* 
MACFrame80211::set_header(Packet* p)
{

  WritablePacket* p_out = p->uniqueify();

  p_out->push(_common_hdr_size);
  memcpy(p_out->data(), &_common_hdr, _common_hdr_size);

  return p_out;
}

int 
MACFrame80211::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _common_hdr_size);
  return _common_hdr_size;
}

String
MACFrame80211::read_param(Element *e, void *vparam)
{
  return MACFrame::read_param( e, vparam);
}

int
MACFrame80211::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  return MACFrame::write_param(in_s, e, vparam, errh);
}

void
MACFrame80211::add_handlers()
{
  MACFrame::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrame80211)
