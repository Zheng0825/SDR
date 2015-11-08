/*
 * setcrc32.{cc,hh} -- element sets CRC 32 checksum
 * Robert Morris
 * Soon-Hyeok Choi
 *
 * Copyright (C) 2009  The University of Texas at Austin.
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
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
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/crc32.h>
#include "setcrcber.hh"
#include <click/hydra_anno.hh>
#include "macframe80211.hh"
#include "setdefer.hh"

CLICK_DECLS

SetCRCBER::SetCRCBER()
  : _filename("stdin"), _logfilename("stdout"), _fptr_log(stdout), 
    _mac_rts(0), _mac_cts(0), _mac_data(0), _mac_ack(0), _mac_data_hdr_size(0),
    _sifs(0), _difs(0), 
    _seq_num(0)
{
}

SetCRCBER::~SetCRCBER()
{
}

int
SetCRCBER::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* mac_rts_e = 0;
  Element* mac_cts_e = 0;
  Element* mac_data_e = 0;
  Element* mac_ack_e = 0;
  Element* sifs_e = 0;
  Element* difs_e = 0;
  if (cp_va_parse(conf, this, errh,
		  cpEtherAddress, "my MAC address", &_my_mac_addr,
		  cpElement, "rts header format", &mac_rts_e,
		  cpElement, "cts header format", &mac_cts_e,
		  cpElement, "data header format", &mac_data_e,
		  cpElement, "ack header format", &mac_ack_e,
		  cpElement, "SIFS", &sifs_e,
		  cpElement, "DIFS", &difs_e,
                  cpFilename, "test pattern", &_filename,
                  cpFilename, "log file", &_logfilename,
		  cpOptional,
		  cpKeywords, 
 		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  if(mac_rts_e) {
    _mac_rts = (MACFrame80211 *) mac_rts_e->cast("MACFrame80211");
    if(!_mac_rts) return errh->error("RTS Frame must be set using MACFrame80211");
  } 
  if(mac_cts_e) {
    _mac_cts = (MACFrame80211 *) mac_cts_e->cast("MACFrame80211");
    if(!_mac_cts) return errh->error("CTS Frame must be set using MACFrame80211");
  } 
  if(mac_data_e) {
    _mac_data = (MACFrame80211 *) mac_data_e->cast("MACFrame80211");
    if(!_mac_data) return errh->error("Data Frame must be set using MACFrame80211");
  } 
  if(mac_ack_e) {
    _mac_ack = (MACFrame80211 *) mac_ack_e->cast("MACFrame80211");
    if(!_mac_ack) return errh->error("ACK Frame must be set using MACFrame80211");
  } 

  if(sifs_e) {
    _sifs = (SetDefer *) sifs_e->cast("SetDefer");
    if(!_sifs) return errh->error("SIFS should be defined using SetDefer");
  } 

  if(difs_e) {
    _difs = (SetDefer *) difs_e->cast("SetDefer");
    if(!_difs) return errh->error("SIFS should be defined using SetDefer");
  } 

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
SetCRCBER::initialize(ErrorHandler *errh)
{
  _mac_data_hdr_size = _mac_data->header_size();
 
  // open pattern file   
  FILE* fptr = 0;
  if (_filename != "stdin") 
  {
    fptr = fopen(_filename.c_str(), "rb");
    hydra_chatter(9, _verbose, "%{element}::configure input file name %s\n", this, _filename.c_str());
    if (!fptr)
      return errh->error("%s: %s", _filename.c_str(), strerror(errno));
  } 
  else 
  {
    fptr = stdin;
  }

  // read file
  size_t read_size = 0;
  size_t remain_size = IEEE80211_MTU;
  while(remain_size > 0)
  {
    size_t current_read = fread((void *)(_send_pattern + read_size), sizeof(char), remain_size, fptr);

    read_size += current_read;
    remain_size -= current_read;
  }

  if(fptr != stdin)
    fclose(fptr); 


  hydra_chatter(9, _verbose, "%{element}::configure output file name %s\n", this, _logfilename.c_str());
  if (_logfilename != "stdout") 
  {
    _fptr_log = fopen(_logfilename.c_str(), "wt");
    if (!_fptr_log)
      return errh->error("%s: %s", _logfilename.c_str(), strerror(errno));
  } 
  
  printf("SetCRCBER: Opened log file %s\n", _logfilename.c_str());
  return 0;
}

Packet *
SetCRCBER::simple_action(Packet *p)
{
  struct fc_80211* header = (struct fc_80211*) p->data();
  if(is_rts(*header))
    process_rts(p);
  else if(is_cts(*header))
    process_cts(p);
  else if(is_data(*header))
    process_data(p);
  else if(is_ack(*header))
    process_ack(p);

  // It uses second bytes of fc header which is currently not used
  // For OAR it should not use this header filed
  unsigned char* fc_temp = (unsigned char *) p->data();
  fc_temp[1] = _seq_num;
  _seq_num++;

  // calculate crc
  int len = p->length();
  unsigned int crc = 0xffffffff;
  crc = update_crc(crc, (char *) p->data(), len);

  WritablePacket *q = p->put(4);
  memcpy(q->data() + len, &crc, 4);
  
  return(q);
}

void SetCRCBER::process_rts(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();

  char defer_type[5];
  int defer_type_int = 0;
  int slot_size = 0;
  if(cha->defer_usec == _difs->real_defer(pkt->length()))
  {
    memcpy(defer_type, "difs\0", 5);
    defer_type_int = 1;
    slot_size = cha->slot/_difs->slot_time();
  }
  else
  {
    memcpy(defer_type, "sifs\0", 5);
  }


  printf("TX RTS seqn %03d rate %02d txpower %04d\n", _seq_num, cha->rate_mbps, cha->power);
  fprintf(_fptr_log, 
     "%d.%06d node %d TX RTS seqn %03d src %02d dst %02d hlen %02d dlen 0000 rate %02d txpower %04d %s %d slot_num %04d\n",
     now.tv_sec, now.tv_usec, 
     _my_mac_addr.data()[5], _seq_num, 
     _mac_rts->src(pkt)[5], _mac_rts->dst(pkt)[5], 
     pkt->length(), cha->rate_mbps, cha->power,
     defer_type, defer_type_int, slot_size);
}


void SetCRCBER::process_cts(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();

  char defer_type[5];
  int defer_type_int = 0;
  int slot_size = 0;
  if(cha->defer_usec == _difs->real_defer(pkt->length()))
  {
    memcpy(defer_type, "difs\0", 5);
    defer_type_int = 1;
    slot_size = cha->slot/_difs->slot_time();
  }
  else
  {
    memcpy(defer_type, "sifs\0", 5);
  }

  printf("TX CTS seqn %03d rate %02d txpower %04d\n", _seq_num, cha->rate_mbps, cha->power);
  fprintf(_fptr_log, 
     "%d.%06d node %d TX CTS seqn %03d src 99 dst %02d hlen %02d dlen 0000 rate %02d txpower %04d %s %d slot_num %04d\n",
     now.tv_sec, now.tv_usec, 
     _my_mac_addr.data()[5], _seq_num, 
     _mac_cts->dst(pkt)[5], 
     pkt->length(), cha->rate_mbps, cha->power, 
     defer_type, defer_type_int, slot_size);

}

void SetCRCBER::process_data(Packet* pkt)
{
  char* payload_data = (char *)pkt->data() + _mac_data_hdr_size;
  int payload_len = pkt->length() - _mac_data_hdr_size;
  if(payload_len > IEEE80211_MTU)
    payload_len = IEEE80211_MTU;

  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();

  char defer_type[5];
  int defer_type_int = 0;
  int slot_size = 0;
  if(cha->defer_usec == _difs->real_defer(pkt->length()))
  {
    memcpy(defer_type, "difs\0", 5);
    defer_type_int = 1;
    slot_size = cha->slot/_difs->slot_time();
  }
  else
  {
    memcpy(defer_type, "sifs\0", 5);
  }

  printf("TX DTA seqn %03d rate %02d txpower %04d\n", _seq_num, cha->rate_mbps, cha->power);
  fprintf(_fptr_log, 
     "%d.%06d node %d TX DTA seqn %03d src %02d dst %02d hlen %02d dlen %04d rate %02d txpower %04d %s %d slot_num %04d\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], _seq_num, 
     _mac_data->src(pkt)[5], _mac_data->dst(pkt)[5], 
     _mac_data_hdr_size, payload_len,
     cha->rate_mbps, cha->power, 
     defer_type, defer_type_int, slot_size);


  memcpy(payload_data, _send_pattern, payload_len);

  // piggyback power level on data header.
  // temporary for testing
  // It uses seq number field of data header.
  char* power_level_ptr = (char *)pkt->data() + _mac_data_hdr_size - 2;
  memcpy(power_level_ptr, &cha->power, sizeof(u_int16_t));
  //printf("tx power = %d, piggybacked = %d\n", cha->power, *(u_int16_t *) power_level_ptr);

}

void SetCRCBER::process_ack(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();

  char defer_type[5];
  int defer_type_int = 0;
  int slot_size = 0;
  if(cha->defer_usec == _difs->real_defer(pkt->length()))
  {
    memcpy(defer_type, "difs\0", 5);
    defer_type_int = 1;
    slot_size = cha->slot/_difs->slot_time();
  }
  else
  {
    memcpy(defer_type, "sifs\0", 5);
  }

  printf("TX ACK seqn %03d rate %02d txpower %04d\n", _seq_num, cha->rate_mbps, cha->power);
  fprintf(_fptr_log, 
      "%d.%06d node %d TX ACK seqn %03d src 99 dst %02d hlen %02d dlen 0000 rate %02d txpower %04d %s %d slot_num %04d\n",
      now.tv_sec, now.tv_usec,
      _my_mac_addr.data()[5], _seq_num, 
      _mac_ack->dst(pkt)[5], 
      pkt->length(), cha->rate_mbps, cha->power,
     defer_type, defer_type_int, slot_size);

}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetCRCBER)
ELEMENT_MT_SAFE(SetCRCBER)
