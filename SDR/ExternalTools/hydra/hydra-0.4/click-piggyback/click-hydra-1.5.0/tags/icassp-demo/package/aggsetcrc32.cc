
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
