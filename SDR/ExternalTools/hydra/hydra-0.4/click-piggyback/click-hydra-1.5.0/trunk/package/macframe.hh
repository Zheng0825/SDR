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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframe.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_MACFRAME_HH
#define CLICK_MACFRAME_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <click/etheraddress.hh>

#include <click/hydra_anno.hh>

CLICK_DECLS

/*
 * =c
 * MACFrame()
 * =s 
 * convert or generate MAC frame
 * =d
 * Description goes here.
 * =a MACFrame
 */


class MACFrame : public Element {
public:
  MACFrame();
  ~MACFrame();

  const char *class_name() const		{ return "MACFrame"; }
  const char *port_count() const		{ return "-/-"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

  void add_handlers();

  virtual WritablePacket* prepare_packet(Packet* p);
  virtual WritablePacket* prepare_packet(Packet* p, EtherAddress& dst);
  virtual const unsigned char* dst(Packet* p);  
  virtual const unsigned char* src(Packet* p);  
  virtual WritablePacket* set_header(Packet* p);
  virtual int get_header(Packet* p, void* hdr);
  virtual int header_size() {return 0;}
  virtual int get_feedback(Packet* , void* ) { return 0;}

protected:
  EtherAddress _mac_addr;

  atomic_uint32_t _drops;

  unsigned _verbose;

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

};

CLICK_ENDDECLS
#endif
