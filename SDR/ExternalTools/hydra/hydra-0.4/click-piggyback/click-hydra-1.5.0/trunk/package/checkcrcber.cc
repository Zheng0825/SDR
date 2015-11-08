/*
 * checkcrc32.{cc,hh} -- element checks CRC 32 checksum
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
#include <click/hydra_anno.hh>
#include "checkcrcber.hh"
#include "macframe80211.hh"
#include "powercontrol.hh"
CLICK_DECLS

CheckCRCBER::CheckCRCBER()
  : _filename("stdin"), _logfilename("stdout"), _fptr_log(stdout), 
    _mac_rts(0), _mac_cts(0), _mac_data(0), _mac_ack(0),
    _mac_data_hdr_size(0), _power_control(0)
{
  _drops = 0;
}

CheckCRCBER::~CheckCRCBER()
{
}


int CheckCRCBER::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* mac_rts_e = 0;
  Element* mac_cts_e = 0;
  Element* mac_data_e = 0;
  Element* mac_ack_e = 0;
  Element* power_control_e = 0;

  if (cp_va_parse(conf, this, errh,
		  cpEtherAddress, "my MAC address", &_my_mac_addr,
		  cpElement, "rts header format", &mac_rts_e,
		  cpElement, "cts header format", &mac_cts_e,
		  cpElement, "data header format", &mac_data_e,
		  cpElement, "ack header format", &mac_ack_e,
                  cpFilename, "packet pattern", &_filename,
                  cpFilename, "log file", &_logfilename,
		  cpElement, "power control", &power_control_e,
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

  if(power_control_e) {
    _power_control = (PowerControl *) power_control_e->cast("PowerControl");
    if(!_power_control) return errh->error("Power control must be set using PowerControl element");
  } 


  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int CheckCRCBER::initialize(ErrorHandler *errh)
{
    _mac_data_hdr_size = _mac_data->header_size();
    
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
  
  printf("CheckCRCBER: Opened log file %s\n", _logfilename.c_str());
  return 0;
}

Packet *
CheckCRCBER::simple_action(Packet *p)
{
  // Check len
  int len = p->length();
  if(len < 4) 
  {
    fail(p);
    hydra_chatter(8, _verbose, "%{element}: CRC32 failed, len %d", this, p->length());
    return 0;
  }

  // Check CRC
  unsigned int crc = update_crc(0xffffffff, (char *) p->data(), len - 4);
  unsigned int pcrc;
  memcpy(&pcrc, p->data() + len - 4, 4);
  p->take(4);

  if(pcrc != crc)
  {
    struct fc_80211* header = (struct fc_80211*) p->data();
    if(is_rts(*header)) { fail_rts(p); }
    else if(is_cts(*header)) { fail_cts(p); }
    //else if(is_data(*header)) { fail_data(p); }
    else if(is_data(*header)) { fail_data_for_draw(p); }
    else if(is_ack(*header)) { fail_ack(p); }
    else { fail(p); }

    hydra_chatter(8, _verbose, "%{element}: CRC32 failed, len %d", this, p->length());
    p->kill();
    _drops++;
    return 0;
  }

  // If no CRC error, then log success of receiving
  struct fc_80211* header = (struct fc_80211*) p->data();
  if(is_rts(*header))
    success_rts(p);
  else if(is_cts(*header))
    success_cts(p);
  else if(is_data(*header))
    success_data(p);
  else if(is_ack(*header))
    success_ack(p);
  
  return p;
}

void CheckCRCBER::success_rts(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  unsigned char* seq_num = (unsigned char *) pkt->data();

  printf("RX RTS seqn %03d rate %03d snr %05.02f\n", seq_num[1], cha->rate_mbps, cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d RX RTS seqn %03d src %02d dst %02d hlen %02d dlen 0000 rate %02d snr %05.02f\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], seq_num[1],
     _mac_rts->src(pkt)[5], _mac_rts->dst(pkt)[5], 
     pkt->length(), cha->rate_mbps, cha->avg_snr);

}

void CheckCRCBER::success_cts(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  unsigned char* seq_num = (unsigned char *) pkt->data();


  printf("RX CTS seqn %03d rate %03d snr %05.02f\n", seq_num[1], cha->rate_mbps, cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d RX CTS seqn %03d src 99 dst %02d hlen %02d dlen 0000 rate %02d snr %05.02f\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], seq_num[1],
     _mac_cts->dst(pkt)[5], 
     pkt->length(), cha->rate_mbps, cha->avg_snr);

}

void CheckCRCBER::success_data(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  unsigned char* seq_num = (unsigned char *) pkt->data();


  int payload_len = pkt->length() - _mac_data_hdr_size;
  printf("RX DTA seqn %03d rate %03d snr %05.02f\n", seq_num[1], cha->rate_mbps, cha->avg_snr);


  // Temporary for testing
  char* power_level_ptr = (char *)pkt->data() + _mac_data_hdr_size - 2;
  u_int16_t tx_power = 0;
  memcpy(&tx_power, power_level_ptr, sizeof(u_int16_t)); 

  fprintf(_fptr_log, 
     "%d.%06d node %d RX DTA seqn %03d src %02d dst %02d hlen %02d dlen %04d rate %02d snr %05.02f txpower %04d\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], seq_num[1],
     _mac_data->src(pkt)[5], _mac_data->dst(pkt)[5], 
     _mac_data_hdr_size, payload_len, 
     cha->rate_mbps, cha->avg_snr, tx_power);

  _power_control->set_power((unsigned) tx_power);
}

void CheckCRCBER::success_ack(Packet* pkt)
{

  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  unsigned char* seq_num = (unsigned char *) pkt->data();

  printf("RX ACK seqn %03d rate %03d snr %05.02f\n", seq_num[1], cha->rate_mbps, cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d RX ACK seqn %03d src 99 dst %02d hlen %02d dlen 0000 rate %02d snr %05.02f\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], seq_num[1],
     _mac_ack->dst(pkt)[5], 
     pkt->length(), cha->rate_mbps, cha->avg_snr);
}

void CheckCRCBER::fail_rts(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  printf("DP RTS seqn 999 rate 99 snr %05.02f\n", cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d DP RTS seqn 999 src 99 dst 99 hlen %02d dlen 0000 rate %02d snr %05.02f\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 
     pkt->length(), cha->rate_mbps, cha->avg_snr);
}

void CheckCRCBER::fail_cts(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();

  printf("DP CTS seqn 999 rate 99 snr %05.02f\n", cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d DP CTS seqn 999 src 99 dst 99 hlen %02d dlen 0000 rate %02d snr %05.02f\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 
     pkt->length(), cha->rate_mbps, cha->avg_snr);

}

void CheckCRCBER::fail_data(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha =
         (struct click_hydra_anno *) pkt->all_user_anno();

  printf("DP DTA seqn 999 rate 99 snr %05.02f\n", cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d DP DTA seqn 999 src 99 dst 99 hlen %02d dlen 0000 rate %02d snr %05.02f ",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 
     pkt->length(), cha->rate_mbps, cha->avg_snr);

  // CHECK BER
  char* payload_data = (char *)pkt->data() + _mac_data_hdr_size;
  int payload_len = pkt->length() - _mac_data_hdr_size;
  if(payload_len > IEEE80211_MTU)
    payload_len = IEEE80211_MTU;

  int success = 0;
  int fail = 0;
  for(int i=0; i<payload_len; i++)
  {
    for(int j=0; j<8; j++)
    {
      unsigned char mask = 0x01 << j;
      char recv_bit = payload_data[i] & mask;
      char org_bit = _send_pattern[i] & mask;
      if(recv_bit == org_bit)
         success++;
      else
         fail++;
    }
  }

  int total = success + fail;
  float ber = ((float) fail) / ((float) total);
  int packet_error = 0;

  fprintf(_fptr_log, "ber %05.03f total %d success %d fail %d\n", 
        ber, total, success, fail); 

}

void CheckCRCBER::fail_data_for_draw(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  unsigned char* seq_num = (unsigned char *) pkt->data();


  int payload_len = pkt->length() - _mac_data_hdr_size;
  printf("DP DTA seqn %03d rate %03d snr %05.02f\n", seq_num[1], cha->rate_mbps, cha->avg_snr);


  // Temporary for testing
  char* power_level_ptr = (char *)pkt->data() + _mac_data_hdr_size - 2;
  u_int16_t tx_power = 0;
  memcpy(&tx_power, power_level_ptr, sizeof(u_int16_t)); 

  // XXX: Sequence number 999 means data packet was corrupted.
  //      So, in Matlab, it should be dark circle, while success is drawn
  //      as white circle.
  fprintf(_fptr_log, 
     "%d.%06d node %d DP DTA seqn 999 src %02d dst %02d hlen %02d dlen %04d rate %02d snr %05.02f txpower %04d\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 
     _mac_data->src(pkt)[5], _mac_data->dst(pkt)[5], 
     _mac_data_hdr_size, payload_len, 
     cha->rate_mbps, cha->avg_snr, tx_power);

  //_power_control->set_power((unsigned) tx_power);
}

void CheckCRCBER::fail_ack(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  printf("DP ACK seqn 999 rate 99 snr %05.02f\n", cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d DP ACK seqn 999 src 99 dst 99 hlen %02d dlen 0000 rate %02d snr %05.02f\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 
     pkt->length(), cha->rate_mbps, cha->avg_snr);
}

void CheckCRCBER::fail(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  printf("DP UKN seqn 999 rate 99 snr %05.02f\n", cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d DP UKN seqn 999 src 99 dst 99 hlen %02d dlen 0000 rate %02d snr %05.02f\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 
     pkt->length(), cha->rate_mbps, cha->avg_snr);

}



CLICK_ENDDECLS
EXPORT_ELEMENT(CheckCRCBER)
ELEMENT_MT_SAFE(CheckCRCBER)
