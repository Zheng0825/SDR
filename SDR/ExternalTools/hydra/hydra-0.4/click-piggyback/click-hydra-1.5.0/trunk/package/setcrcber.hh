/* 
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
 */

#ifndef CLICK_SETCRCBER_HH
#define CLICK_SETCRCBER_HH
#include <click/element.hh>
#include <click/etheraddress.hh>
#include <clicknet/80211.h>
CLICK_DECLS

class MACFrame80211;
class SetDefer;

class SetCRCBER : public Element { public:
  
  SetCRCBER();
  ~SetCRCBER();

  const char *class_name() const	{ return "SetCRCBER"; }
  const char *port_count() const	{ return "1/1"; }
  const char *processing() const	{ return AGNOSTIC; }
  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);
  
  Packet *simple_action(Packet *pkt);
protected:
  void process_rts(Packet* pkt);
  void process_cts(Packet* pkt);
  void process_data(Packet* pkt);
  void process_ack(Packet* pkt);

protected:
  EtherAddress _my_mac_addr;

  String _filename;
  String _logfilename;
  FILE* _fptr_log;

  MACFrame80211* _mac_rts;
  MACFrame80211* _mac_cts;
  MACFrame80211* _mac_data;
  MACFrame80211* _mac_ack;
  int _mac_data_hdr_size;

  SetDefer* _sifs;
  SetDefer* _difs;

  unsigned char _seq_num;

  char _send_pattern[IEEE80211_MTU];
  unsigned _verbose;
};

CLICK_ENDDECLS
#endif
