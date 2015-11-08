/*
 * deaggregator.{cc,hh}
 * Wonsoo Kim
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
#include "deaggregator.hh"
#include "macframe80211.hh"
#include "powercontrol.hh"

#define  CRC_SIZE 4

CLICK_DECLS

DeAggregator::DeAggregator(): _logfilename("stdout"), _fptr_log(stdout), _mac_rts(0), _mac_cts(0), _mac_data(0), _mac_ack(0), _mac_data_hdr_size(0)
{
  hydra_chatter(9,8, "%{element}::constructor", this);
}

DeAggregator::~DeAggregator()
{
}

int DeAggregator::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* mac_rts_e = 0;
  Element* mac_cts_e = 0;
  Element* mac_data_e = 0;
  Element* mac_ack_e = 0;

  if (cp_va_parse(conf, this, errh,
		  cpEtherAddress, "my MAC address", &_my_mac_addr,
		  cpElement, "rts header format", &mac_rts_e,
		  cpElement, "cts header format", &mac_cts_e,
		  cpElement, "data header format", &mac_data_e,
		  cpElement, "ack header format", &mac_ack_e,
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

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int DeAggregator::initialize(ErrorHandler *errh)
{
  _mac_data_hdr_size = _mac_data->header_size();
    
  hydra_chatter(9, _verbose, "%{element}::configure output file name %s\n", this, _logfilename.c_str());
  if (_logfilename != "stdout") 
  {
     _fptr_log = fopen(_logfilename.c_str(), "wt");
     if (!_fptr_log)
        return errh->error("%s: %s", _logfilename.c_str(), strerror(errno));
  } 
  
  printf("DeAggregator: Opened log file %s\n", _logfilename.c_str());
  
  return 0;
}

void 
DeAggregator::push(int port, Packet *p)
{
  // Check length (MAC frame length)
  int len = p->length();
  if(len < 4) 
  {
    fail(p);
    hydra_chatter(8, _verbose, "%{element}: Deaggregation/CRC32 failed, len %d", this, p->length());
    p->kill();
    return;
  }
  
  // parse annotation for deaggregation
  struct click_hydra_anno* cha=
	  (struct click_hydra_anno *) p->all_user_anno();
   
  uint32_t len_broadcast = cha->agg_anno->len_broadcast;
  uint32_t len_unicast = cha->agg_anno->len_unicast;
  bool crcerror=false; 
  int total_size=0; 
  WritablePacket* frag_p;
  struct data_80211* data_header;
  struct fc_80211* header;
  uint16_t frag_len=0;
  uint16_t PAD_octet=0;
 
  hydra_chatter(1, _verbose, "%{element}::push:len_broadcast %d len_unicast %d", this, len_broadcast, len_unicast);
  // Deaggregation
  // 1. Processing Broadcast data
  while(len_broadcast>0)
  {
    hydra_chatter(1, _verbose, "%{element}::push: process broadcast frame", this);
    header = (struct fc_80211*)(p->data()+total_size);
    if(is_data(*header)) 
    {  
       data_header=(struct data_80211*)(p->data()+total_size);
       // seq is used for length of a fragmentized frame
       frag_len=*((uint16_t *)(data_header->i_seq)); 
       frag_p=Packet::make(HYDRA_HEADROOM, (unsigned char*)(p->data()+total_size), frag_len+CRC_SIZE, HYDRA_TAILROOM);
       struct click_hydra_anno* frag_cha=
	      (struct click_hydra_anno *) frag_p->all_user_anno(); 
       agg_anno_allocator(frag_p, cha->agg_anno->len_metadata);
       frag_cha->rate_mbps=cha->rate_mbps;   
       frag_cha->agg_anno->len_metadata = cha->agg_anno->len_metadata; 
       frag_cha->avg_snr=10.0*log10(cha->avg_snr); 
       memcpy(frag_cha->agg_anno->metadata, cha->agg_anno->metadata, frag_cha->agg_anno->len_metadata); 
       frag_p->set_hydra_destructor(agg_anno_destructor); 

       if((frag_len+CRC_SIZE)==(len_unicast+len_broadcast)) //No aggregation
       {	 
	        PAD_octet=0;
       }
       else 
       {	  
	       if(((frag_len+CRC_SIZE)%4)==0)    
	         PAD_octet=0;
         else
	         PAD_octet=4-((frag_len+CRC_SIZE)%4);
       } 
       crcerror=check_crc(frag_p, frag_len+CRC_SIZE); 
       
    }
    else  //error case: only broadcast data exists.
    {  
       fail(p);
       hydra_chatter(8, _verbose, "%{element}: Broadcast: Fail to check frame type(only data allowed), len %d", this, p->length());
       p->kill();
       _drops++;

       return; 
    } 

    // Drop the aggregated packet!!
    if(crcerror)
    {
       //fail(p);
       hydra_chatter(8, _verbose, "%{element}: Broadcast:CRC failed, len %d", this, p->length());
       p->kill();
       _drops++;

       return; 
    }
    else
    { 
       //New annotation for deaggregated frame
       struct click_hydra_anno* frag_cha=
             (struct click_hydra_anno *) frag_p->all_user_anno();
       agg_anno_allocator(frag_p, cha->agg_anno->len_metadata);
       frag_cha->type=cha->type;
       frag_cha->agg_anno->len_metadata=cha->agg_anno->len_metadata;
       frag_cha->agg_anno->len_broadcast=cha->agg_anno->len_broadcast;
       frag_cha->agg_anno->len_unicast=cha->agg_anno->len_unicast;
       frag_cha->agg_anno->firstframe=0;
       frag_cha->agg_anno->lastframe=0;
       frag_cha->rate_mbps=cha->rate_mbps;
       frag_cha->avg_snr=10.0*log10(cha->avg_snr);
       //frag_cha->avg_snr=cha->avg_snr;
       memcpy(frag_cha->agg_anno->metadata, cha->agg_anno->metadata, frag_cha->agg_anno->len_metadata);
       frag_p->set_hydra_destructor(agg_anno_destructor);
       frag_p->take(4);
       total_size=total_size+frag_len+CRC_SIZE+PAD_octet;
       len_broadcast=len_broadcast-(frag_len+CRC_SIZE+PAD_octet);
       hydra_chatter(8, _verbose, "%{element}:sending up broadcast frame, firstframe %d lastframe %d", this, frag_cha->agg_anno->firstframe, frag_cha->agg_anno->lastframe);

       success_data(frag_p);
       output(0).push(frag_p);	

    }
    
  }  

  bool success_unicast=true;
  int num_unicast=0;
  crcerror=false;
  Unicast unicast_p; 
  _unicast.clear();

  // 2.Process Unicast data
  while(len_unicast>0)
  {
    hydra_chatter(1, _verbose, "%{element}::push,processing unicast frame:len_unicast %d", this, len_unicast);
    header = (struct fc_80211*)(p->data()+total_size);
     
    // 4 data types: DATA/RTS/CTS/ACK    
    if(is_data(*header)) 
    {
       data_header=(struct data_80211*) (p->data()+total_size);
       // FIXME: use the next line -1 +1
       //frag_len=len_unicast-CRC_SIZE;  // CSI piggyback
       frag_len=*((uint16_t *)(data_header->i_seq)); 
       hydra_chatter(1, _verbose, "%{element}::push,processing data:frag_len(wo CRC) %d", this, frag_len);
    }
    else if(is_rts(*header))
    {
       frag_len=18; 
       hydra_chatter(1, _verbose, "%{element}::push,processing rts:frag_len(wo CRC) %d", this, frag_len);
    }
    else if(is_cts(*header))
    {
       // No Aggregation for CTS 
       frag_len=len_unicast-CRC_SIZE;  
       hydra_chatter(1, _verbose, "%{element}::push,processing cts:frag_len(wo CRC) %d", this, frag_len);
    }
    else if(is_ack(*header))
    {
       frag_len=12; 
       hydra_chatter(1, _verbose, "%{element}::push,processing ack:frag_len(wo CRC) %d", this, frag_len);
    }
    else //Error case 
    {
       fail(p);
       hydra_chatter(8, _verbose, "%{element}: Fail to check frame_type, len %d", this, p->length());
       p->kill();
       _drops++;

      return;
    }

    //Deaggregation and CheckCRC 
    frag_p=Packet::make(HYDRA_HEADROOM, (unsigned char*)(p->data()+total_size), frag_len+CRC_SIZE, HYDRA_TAILROOM);
    struct click_hydra_anno* frag_cha=
       (struct click_hydra_anno *) frag_p->all_user_anno(); 
    agg_anno_allocator(frag_p, cha->agg_anno->len_metadata);
    frag_cha->rate_mbps=cha->rate_mbps;   
    frag_cha->agg_anno->len_metadata = cha->agg_anno->len_metadata; 
    frag_cha->avg_snr=10.0*log10(cha->avg_snr); 
    memcpy(frag_cha->agg_anno->metadata, cha->agg_anno->metadata, frag_cha->agg_anno->len_metadata); 
    frag_p->set_hydra_destructor(agg_anno_destructor); 
    
    if((frag_len+CRC_SIZE)==len_unicast) //No aggregation
    {
       PAD_octet=0;
    }
    else 
    {  
       if((frag_len+CRC_SIZE)%4 == 0)
	   PAD_octet=0;
       else
	   PAD_octet=4-((frag_len+CRC_SIZE)%4);
    }
    crcerror=check_crc(frag_p, frag_len+CRC_SIZE);

    // Based on CRC, process further
    if(crcerror)
    {
      //fail(p);
      hydra_chatter(8, _verbose, "%{element}: CRC failed, len %d", this, p->length());
      p->kill();
      _drops++;

      return;
    }
    else //SUCCESS
    {
       //Insert frag_p into Vector _unicast
       frag_p->take(CRC_SIZE);
       unicast_p.frag_data=Packet::make(HYDRA_HEADROOM, (unsigned char*)frag_p->data(), frag_len, HYDRA_TAILROOM);
       unicast_p.len=frag_len;
       _unicast.push_back(unicast_p);
       total_size=total_size+frag_len+CRC_SIZE+PAD_octet;
       len_unicast=len_unicast-(frag_len+CRC_SIZE+PAD_octet); 
       num_unicast=num_unicast+1;
    }
  
  }  

  WritablePacket* unicast_data;
  //success of AMPDU: send up all MPDU
  if(success_unicast)
  {
     for(int n=0;n<num_unicast; n++)
     {   
        unicast_data=Packet::make(HYDRA_HEADROOM, (unsigned char*)_unicast[n].frag_data->data(), _unicast[n].len, HYDRA_TAILROOM);
        //New annotation for deaggregated frame
        struct click_hydra_anno* frag_cha=
	             (struct click_hydra_anno *) unicast_data->all_user_anno();
        agg_anno_allocator(unicast_data, cha->agg_anno->len_metadata);
      	frag_cha->type=cha->type;
        frag_cha->agg_anno->len_metadata=cha->agg_anno->len_metadata;
        frag_cha->agg_anno->len_broadcast=cha->agg_anno->len_broadcast;
        frag_cha->agg_anno->len_unicast=cha->agg_anno->len_unicast;
        frag_cha->agg_anno->firstframe=0;
        frag_cha->agg_anno->lastframe=0;
        frag_cha->rate_mbps=cha->rate_mbps;
        frag_cha->avg_snr=10.0*log10(cha->avg_snr);
        memcpy(frag_cha->agg_anno->metadata, cha->agg_anno->metadata, frag_cha->agg_anno->len_metadata);
	frag_p->set_hydra_destructor(agg_anno_destructor);
	
	if(n==0)
         frag_cha->agg_anno->firstframe=1;
	if(n==(num_unicast-1))
         frag_cha->agg_anno->lastframe=1;

        // FOR DEBUGGING
        hydra_chatter(1, _verbose, "%{element}::push, len_metadata=%d, metadata=%d, firstframe=%d, lastframe=%d", this, frag_cha->agg_anno->len_metadata, frag_cha->agg_anno->metadata[0], frag_cha->agg_anno->firstframe, frag_cha->agg_anno->lastframe);
        
        // Log files to measure throughput
        header = (struct fc_80211*)unicast_data->data();
     
        // 4 data types: DATA/RTS/CTS/ACK    
        if(is_data(*header))
          success_data(unicast_data);
        if(is_rts(*header)) 
          success_rts(unicast_data);
        if(is_cts(*header)) 
          success_cts(unicast_data);
        if(is_ack(*header)) 
          success_ack(unicast_data);
              
	
        output(0).push(unicast_data);
     }
  } 
  else  //Drop unicast frames
  {
     fail(p);
     hydra_chatter(8, _verbose, "%{element}: Unicast:Deaggregation failed, len %d", this, p->length());
     _drops++;

  } 
  
  p->kill(); // remove the aggregated packet to free memory  
  return;
}

bool DeAggregator::check_crc(Packet* pkt, uint16_t len)
{

  // Check CRC for each MPDU
  unsigned int crc = update_crc(0xffffffff, (char *) pkt->data(), len-CRC_SIZE);
  unsigned int pcrc;
  bool crc_error;
  
  memcpy(&pcrc, pkt->data()+len-CRC_SIZE, CRC_SIZE);

  if(pcrc != crc)
  {
    crc_error=true;
    fail(pkt); 
  }
  else
    crc_error=false;
  
  return crc_error;
}


void DeAggregator::success_rts(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  //unsigned char* seq_num = (unsigned char *) pkt->data();
  
  float drate=0.0; 
  
  switch(cha->rate_mbps)
  {
    case 6:  drate=6.5; break; 
    case 9:  drate=13.0; break; 
    case 12: drate=19.5; break; 
    case 18: drate=26.0; break; 
    case 24: drate=39.0; break;
    case 36: drate=52.0; break; 
    case 48: drate=58.5; break;
    case 54: drate=65.0; break;
    default: drate=(float) cha->rate_mbps; 
  }

  //printf("RX RTS seqn %03d rate %03d snr %05.02f\n", 0, cha->rate_mbps, cha->avg_snr);
  printf("RX RTS seqn %03d rate %02.01f snr %05.02f\n", 0, drate, cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d RX RTS seqn %03d src %02d dst %02d hlen %02d dlen 0000 rate %02.01f snr %05.02f blen 0000\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 0,
     _mac_rts->src(pkt)[5], _mac_rts->dst(pkt)[5], 
     pkt->length(), drate, cha->avg_snr);
}

void DeAggregator::success_cts(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  //unsigned char* seq_num = (unsigned char *) pkt->data();
  
  float drate=0.0; 
  
  switch(cha->rate_mbps)
  {
    case 6:  drate=6.5; break; 
    case 9:  drate=13.0; break; 
    case 12: drate=19.5; break; 
    case 18: drate=26.0; break; 
    case 24: drate=39.0; break;
    case 36: drate=52.0; break; 
    case 48: drate=58.5; break;
    case 54: drate=65.0; break;
    default: drate=(float) cha->rate_mbps; 
  }

  //printf("RX CTS seqn %03d rate %03d snr %05.02f\n", 0, cha->rate_mbps, cha->avg_snr);
  printf("RX CTS seqn %03d rate %02.01f snr %05.02f\n", 0, drate, cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d RX CTS seqn %03d src 99 dst %02d hlen %02d dlen 0000 rate %02.01f snr %05.02f blen 0000\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 0,
     _mac_cts->dst(pkt)[5], 
     pkt->length(), drate, cha->avg_snr);

}

void DeAggregator::success_data(Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  //unsigned char* seq_num = (unsigned char *) pkt->data();
  uint32_t broad_len=0;  
  float drate=0.0; 
  
  
   if(cha->agg_anno->len_broadcast==0)
     broad_len=0; 
   else
     broad_len=cha->agg_anno->len_broadcast-CRC_SIZE-_mac_data_hdr_size;
  
  switch(cha->rate_mbps)
  {
    case 6:  drate=6.5; break; 
    case 9:  drate=13.0; break; 
    case 12: drate=19.5; break; 
    case 18: drate=26.0; break; 
    case 24: drate=39.0; break;
    case 36: drate=52.0; break; 
    case 48: drate=58.5; break;
    case 54: drate=65.0; break;
    default: drate=(float) cha->rate_mbps; 
  }

  int payload_len=pkt->length()-_mac_data_hdr_size;
  printf("RX DTA seqn %03d rate %02.01f snr %05.02f\n", 0, drate, cha->avg_snr);

  fprintf(_fptr_log, 
     "%d.%06d node %d RX DTA seqn %03d src %02d dst %02d hlen %02d dlen %04d rate %02.01f snr %05.02f blen %04d txpower %04d\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 0,
     _mac_data->src(pkt)[5], _mac_data->dst(pkt)[5], 
     _mac_data_hdr_size, payload_len, 
     drate, cha->avg_snr, broad_len, 0);

 // _power_control->set_power((unsigned) tx_power);
}

void DeAggregator::success_ack(Packet* pkt)
{

  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  //unsigned char* seq_num = (unsigned char *) pkt->data();
  
  float drate=0.0; 
  
  switch(cha->rate_mbps)
  {
    case 6:  drate=6.5; break; 
    case 9:  drate=13.0; break; 
    case 12: drate=19.5; break; 
    case 18: drate=26.0; break; 
    case 24: drate=39.0; break;
    case 36: drate=52.0; break; 
    case 48: drate=58.5; break;
    case 54: drate=65.0; break;
    default: drate=(float) cha->rate_mbps; 
  }

  //printf("RX ACK seqn %03d rate %02.01f snr %05.02f\n", 0, cha->rate_mbps, cha->avg_snr);
  printf("RX ACK seqn %03d rate %02.01f snr %05.02f\n", 0, drate, cha->avg_snr);
  fprintf(_fptr_log, 
     "%d.%06d node %d RX ACK seqn %03d src 99 dst %02d hlen %02d dlen 0000 rate %02.01f snr %05.02f blen 0000\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 0,
     _mac_ack->dst(pkt)[5], 
     pkt->length(), drate, cha->avg_snr);
}
/*
void DeAggregator::fail_rts(Packet* pkt)
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

void DeAggregator::fail_cts(Packet* pkt)
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

void DeAggregator::fail_data(Packet* pkt)
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

void DeAggregator::fail_data_for_draw(Packet* pkt)
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

void DeAggregator::fail_ack(Packet* pkt)
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
*/
void DeAggregator::fail(Packet* pkt)
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
EXPORT_ELEMENT(DeAggregator)
