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

#ifndef CLICK_AGGSETCRC32_HH
#define CLICK_AGGSETCRC32_HH
#include <click/element.hh>

CLICK_DECLS

class AggSetCRC32 : public Element { public:
  
  AggSetCRC32();
  ~AggSetCRC32();

  const char *class_name() const	{ return "AggSetCRC32"; }
  const char *port_count() const	{ return PORTS_1_1; }
  const char *processing() const	{ return AGNOSTIC; }
 
  Packet *simple_action(Packet *);

};

CLICK_ENDDECLS
#endif