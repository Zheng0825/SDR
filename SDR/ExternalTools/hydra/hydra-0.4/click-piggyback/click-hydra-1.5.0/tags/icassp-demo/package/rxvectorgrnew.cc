/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>

#include <clicknet/mpi.h>
#include "rxvectorgrnew.hh"

CLICK_DECLS

RXVectorGrNew::RXVectorGrNew() :_hdr_size( sizeof(struct gr_rxvector_new) )
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RXVectorGrNew::~RXVectorGrNew()
{
}

void*
RXVectorGrNew::cast(const char *n)
{
    if (strcmp(n, "RXVectorGrNew") == 0)
	return (RXVectorGrNew *) this;
    
    return MPIVector::cast(n);
}

int
RXVectorGrNew::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
 			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}


Packet *
RXVectorGrNew::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);
  if(!p || !p->length())
  {
    if(p) p->kill();
    return 0;
  }
  
  //WritablePacket* p_out = prepare_packet(p);
  Packet* p_out = prepare_packet(p);

  return p_out;
}

Packet*
RXVectorGrNew::prepare_packet(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::prepare_packet:", this);
 
  //struct timeval now;
  //click_gettimeofday(&now);
  //printf("[rxvector]rx_time = %d.%06d\n", now.tv_sec, now.tv_usec);
 
  Packet* p_out = p;
  
  struct gr_rxvector_new* rxv = (struct gr_rxvector_new *) p_out->data();

  struct click_hydra_anno* cha = 
		(struct click_hydra_anno *) p_out->all_user_anno();
  int pktlength=rxv->len_broadcast+rxv->len_unicast;
  int hdr_size=0;
  hdr_size=_hdr_size-1+rxv->len_metadata;
  
  hydra_chatter(1, _verbose, "%{element}::prepare_packet:type=%d, rate=%d len_broadcast=%d, len_unicast=%d, _hdr_size=%d, p_length=%d", this, rxv->type, rxv->rate, rxv->len_broadcast, rxv->len_unicast, hdr_size, p_out->length()); 

  if( pktlength != p_out->length()-hdr_size)
  {
    hydra_chatter(8, _verbose, "%{element}::drop_packet: rxv_len=%d, pkt_len=%d, hdr_size=%d", this, pktlength, p_out->length(), hdr_size);
    //assert( rxv->length == p_out->length() - _hdr_size)
    //printf("drop packet in rxvector\n");
    p_out->kill();
    return 0;
  }
  
  //memory allocation 
  cha->type = convert_type(rxv->type); 
  //printf("[rxv]cha->type=%d\n", cha->type);  
  if(cha->type==HYDRA_RX_DATA)
  { 
     agg_anno_allocator(p_out, rxv->len_metadata); 
     cha->agg_anno->len_metadata = rxv->len_metadata;
     cha->agg_anno->len_broadcast = rxv->len_broadcast;
     cha->agg_anno->len_unicast = rxv->len_unicast;
     cha->rate_mbps = convert_rate(rxv->rate); 
     cha->avg_snr = (double)rxv->snr;
     //printf("[rxv]len_meta=%d, len_unicast=%d\n", cha->agg_anno->len_metadata, cha->agg_anno->len_unicast); 
  
     if(rxv->len_metadata) 
        memcpy(cha->agg_anno->metadata, rxv->metadata, rxv->len_metadata);

     //printf("[rxv]metadata\n");
     //for(int n=0; n<rxv->len_metadata; n++)
     //   printf("%d",cha->agg_anno->metadata[n]);
     //printf("\n");
  
     // for memory free
     p_out->set_hydra_destructor(agg_anno_destructor);
  }
  // FIXME: change belows according to the drexel implementation.
/*
  cha->channel_vector.clear();
  struct click_hydra_channel chc;
  chc.real = 0.1; 
  chc.imaginary = 0.2;  
  cha->channel_vector.push_back(chc);
  //hydra_chatter(8, _verbose, "%{element}::prepare_packet: real=%f imag=%f", this, chc.real, chc.imaginary);
*/
/*
  if(rxv->info_size > MAX_CHANNEL_INFO)
  {
    p_out->pull(_hdr_size);
    return p_out;
  }
  
  for(int i=0; i<rxv->info_size; i++)
  {
    cha->channel_vector[i].real = rxv->ch_info[i].real;
    cha->channel_vector[i].imaginary = rxv->ch_info[i].imaginary;
  }
*/
  //printf("hdr_size=%d\n", hdr_size);
  p_out->pull(hdr_size);
  //printf("send up the frame in rxvector\n");
  return p_out;
}

uint16_t
RXVectorGrNew::convert_rate(uint32_t rxv_rate)
{
  hydra_chatter(8, _verbose, "%{element}::convert_rate:", this);
  uint16_t anno_rate = 0;

  switch(rxv_rate)
  {
    case MPIRATE_6M: anno_rate = 6; break;
    case MPIRATE_9M: anno_rate = 9; break;
    case MPIRATE_12M: anno_rate = 12; break;
    case MPIRATE_18M: anno_rate = 18; break;
    case MPIRATE_24M: anno_rate = 24; break;
    case MPIRATE_36M: anno_rate = 36; break;
    case MPIRATE_48M: anno_rate = 48; break;
    case MPIRATE_54M: anno_rate = 54; break;
    case MPIRATE_13M: anno_rate = 13; break;
    case MPIRATE_26M: anno_rate = 26; break;
    case MPIRATE_39M: anno_rate = 39; break;
    case MPIRATE_52M: anno_rate = 52; break;
    case MPIRATE_78M: anno_rate = 78; break;
    case MPIRATE_104M: anno_rate = 104; break;
    case MPIRATE_117M: anno_rate = 117; break;
    case MPIRATE_130M: anno_rate = 130; break;
  }
  return anno_rate;
}

uint8_t
RXVectorGrNew::convert_type(uint32_t rxv_type)
{
  hydra_chatter(8, _verbose, "%{element}::convert_type:", this);
  uint8_t anno_type = 0;
  switch(rxv_type)
  {
    case MPIMSG_RX_DATA: 
    {
      hydra_chatter(9, _verbose, "%{element}::convert_type: RX_DATA", this);
      anno_type = HYDRA_RX_DATA; 
      break;
    }
    case MPIMSG_CS_IDLE: anno_type = HYDRA_CCA_IDLE; break;
    case MPIMSG_CS_BUSY: anno_type = HYDRA_CCA_BUSY; break;
    default:
      hydra_chatter(0, _verbose, "%{element}::convert_type: never this case", this);
      break;
  }

  return anno_type;
}

String
RXVectorGrNew::read_param(Element *e, void *vparam)
{
  RXVectorGrNew *mf = (RXVectorGrNew *)e;
  switch( (intptr_t)vparam )
  {
    case 0:
      return String(mf->_verbose);
    default:
      return String();
  }
  return String();
}

int
RXVectorGrNew::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  RXVectorGrNew *mf = (RXVectorGrNew *)e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case 0:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      mf->_verbose = verbose;
      break; 
    }
  }

  return 0;
}

void
RXVectorGrNew::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RXVectorGrNew)
