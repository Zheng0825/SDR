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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/txvector.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_TXVECTOR_HH
#define CLICK_TXVECTOR_HH
#include <click/element.hh>
#include <click/atomic.hh>

#include "mpivector.hh"

CLICK_DECLS

/*
 * =c
 * TXVector()
 * =s 
 * convert or generate MAC frame
 * =d
 * Description goes here.
 * =a TXVector
 */


class TXVector : public MPIVector {
public:
  TXVector();
  ~TXVector();

  const char *class_name() const		{ return "TXVector"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

  void add_handlers();

  //WritablePacket* prepare_packet(Packet* p);
  Packet* prepare_packet(Packet* p);
  //int header_size() {return _hdr_size;}

protected:
  unsigned _hdr_size;
  //unsigned _verbose;

  
  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);
private:
  uint8_t convert_rate(uint16_t anno_rate);
  uint8_t convert_type(uint8_t anno_type);
};

CLICK_ENDDECLS
#endif
