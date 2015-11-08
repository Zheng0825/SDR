/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 * 
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
 * 
 ***********************************************************/
 

#include <click/config.h>
#include <click/glue.hh>
#include <click/error.hh>
#include <click/confparse.hh>
#include <click/packet_anno.hh>

#include <clicknet/mpi.h>
#include <click/hydra_anno.hh>

#include "simgrphy.hh"
#include "simchannel.hh"

CLICK_DECLS

SimGrPhy::SimGrPhy()
  : _timer(this), _state(PHY_IDLE), _pkt_buf(0), 
    _phy_time(0), _verbose(0)
{
  //hydra_chatter(9, 9, "%{element}::constructor", this);
}

SimGrPhy::~SimGrPhy()
{
}

int SimGrPhy::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* phy_time_e = 0;
  int res =  cp_va_parse(conf, this, errh,
		     cpElement, "PHYProcessTime element", &phy_time_e,
                     cpOptional,
		     cpKeywords,
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
SimGrPhy::initialize(ErrorHandler*)
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
SimGrPhy::push(int port, Packet *p) 
{
  hydra_chatter(9, _verbose, "%{element}::push", this);

  if(port == PORT_MAC)
    process_mac(p);
  else if(port == PORT_CHANNEL)
    process_channel(p);

}


void
SimGrPhy::start_tx_timer(Packet* pkt)
{
  int tx_len = pkt->length() - sizeof(struct gr_txvector);
  unsigned tx_time = _phy_time->tx_encoding(tx_len);
  struct timeval tx_tv;

  tx_tv.tv_sec = tx_time/1000000;
  tx_tv.tv_usec = tx_time%1000000;

  hydra_chatter(9, _verbose, "%{element}::start_tx_timer delay = %d.%06d", this, tx_tv.tv_sec, tx_tv.tv_usec);

  _timer.schedule_after(tx_tv);
}

void
SimGrPhy::start_rx_timer(Packet* pkt)
{

  int rx_len = pkt->length() - sizeof(struct gr_rxvector);
  unsigned rx_time = _phy_time->rx_decoding(rx_len);
  struct timeval rx_tv;

  rx_tv.tv_sec = rx_time/1000000;
  rx_tv.tv_usec = rx_time%1000000;

  hydra_chatter(9, _verbose, "%{element}::start_rx_timer delay = %d.%06d", this, rx_tv.tv_sec, rx_tv.tv_usec);
  _timer.schedule_after(rx_tv);
}

void SimGrPhy::process_mac(Packet* p)
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
      struct mpitype* mtype = (struct mpitype *) p_out->data();
      mtype->type = MPIMSG_RX_DATA;

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
SimGrPhy::process_channel(Packet* p)
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

void SimGrPhy::run_timer(Timer* )
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

void SimGrPhy::push_type(int port, uint8_t type)
{
  struct gr_rxvector rv;
  printf("push_type=%d\n", type);
  rv.type = type;
  rv.length = 0;
  rv.rate = 0;
  rv.snr = 0.0;

  WritablePacket* p_out = Packet::make(4, (unsigned char *)&rv, sizeof(struct rxvector), 4);

  if(!p_out)
  {
    hydra_chatter(0, _verbose, "%{element}::push_type fail to make new packet", this);
    return;
  }

  output(port).push(p_out);
}

void SimGrPhy::push_wrong_packet(int port, uint8_t type)
{
  struct gr_rxvector rv;
  int wrong_packet_size = 5;

  rv.type = type;
  rv.length = wrong_packet_size;
  rv.rate = 0;
  rv.snr = 0.0;

  WritablePacket* p_out = Packet::make(4, (unsigned char *)&rv, sizeof(struct rxvector), wrong_packet_size + 4);

  if(!p_out)
  {
    hydra_chatter(0, _verbose, "%{element}::push_type fail to make new packet", this);
    return;
  }

  p_out = p_out->put(wrong_packet_size);
  memset((void *) (p_out->data() + sizeof(struct rxvector)), 0x00, wrong_packet_size);

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
EXPORT_ELEMENT(SimGrPhy)
