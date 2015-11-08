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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/fromlabviewphy.hh,v $
 *  
 ***********************************************************/

#ifndef FROMLABVIEWPHY_HH
#define FROMLABVIEWPHY_HH
#include <click/element.hh>

#include "fromphy.hh"
CLICK_DECLS

class FromLabviewPhy : public FromPhy {

public:

  FromLabviewPhy();
  ~FromLabviewPhy();

  const char *class_name() const { return "FromLabviewPhy"; }
  const char *port_count() const { return "1/2"; }
  const char *processing() const { return PUSH; }

  int configure(Vector<String> &, ErrorHandler *);

  void push(int, Packet *);

  void add_handlers();

protected:
  //unsigned _verbose;
  struct timeval _rx_start;

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
