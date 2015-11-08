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

#include <click/config.h>
#include <click/crc32.h>
#include <clicknet/80211.h>
#include "aggsetcrc32.hh"

CLICK_DECLS

AggSetCRC32::AggSetCRC32()
{
}

AggSetCRC32::~AggSetCRC32()
{
}

Packet *
AggSetCRC32::simple_action(Packet *p)
{
  int total_len = p->length();
  unsigned int crc = 0xffffffff;
  WritablePacket* q = p->uniqueify();
  unsigned char* start_pos = q->data();

  int offset = 0;
  while(offset < total_len)
  {
    struct data_80211* data_hdr = (struct data_80211 *) (start_pos + offset); 
    int packet_size = *(uint16_t *) data_hdr->i_seq;
    uint32_t cur_crc = 
	     update_crc(crc, (char *) (start_pos + offset), packet_size);
    offset += packet_size;
    memcpy((start_pos + offset), &cur_crc, 4);
    offset += 4; // crc
    offset += ((packet_size % 4) == 0 ) ? 0 : (4 - (packet_size % 4)); // pad
  }
 
  assert(offset == total_len); 

  return(q);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AggSetCRC32)
