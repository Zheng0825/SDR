/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>

#include <clicknet/mpi.h>
#include "rxvectorgr.hh"

CLICK_DECLS

RXVectorGr::RXVectorGr() :_hdr_size( sizeof(struct gr_rxvector) )
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RXVectorGr::~RXVectorGr()
{
}

void*
RXVectorGr::cast(const char *n)
{
    if (strcmp(n, "RXVectorGr") == 0)
	return (RXVectorGr *) this;
    
    return MPIVector::cast(n);
}

int
RXVectorGr::configure(Vector<String> &conf, ErrorHandler *errh)
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
RXVectorGr::simple_action(Packet *p)
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

//WritablePacket*
Packet*
RXVectorGr::prepare_packet(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::prepare_packet:", this);
  //WritablePacket* p_out = p->uniqueify();
  Packet* p_out = p;
  
  struct gr_rxvector* rxv = (struct gr_rxvector *) p_out->data();

  struct click_hydra_anno* cha = 
		(struct click_hydra_anno *) p_out->all_user_anno();


  if( rxv->length != p_out->length() - _hdr_size)
  {
    hydra_chatter(8, _verbose, "%{element}::prepare_packet: rxv_len=%d, pkt_len=%d, hdr_size=%d", this, rxv->length, p_out->length(), _hdr_size);
    //assert( rxv->length == p_out->length() - _hdr_size)
    p_out->kill();
    return 0;
  }
   
  cha->type = convert_type(rxv->type); 
  cha->rate_mbps = convert_rate(rxv->rate); 
  //cha->rrate_mbps = convert_rate(rxv->rate); 
  cha->avg_snr = (double)rxv->snr;

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
  p_out->pull(_hdr_size);

  return p_out;
}

uint16_t
RXVectorGr::convert_rate(uint8_t rxv_rate)
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
  }
  return anno_rate;
}

uint8_t
RXVectorGr::convert_type(uint8_t rxv_type)
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
RXVectorGr::read_param(Element *e, void *vparam)
{
  RXVectorGr *mf = (RXVectorGr *)e;
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
RXVectorGr::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  RXVectorGr *mf = (RXVectorGr *)e;
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
RXVectorGr::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RXVectorGr)
