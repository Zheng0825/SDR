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
 
#ifndef CLICK_TXVECTORGRNEW_HH
#define CLICK_TXVECTORGRNEW_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <click/etheraddress.hh>
#include <clicknet/80211.h>
#include "mpivector.hh"

CLICK_DECLS

class MACFrame80211; 
class SetDefer; 

class TXVectorGrNew : public MPIVector {
public:
  TXVectorGrNew();
  ~TXVectorGrNew();

  const char *class_name() const		{ return "TXVectorGrNew"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);

  Packet *simple_action(Packet *);

  void add_handlers();

  //WritablePacket* prepare_packet(Packet* p);
  Packet* prepare_packet(Packet* p);
  //int header_size() {return _hdr_size;}

protected:
  unsigned _hdr_size;
  int _mac_data_hdr_size;
  int hdr_size; 
  unsigned char _seq_num;
  unsigned _verbose;
  EtherAddress _my_mac_addr; 

  String _logfilename; 
  FILE* _fptr_log;

  MACFrame80211* _mac_rts;
  MACFrame80211* _mac_cts;
  MACFrame80211* _mac_data;
  MACFrame80211* _mac_ack; 

  SetDefer* _sifs;
  SetDefer* _difs;

  void process_rts(Packet* pkt, int hdrsize);
  void process_cts(Packet* pkt, int hdrsize);
  void process_data(Packet* pkt, int hdrsize, int num_uni_data, int num_broad_data);
  void process_ack(Packet* pkt, int hdrsize);
  
  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);
private:
  uint32_t convert_rate(uint16_t anno_rate);
  uint32_t convert_type(uint8_t anno_type);
};

CLICK_ENDDECLS
#endif
