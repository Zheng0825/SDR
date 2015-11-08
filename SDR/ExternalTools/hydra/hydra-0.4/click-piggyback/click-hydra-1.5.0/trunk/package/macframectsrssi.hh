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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframectsrssi.hh,v $ 
 *  
 ***********************************************************/


#ifndef CLICK_MACFRAMECTSRSSI_HH
#define CLICK_MACFRAMECTSRSSI_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <clicknet/80211.h>

#include "macframects.hh"

CLICK_DECLS

class MACFrameCTSRSSI : public MACFrameCTS {
public:
  MACFrameCTSRSSI();
  ~MACFrameCTSRSSI();

  const char *class_name() const		{ return "MACFrameCTSRSSI"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

  void add_handlers();

  WritablePacket* prepare_packet(Packet* p);
  WritablePacket* prepare_packet(Packet* p, EtherAddress& dst, double snr);

  WritablePacket* set_header(Packet* p);
  int get_header(Packet* p, void* hdr);
  int header_size() {return sizeof(struct cts_80211_rssi);}
  int get_feedback(Packet* p, void* feedback);

protected:

  static String read_param(Element *e, void *vparam);
  int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

  struct cts_80211_rssi _cts_rssi_hdr;
  size_t _cts_rssi_hdr_size;
  size_t _rssi_len;
private:
};

CLICK_ENDDECLS
#endif
