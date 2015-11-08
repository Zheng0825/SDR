/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 

#include <click/config.h>
#include <click/glue.hh>
#include <click/error.hh>
#include <click/confparse.hh>
#include <click/packet_anno.hh>

#include <clicknet/mpi.h>
#include <clicknet/80211.h>
#include <click/hydra_anno.hh>

#include "simgrphynew.hh"
#include "simchannel.hh"

CLICK_DECLS

SimGrPhyNew::SimGrPhyNew()
  : _timer(this), _state(PHY_IDLE), _pkt_buf(0), 
    _phy_time(0), _verbose(0), _len_csi(0)
{
  //hydra_chatter(9, 9, "%{element}::constructor", this);
}

SimGrPhyNew::~SimGrPhyNew()
{
}

int SimGrPhyNew::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* phy_time_e = 0;
  int res =  cp_va_parse(conf, this, errh,
		     cpElement, "PHYProcessTime element", &phy_time_e,
                     cpOptional,
		     cpKeywords,
		     "CSISIZE", cpUnsigned, "csi_size", &_len_csi, 
 		     "VERBOSE", cpUnsigned, "verbose", &_verbose,
                     cpEnd);

  hydra_chatter(9, _verbose, "%{element}::configure", this);

  if( res < 0)
    return -1;

  if(phy_time_e) 
  {
    _phy_time = (PHYProcessTime *) phy_time_e->cast("PHYProcessTime");
    if(!_phy_time) 
      return errh->error("need PHYProcessTime element");
  } 

  return 0;
}

int
SimGrPhyNew::initialize(ErrorHandler*)
{
  hydra_chatter(9, _verbose, "%{element}::initialize", this);
  _timer.initialize(this);

/*
  _state = PHY_RECEIVE;
  _timer.schedule_after(_rx_tv);
*/
  return 0;
}

void
SimGrPhyNew::push(int port, Packet *p) 
{
  hydra_chatter(1, _verbose, "%{element}::push", this);
  if(port == PORT_MAC)
  {
    process_mac(p); 
    //printf("process_mac\n");
  }
  else if(port == PORT_CHANNEL)
  {  
    process_channel(p); 
    //printf("process_phy\n");
  }

}

void
SimGrPhyNew::start_tx_timer(Packet* pkt)
{
  struct gr_txvector_new* txv=(struct gr_txvector_new *) pkt->data();
  uint32_t len_metadata=txv->len_metadata;
  int tx_len = pkt->length()-(sizeof(struct gr_txvector_new)-1+len_metadata);
  //unsigned tx_time = _phy_time->tx_encoding(tx_len);
  unsigned tx_time = _phy_time->tx_delay();
  struct timeval tx_tv;

  tx_tv.tv_sec = tx_time/1000000;
  tx_tv.tv_usec = tx_time%1000000;

  hydra_chatter(9, _verbose, "%{element}::start_tx_timer delay = %d.%06d", this, tx_tv.tv_sec, tx_tv.tv_usec);

  _timer.schedule_after(tx_tv);
}

void
SimGrPhyNew::start_rx_timer(Packet* pkt)
{
  struct gr_rxvector_new* rxv=(struct gr_rxvector_new *) pkt->data();
  uint32_t len_metadata=rxv->len_metadata;

  int rx_len =pkt->length()-(sizeof(struct gr_rxvector_new)-1+len_metadata);
  //unsigned rx_time = _phy_time->rx_decoding(rx_len);
  unsigned rx_time = _phy_time->rx_delay();
  struct timeval rx_tv;

  rx_tv.tv_sec = rx_time/1000000;
  rx_tv.tv_usec = rx_time%1000000;

  hydra_chatter(9, _verbose, "%{element}::start_rx_timer delay = %d.%06d", this, rx_tv.tv_sec, rx_tv.tv_usec);
  _timer.schedule_after(rx_tv);
}

void SimGrPhyNew::process_mac(Packet* p)
{

  if(_state != PHY_IDLE)
  {
    hydra_chatter(5, _verbose, "%{element}::process_mac: current is not IDLE state", this);
    if(_state == PHY_TRANSMIT)
    {
      hydra_chatter(0, _verbose, "%{element}::process_mac: duplicated TX", this);
      p->kill();
      assert(false);
    }
  
    //else if(_state == PHY_RECEIVE || _state == PHY_CS)  
    //now transmission has higher priority
    // if(_timer.scheduled())
    {
      _timer.unschedule();
      _pkt_buf->kill();
      _pkt_buf = 0;
    }
    
  }
 
  struct mpitype* mt = (struct mpitype *) p->data();  
  switch(mt->type)
  {
    case MPIMSG_TX_DATA:
    {
      WritablePacket* p_out = p->uniqueify();
      
      //FOR debugging
      //struct fc_80211* fc_header= (struct fc_80211 *) (p_out->data()+24);
      //printf("subtype=%d, type=%d\n", fc_header->subtype, fc_header->type);
      //mtype->type = MPIMSG_RX_DATA;
      struct gr_txvector_new* txv = (struct gr_txvector_new *) p_out->data();
     
      // set the size of CSI information here 
      uint32_t len_metadata=(uint32_t) _len_csi;
      uint32_t type= MPIMSG_RX_DATA;
      uint32_t len_broadcast = txv->len_broadcast;
      uint32_t len_unicast = txv->len_unicast;
      uint32_t rate = txv->rate;
      float txpwr = txv->txpwr;
      char  metadata[len_metadata];

      for(int n=0; n<len_metadata; n++)
	  metadata[n]=0x07;
     
      hydra_chatter(1, _verbose, "%{element}::len_meta %d len_broadcast %d len_unicast %d", this, len_metadata, len_broadcast, len_unicast);
      
      //txv->type = MPIMSG_RX_DATA;
      if(len_metadata >= txv->len_metadata) 
         p_out->push(len_metadata-txv->len_metadata);
      else
	 p_out->pull(txv->len_metadata-len_metadata); 
      // Add pseudo metadata in the packet 
      struct gr_txvector_new* txvec = (struct gr_txvector_new *) p_out->data();
      
      txvec->type = type;
      txvec->len_metadata = len_metadata;
      txvec->len_broadcast = len_broadcast;
      txvec->len_unicast = len_unicast;
      txvec->rate = rate;
      txvec->txpwr = txpwr;
      for(int n=0; n< txvec->len_metadata; n++)
	 txvec->metadata[n]=metadata[n]; 
      
      //printf("type=%d, len_meta=%d, len_broadcast=%d, len_unicast=%d, metadata=%d\n", *(int *)p->data(), *(int *)(p->data()+4), *(int *)(p->data()+8), *(int *)(p->data()+12), *(char *)(p->data()+24));   
      
      //fc_header= (struct fc_80211 *) (p_out->data()+25);
      //printf("revised: subtype=%d, type=%d\n", fc_header->subtype, fc_header->type);
      _pkt_buf = p_out;
      _state = PHY_TRANSMIT;
      start_tx_timer(_pkt_buf);   
      break;
    }
    default:
    {
      hydra_chatter(0, _verbose, "%{element}::process_mac: never this type = %d", this, mt->type);
      p->kill();
      break;
    }
  }

  return;

}

void
SimGrPhyNew::process_channel(Packet* p)
{

  if(_state != PHY_IDLE)
  {
    hydra_chatter(5, _verbose, "%{element}::process_channel: current is not IDLE state", this);
  
    //if(_state == PHY_RECEIVE || _state == PHY_TRANSMIT)  
    p->kill();
    return;
  }

  struct mpitype* mt = (struct mpitype *) p->data();  
  switch(mt->type)
  {
    case MPIMSG_RX_DATA:
    {
      push_type(PORT_MAC, MPIMSG_CS_BUSY);

      _pkt_buf = p;
      _state = PHY_RECEIVE;
      start_rx_timer(_pkt_buf);   
      break;
    }
    default:
    {
      hydra_chatter(0, _verbose, "%{element}::process_channel: never this type = %d", this, mt->type);
      break;
    }
  }

  return;
}

void SimGrPhyNew::run_timer(Timer* )
{
  assert(_pkt_buf);

  switch(_state) // if state was _state
  {
    case PHY_TRANSMIT:
    {
      output(PORT_CHANNEL).push(_pkt_buf);
      _pkt_buf = 0;
      _state = PHY_IDLE;

      push_type(PORT_MAC, MPIMSG_TX_DONE);
      break;
    }
    case PHY_RECEIVE:
    {
      if(PAINT_ANNO(_pkt_buf) == SimChannel::RECV_DATA)
      {
        output(PORT_MAC).push(_pkt_buf);
      }
      else //if(PAINT_ANNO(_pkt_buf) == SimChannel::RECV_CS)
      {
        _pkt_buf->kill();
        push_wrong_packet(PORT_MAC, MPIMSG_RX_DATA);
      } 

      _pkt_buf = 0;
      _state = PHY_IDLE;
      break;
    }
    default:
    {
      hydra_chatter(0, _verbose, "%{element}::run-timer: never this state = %d", this, _state);
      break;
    }
  }

  return;
}

void SimGrPhyNew::push_type(int port, uint32_t type)
{
  struct gr_rxvector_new rv;
  //printf("type=%d\n", type);
  rv.type = type;
  rv.len_metadata=0;
  rv.len_broadcast=0;
  rv.len_unicast=0;
  rv.rate = 0;
  rv.snr = 0.0;

  WritablePacket* p_out = Packet::make(4, (unsigned char *)&rv, sizeof(struct gr_rxvector_new)-1, 4);

  if(!p_out)
  {
    hydra_chatter(0, _verbose, "%{element}::push_type fail to make new packet", this);
    return;
  }
  
  //struct timeval now;
  //click_gettimeofday(&now);
  //printf("Type=[%d]:tx_time = %d.%06d\n", type, now.tv_sec, now.tv_usec);

  output(port).push(p_out);
}

void SimGrPhyNew::push_wrong_packet(int port, uint32_t type)
{
  struct fake_rxvector rv;
  int wrong_packet_size = 5;

  rv.type = type;
  rv.len_metadata=10;
  rv.len_broadcast=0;
  rv.len_unicast=wrong_packet_size;
  rv.rate = 0;
  rv.snr = 0.0;
  
  for(int n=0;n<rv.len_metadata; n++)
     rv.metadata[n]=0x00;

  WritablePacket* p_out = Packet::make(4, (unsigned char *)&rv, sizeof(struct fake_rxvector), wrong_packet_size+4);

  if(!p_out)
  {
    hydra_chatter(0, _verbose, "%{element}::push_type fail to make new packet", this);
    return;
  }

  p_out = p_out->put(wrong_packet_size);
  memset((void *) (p_out->data()+sizeof(struct fake_rxvector)), 0x00, wrong_packet_size);
  
  struct timeval now;
  click_gettimeofday(&now);
  //printf("wrong_packet:Type=[%d]:tx_time = %d.%06d\n", type, now.tv_sec, now.tv_usec);

  output(port).push(p_out);
}

/*
void
SimGrPhy::push_packet(int port, Packet* p)
{
  struct gr_txvector* tv = (struct gr_txvector *) p->data();
  u_int8_t tv_type = tv->type;

  unsigned tv_len = sizeof(struct txvector);
  unsigned rv_len = sizeof(struct rxvector);
  unsigned data_len = p->length() - tv_len;

  Packet* p_out = p->uniqueify();
  //WritablePacket* p_out = Packet::make( 4 + rv_len,
  //                                     (unsigned char *)(p->data() + tv_len), 
  //                                      data_len, 4);
  if(!p_out)
  {
    click_chatter("%{element}::push_packet fail to make new packet", this);
    return;
  }
 
  p_out->pull(tv_len); 
  p_out->push(rv_len);

  struct rxvector* rv = (struct rxvector *) p_out->data();
  rv->type = tv_type;
  rv->rate = 0;
  rv->length = data_len;
  rv->service = 0;
  rv->rssi = 0;
  // FIXME: calculate the SNR based on the distance
  rv->avg_snr = 10000; // -10db
  
  output(port).push(p_out);

}
*/

CLICK_ENDDECLS
EXPORT_ELEMENT(SimGrPhyNew)
