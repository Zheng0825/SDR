/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/mpi.h>
#include "txvectorgrnew.hh"
#include "macframe80211.hh"
#include "setdefer.hh"

CLICK_DECLS

TXVectorGrNew::TXVectorGrNew()
  : _hdr_size(sizeof(struct gr_txvector_new)), _logfilename("stdout"), _fptr_log(stdout), _mac_rts(0), _mac_cts(0), _mac_data(0), _mac_ack(0), _sifs(0), _difs(0), _seq_num(0) 
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

TXVectorGrNew::~TXVectorGrNew()
{
}

int
TXVectorGrNew::configure(Vector<String> &conf, ErrorHandler *errh)
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
TXVectorGrNew::initialize(ErrorHandler *errh)
{
  _mac_data_hdr_size = _mac_data->header_size();

  hydra_chatter(9, _verbose, "%{element}::configure output file name %s\n", this, _logfilename.c_str());
  if (_logfilename != "stdout") 
  {
    _fptr_log = fopen(_logfilename.c_str(), "wt");
    if (!_fptr_log)
      return errh->error("%s: %s", _logfilename.c_str(), strerror(errno));
  } 
  
  printf("TXVectorGrNew: Opened log file %s\n", _logfilename.c_str());
  return 0;
}

void*
TXVectorGrNew::cast(const char *n)
{
    if (strcmp(n, "TXVectorGrNew") == 0)
	return (TXVectorGrNew *) this;
    
    return MPIVector::cast(n);
}

Packet *
TXVectorGrNew::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);
  //struct click_hydra_anno* debug = (struct click_hydra_anno*) p->all_user_anno();
  //printf("txvector start: 0x%x\n", debug->mac_type);

  if(!p || !p->length())
  {
    hydra_chatter(0, _verbose, "%{element}::simple_action: wrong packet", this);
    if(p) p->kill();
    return 0;
  }

  //WritablePacket* p_out = prepare_packet(p);
  Packet* p_out = prepare_packet(p);
  // never use manipulate or free a packet after calling its uniqueify 
  //p->kill();

  //printf("txvector end: 0x%x\n", debug->mac_type);

  return p_out;
}

//WritablePacket*
Packet*
TXVectorGrNew::prepare_packet(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::prepare_packet:", this);

  //WritablePacket* p_out = p->uniqueify();
  Packet* p_out = p;  
  hdr_size=0;
  int num_uni_data=0; 
  int num_broad_data=0; 
  struct click_hydra_anno* cha = 
		(struct click_hydra_anno *) p_out->all_user_anno();
  if(cha->agg_anno)
  {   
     struct click_agg_anno* agg=cha->agg_anno;
     hdr_size=_hdr_size-1+agg->len_metadata; 
     p_out->push(hdr_size);
     
     struct gr_txvector_new* txv = (struct gr_txvector_new *) p_out->data();
     txv->len_metadata  = agg->len_metadata;
     // FIXME: -2 +2 
     //txv->len_broadcast = 0;  //CSI piggyback
     //txv->len_unicast=p_out->length()-hdr_size; //CSI piggyback
     txv->len_unicast   = agg->len_unicast;
     txv->len_broadcast = agg->len_broadcast;
     memcpy(txv->metadata, agg->metadata, txv->len_metadata);
     //txv->type = convert_type(cha->type); //nothing to be set
     txv->type = convert_type(HYDRA_TX_DATA);
     txv->rate = convert_rate(cha->rate_mbps); 
     txv->txpwr = (float) cha->power;
    
     //printf("txv->txpwr=%f\n", txv->txpwr);
     // To calculate total header overhead
     num_uni_data=agg->num_unicast;
     num_broad_data=agg->num_broadcast;
     hydra_chatter(1, _verbose, "%{element}::prepare_packet:hdr_size %d type %d rate %d len_unicast %d len_broadcast %d len_metadata %d p_length %d", this, hdr_size, txv->type, txv->rate, txv->len_unicast, txv->len_broadcast, txv->len_metadata, p_out->length());
  
  }
  else
  {   
     hdr_size=_hdr_size-1;
     p_out->push(hdr_size);
     struct gr_txvector_nometa* txv = (struct gr_txvector_nometa *) p_out->data();
     txv->len_metadata=0;
     // FIXME: unicast data when no annotation? 
     txv->len_unicast=p_out->length()-hdr_size;
     txv->len_broadcast=0;
     txv->type = convert_type(HYDRA_TX_DATA);
     txv->rate = convert_rate(cha->rate_mbps); 
     txv->txpwr = (float) cha->power;
     
     // To calculate total header overhead: no aggregation case
     num_uni_data=1;
     num_broad_data=0;
     hydra_chatter(1, _verbose, "%{element}::prepare_packet(no annotation):hdr_size %d type %d len_unicast %d len_broadcast %d len_metadata %d p_length %d", this, hdr_size, txv->type, txv->len_unicast, txv->len_broadcast, txv->len_metadata, p_out->length());
     //printf("txv no meta:len_broadcast %d, len_unicast %d, pkt_len %d\n", txv->len_broadcast, txv->len_unicast, p_out->length());
  }

  // write log file to measure throughput
  struct fc_80211* header = (struct fc_80211*)(p_out->data()+hdr_size);
  
  if(is_rts(*header))
    process_rts(p_out, hdr_size);
  else if(is_cts(*header))
    process_cts(p_out, hdr_size);
  else if(is_data(*header))
    process_data(p_out, hdr_size, num_uni_data, num_broad_data);
  else if(is_ack(*header))
    process_ack(p_out, hdr_size);

  return p_out;
}

void TXVectorGrNew::process_rts(Packet* pkt, int hdrsize)
{
  struct timeval now;
  click_gettimeofday(&now);
 
  Packet* cpkt = pkt->clone();
  cpkt->pull(hdrsize);
  
  struct click_hydra_anno* cha = (struct click_hydra_anno *) cpkt->all_user_anno();
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

  char defer_type[5];
  int defer_type_int = 0;
  int slot_size = 0;
  if(cha->defer_usec == _difs->real_defer(cpkt->length()))
  {
    memcpy(defer_type, "difs\0", 5);
    defer_type_int = 1;
    slot_size = cha->slot/_difs->slot_time();
  }
  else
  {
    memcpy(defer_type, "sifs\0", 5);
  }

  printf("TX RTS seqn %03d rate %02.01f txpower %04d\n", 0, drate, cha->power);
  fprintf(_fptr_log, 
     "%d.%06d node %d TX RTS seqn %03d src %02d dst %02d hlen %02d bhlen 00 dlen 0000 blen 0000 rate %02.01f txpower %04d %s %d slot_num %04d\n",
     now.tv_sec, now.tv_usec, 
     _my_mac_addr.data()[5], 0, 
     _mac_rts->src(cpkt)[5], _mac_rts->dst(cpkt)[5], 
     cpkt->length(), drate, cha->power,
     defer_type, defer_type_int, slot_size);
}

void TXVectorGrNew::process_cts(Packet* pkt, int hdrsize)
{
  struct timeval now;
  click_gettimeofday(&now);

  Packet* cpkt = pkt->clone();
  cpkt->pull(hdrsize);
  
  struct click_hydra_anno* cha = (struct click_hydra_anno *) cpkt->all_user_anno();
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

  char defer_type[5];
  int defer_type_int = 0;
  int slot_size = 0;
  if(cha->defer_usec == _difs->real_defer(cpkt->length()))
  {
    memcpy(defer_type, "difs\0", 5);
    defer_type_int = 1;
    slot_size = cha->slot/_difs->slot_time();
  }
  else
  {
    memcpy(defer_type, "sifs\0", 5);
  }

  printf("TX CTS seqn %03d rate %02.01f txpower %04d\n", 0, drate, cha->power);
  fprintf(_fptr_log, 
     "%d.%06d node %d TX CTS seqn %03d src 99 dst %02d hlen %02d bhlen 00 dlen 0000 blen 0000 rate %02.01f txpower %04d %s %d slot_num %04d\n",
     now.tv_sec, now.tv_usec, 
     _my_mac_addr.data()[5], 0, 
     _mac_cts->dst(cpkt)[5], 
     cpkt->length(), drate, cha->power, 
     defer_type, defer_type_int, slot_size);

}

void TXVectorGrNew::process_data(Packet* pkt, int hdrsize, int num_uni_data, int num_broad_data)
{
 
  struct gr_txvector_new* txv = (struct gr_txvector_new *) pkt->data();
  int payload_len = txv->len_broadcast+txv->len_unicast;

  struct timeval now;
  click_gettimeofday(&now);
  
  Packet* cpkt = pkt->clone();
  cpkt->pull(hdrsize);
  int hdr_length = num_uni_data*_mac_data_hdr_size; 
  int broad_hdr_length = num_broad_data*_mac_data_hdr_size; 

  struct click_hydra_anno* cha = (struct click_hydra_anno *) cpkt->all_user_anno();
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

  char defer_type[5];
  int defer_type_int = 0;
  int slot_size = 0;
  if(cha->defer_usec == _difs->real_defer(cpkt->length()))
  {
    memcpy(defer_type, "difs\0", 5);
    defer_type_int = 1;
    slot_size = cha->slot/_difs->slot_time();
  }
  else
  {
    memcpy(defer_type, "sifs\0", 5);
  }

  //printf("TX DTA seqn %03d rate %02d txpower %04d\n", 0, cha->rate_mbps, cha->power);
  printf("TX DTA seqn %03d rate %02.01f txpower %04d\n", 0, drate, cha->power);
  fprintf(_fptr_log, 
     "%d.%06d node %d TX DTA seqn %03d src %02d dst %02d hlen %02d bhlen %02d dlen %04d blen %04d rate %02.01f txpower %04d %s %d slot_num %04d\n",
     now.tv_sec, now.tv_usec,
     _my_mac_addr.data()[5], 0, 
     _mac_data->src(cpkt)[5], _mac_data->dst(cpkt)[5], 
     hdr_length, broad_hdr_length, payload_len, txv->len_broadcast,
     drate, cha->power, 
     defer_type, defer_type_int, slot_size);
}

void TXVectorGrNew::process_ack(Packet* pkt, int hdrsize)
{
  struct timeval now;
  click_gettimeofday(&now);
  
  Packet* cpkt = pkt->clone();
  cpkt->pull(hdrsize);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) cpkt->all_user_anno();
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

  char defer_type[5];
  int defer_type_int = 0;
  int slot_size = 0;
  if(cha->defer_usec == _difs->real_defer(cpkt->length()))
  {
    memcpy(defer_type, "difs\0", 5);
    defer_type_int = 1;
    slot_size = cha->slot/_difs->slot_time();
  }
  else
  {
    memcpy(defer_type, "sifs\0", 5);
  }

  printf("TX ACK seqn %03d rate %02.01f txpower %04d\n", 0, drate, cha->power);
  fprintf(_fptr_log, 
      "%d.%06d node %d TX ACK seqn %03d src 99 dst %02d hlen %02d bhlen 00 dlen 0000 blen 0000 rate %02.01f txpower %04d %s %d slot_num %04d\n",
      now.tv_sec, now.tv_usec,
      _my_mac_addr.data()[5], 0, 
      _mac_ack->dst(cpkt)[5], 
      cpkt->length(), drate, cha->power,
     defer_type, defer_type_int, slot_size);

}

uint32_t
TXVectorGrNew::convert_rate(uint16_t anno_rate)
{
  uint16_t rate = 0;

  switch(anno_rate)
  {
    case 6: rate = MPIRATE_6M; break;
    case 9: rate = MPIRATE_9M; break;
    case 12: rate = MPIRATE_12M; break;
    case 18: rate = MPIRATE_18M; break;
    case 24: rate = MPIRATE_24M; break;
    case 36: rate = MPIRATE_36M; break;
    case 48: rate = MPIRATE_48M; break;
    case 54: rate = MPIRATE_54M; break;
    case 13: rate = MPIRATE_13M; break;
    case 26: rate = MPIRATE_26M; break;
    case 39: rate = MPIRATE_39M; break;
    case 52: rate = MPIRATE_52M; break;
    case 78: rate = MPIRATE_78M; break;
    case 104: rate = MPIRATE_104M; break;
    case 117: rate = MPIRATE_117M; break;
    case 130: rate = MPIRATE_130M; break;
  }
  return rate;
}

uint32_t
TXVectorGrNew::convert_type(uint8_t anno_type)
{
  uint8_t txv_type = 0;
  switch(anno_type)
  {
    // FIXME: some element must change RX_DATA to TX_DATA
    case HYDRA_RX_DATA: 
    case HYDRA_TX_DATA: 
	txv_type = MPIMSG_TX_DATA; 
	break;
    default:
      hydra_chatter(0, _verbose, "%{element}::convert_type: never this case %d", this, anno_type);
      break;
  }
  return txv_type;
}

String
TXVectorGrNew::read_param(Element *e, void *vparam)
{
  TXVectorGrNew *mf = (TXVectorGrNew *)e;
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
TXVectorGrNew::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  TXVectorGrNew *mf = (TXVectorGrNew *)e;
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
TXVectorGrNew::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TXVectorGrNew)
