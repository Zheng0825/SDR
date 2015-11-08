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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/hydraencap.cc,v $ 
 *  
 ***********************************************************/
#include <click/config.h>
#include <click/etheraddress.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <clicknet/llc.h>
//#include <click/packet_anno.hh>
#include <click/hydra_anno.hh>

#include "macframe.hh"
#include "hydraencap.hh"

CLICK_DECLS

HydraEncap::HydraEncap() 
  : _mac_frame(0), _verbose(0),
    _ethtype(0), _predefined(false)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

HydraEncap::~HydraEncap()
{
}

int 
HydraEncap::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* e = 0;
  if (cp_va_parse(conf, this, errh,
		  cpElement, "generate data header for mac protocol", &e,
		  cpOptional,
		  cpKeywords,
		  "VERBOSE", cpUnsigned, "noisy?", &_verbose,

		  "DSTADDR", cpEthernetAddress, "Destination mac address", &_daddr,
		  "ETHTYPE", cpUnsigned, "Ethernet encapsulation type", &_ethtype,
		  cpEnd) < 0)
    return -1;

  if(e) {
    _mac_frame = (MACFrame *) e->cast("MACFrame");
    if(!_mac_frame) return errh->error("mac frame generator must be set using MACFrame");
  } else {
    return errh->error("mac frame generator must be defined");
  }

  // use predifined static addr only if both destnation addr and eth type are set.
  _predefined = (bool)_daddr && _ethtype;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assumes data packet with ethernet header = click_ether
Packet* 
HydraEncap::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  EtherAddress src;
  EtherAddress dst;
  uint16_t ethtype;

  if( !_predefined && (p->length() < sizeof(struct click_ether)) ) 
  {
    hydra_chatter(1, _verbose, "%{element}: packet too small: %d vs %d\n", this, p->length(), sizeof(struct click_ether));

    p->kill();
    return 0;
  }

  if(!_predefined)
  {
    click_ether *eh = (click_ether *) p->data();
    src = EtherAddress(eh->ether_shost);
    dst = EtherAddress(eh->ether_dhost);
    ethtype = eh->ether_type;
    p->pull(sizeof(struct click_ether));
  }	
  else
  {
    dst = _daddr;
    ethtype = _ethtype;
  }
  hydra_chatter(8, _verbose, "%{element}, src = %s dst = %s", this, src.s().c_str(), dst.s().c_str()); 	

  Packet* p_out = prepare_llc(p, ethtype);
  if(!p_out)
  {
    p->kill();
    return 0;
  }

  p_out = _mac_frame->prepare_packet(p_out, dst);

  return p_out;

}

Packet*
HydraEncap::prepare_llc(Packet* p, uint16_t ethtype)
{
  hydra_chatter(9, _verbose, "%{element}::prepare_llc: llc header size = %d", this, sizeof(struct click_llc));

  p = p->push(sizeof(struct click_llc));
  if (!p) 
    return 0;

  // make ll header for ethtype
  // FIXME: make LLHeader and use that element for following routine
  struct click_llc *llc = (struct click_llc *) p->data();
  llc->llc_dsap = llc->llc_ssap = LLC_SNAP_LSAP;
  llc->llc_control = LLC_UI;
  llc->llc_un.type_snap.org_code[0] = 0;
  llc->llc_un.type_snap.org_code[1] = 0;
  llc->llc_un.type_snap.org_code[2] = 0;
  llc->llc_un.type_snap.ether_type = ethtype;

  return p;
}

enum {H_VERBOSE, H_ETHTYPE, H_DST};

String 
HydraEncap::read_param(Element *e, void *thunk)
{
  HydraEncap *td = (HydraEncap *)e;
    switch ((uintptr_t) thunk) {
      case H_VERBOSE:
	return String(td->_verbose) + "\n";
      case H_ETHTYPE:
	return String(td->_ethtype) + "\n";
      case H_DST:
	return td->_daddr.s() + "\n";
    default:
      return String();
    }
}
int 
HydraEncap::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  HydraEncap *f = (HydraEncap *)e;
  String s = cp_uncomment(in_s);
  switch((int)vparam) {
  case H_VERBOSE: {    //debug
    unsigned verbose;
    if (!cp_unsigned(s, &verbose)) 
      return errh->error("debug parameter must be boolean");
    f->_verbose = verbose;
    break;
  }

  case H_ETHTYPE: {    //mode
    int m;
    if (!cp_integer(s, &m)) 
      return errh->error("mode parameter must be int");
    f->_ethtype = m;
    break;
  }
  case H_DST: {    //debug
    EtherAddress e;
    if (!cp_ethernet_address(s, &e)) 
      return errh->error("bssid parameter must be ethernet address");
    f->_daddr = e;
    break;
  }
  }
  return 0;
}
 
void
HydraEncap::add_handlers()
{
  add_read_handler("debug", read_param, (void *) H_VERBOSE);
  add_read_handler("ethtype", read_param, (void *) H_ETHTYPE);
  add_read_handler("daddr", read_param, (void *) H_DST);

  add_write_handler("debug", write_param, (void *) H_VERBOSE);
  add_write_handler("ethtype", write_param, (void *) H_ETHTYPE);
  add_write_handler("daddr", write_param, (void *) H_DST);
}
CLICK_ENDDECLS
EXPORT_ELEMENT(HydraEncap)
