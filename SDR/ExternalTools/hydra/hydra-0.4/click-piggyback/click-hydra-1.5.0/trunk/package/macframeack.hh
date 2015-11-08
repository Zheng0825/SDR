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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframeack.hh,v $
 *  
 ***********************************************************/


#ifndef CLICK_MACFRAMEACK_HH
#define CLICK_MACFRAMEACK_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <clicknet/80211.h>

#include "macframe80211.hh"

CLICK_DECLS


class MACFrameACK : public MACFrame80211 {
public:
  MACFrameACK();
  ~MACFrameACK();

  const char *class_name() const		{ return "MACFrameACK"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

  void add_handlers();

  WritablePacket* prepare_packet(Packet* p);
  WritablePacket* prepare_packet(Packet* p, EtherAddress& dst);
  const unsigned char* dst(Packet* p);  
  const unsigned char* src(Packet* p);  
  WritablePacket* set_header(Packet* p);
  int get_header(Packet* p, void* hdr);
  int header_size() {return _ack_hdr_size;}

protected:

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

  struct ack_80211 _ack_hdr;
  size_t _ack_hdr_size;
private:
};

CLICK_ENDDECLS
#endif
