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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/ratecontrolconst.cc,v $
 *  
 ***********************************************************/
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
//#include <click/packet_anno.hh>
//#include <clicknet/ether.h>
//#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "ratecontrolconst.hh"

CLICK_DECLS

RateControlConst::RateControlConst()
  : _rate(0) 
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RateControlConst::~RateControlConst()
{
}

void*
RateControlConst::cast(const char *n)
{
  if( strcmp(n, "RateControlConst") == 0 )
    return (RateControlConst *) this;
  else
    return RateControl::cast(n);
}

int
RateControlConst::configure(Vector<String> &conf, ErrorHandler *errh)
{

  if (cp_va_parse(conf, this, errh,
		  cpUnsigned, "rate[Mbps] for 802.11 a/g", &_rate, 
		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }

  if(!_rate)
    return errh->error("rate must be defined");

  if(!set_rate_struct())
    return errh->error("rate must be one of supported data rate by 802.11a/g");

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
RateControlConst::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  //struct click_hydra_anno* cha = (struct click_hydra_anno *) p_in->all_user_anno();
  // FIXME: can I use Vector in click annotation?
  //cha->power_vector.clear();
  // do feedback algoithm here
  // and forward
  return p_in;
}

WritablePacket*
RateControlConst::get_txrate(Packet* p_in)
{
  hydra_chatter(8, _verbose, "%{element}::get_txrate: ", this);

  WritablePacket* p_out = p_in->uniqueify(); 

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();

  // FIXME: can I use Vector in click annotation?
  //cha->rate_vector.push_back(_rate_struct);
  cha->rate_mbps = _rate;

  return p_out;

}

bool
RateControlConst::set_rate_struct()
{
  _rate_struct.channel_number = HYDRA_ALL_CHANNEL;
  switch(_rate)
  {
    case 6:
      _rate_struct.modulation = BPSK;
      _rate_struct.coding_rate = one_second;
      break;
    case 9:
      _rate_struct.modulation = BPSK;
      _rate_struct.coding_rate = two_third;
      break;
    case 12:
      _rate_struct.modulation = QPSK;
      _rate_struct.coding_rate = one_second;
      break;
    case 18:
      _rate_struct.modulation = QPSK;
      _rate_struct.coding_rate = three_forth;
      break;
    case 24:
      _rate_struct.modulation = QAM16;
      _rate_struct.coding_rate = one_second;
      break;
    case 36:
      _rate_struct.modulation = QAM16;
      _rate_struct.coding_rate = three_forth;
      break;
    case 48:
      _rate_struct.modulation = QAM64;
      _rate_struct.coding_rate = two_third;
      break;
    case 54:
      _rate_struct.modulation = QAM64;
      _rate_struct.coding_rate = three_forth;
      break;
    default:
       return false;
  }
  return true;
}

enum {H_VERBOSE};

String
RateControlConst::read_handler(Element *e, void *thunk)
{
  RateControlConst *src = (RateControlConst *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
RateControlConst::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  RateControlConst *src = (RateControlConst *) e;
  String s = cp_uncomment(arg);
  switch((int)vparam) {
  case H_VERBOSE: {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("verbose parameter must be unsigned");
    src->_verbose = m;
    break;
  }
  }
  return 0;
}

void
RateControlConst::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RateControlConst)

