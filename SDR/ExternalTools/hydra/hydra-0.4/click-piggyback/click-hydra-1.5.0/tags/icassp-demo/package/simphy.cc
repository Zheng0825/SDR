/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/simphy.cc,v $ 
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/packet_anno.hh>

#include <clicknet/mpi.h>
#include <click/hydra_anno.hh>

#include "simphy.hh"
#include "simchannel.hh"

CLICK_DECLS

SimPhy::SimPhy()
  : _timer(this), _state(PHY_IDLE),
    _tx_wait(10000), _rx_wait(10000), _rcv_buf(0)
{
}

SimPhy::~SimPhy()
{
}

int
SimPhy::configure(Vector<String> &conf, ErrorHandler *errh)
{

  int res =  cp_va_parse(conf, this, errh,
                     cpOptional,
		     cpKeywords,
		     "TXWAIT", cpSecondsAsMicro, "busy waiting time for tx (us)", &_tx_wait,
		     "RXWAIT", cpSecondsAsMicro, "busy waiting time for rx (us)", &_rx_wait,
                     cpEnd);

  if( res < 0)
    return -1;

  _tx_tv.tv_sec = _tx_wait/1000000;
  _tx_tv.tv_usec = _tx_wait%1000000;
  

  _rx_tv.tv_sec = _rx_wait/1000000;
  _rx_tv.tv_usec = _rx_wait%1000000;

   click_chatter("TXWAIT = %d.%06d RX_WAIT = %d.%06d", 
                  _tx_tv.tv_sec, _tx_tv.tv_usec, 
                  _rx_tv.tv_sec, _rx_tv.tv_usec);
  return 0;
}

int
SimPhy::initialize(ErrorHandler*)
{
  _timer.initialize(this);

  _state = PHY_RECEIVE;
  _timer.schedule_after(_rx_tv);

/*
  uint32_t time1 = 1000001;
  uint32_t time2 = 1999999;

  struct timeval t1, t2;
  t1.tv_sec = time1/1000000;
  t1.tv_usec = time1%1000000;

  t2.tv_sec = time2/1000000;
  t2.tv_usec = time2%1000000;;

  if(t1 > t2)
    click_chatter("T1 is longer t1 = %d.%06d t2 = %d.%06d", t1.tv_sec, t1.tv_usec, t2.tv_sec, t2.tv_usec);
  else if(t2 > t1)
    click_chatter("T2 is longer t1 = %d.%06d t2 = %d.%06d", t1.tv_sec, t1.tv_usec, t2.tv_sec, t2.tv_usec);
  else
    click_chatter("T1 T2 same   t1 = %d.%06d t2 = %d.%06d", t1.tv_sec, t1.tv_usec, t2.tv_sec, t2.tv_usec);
*/ 
  return 0;
}

void
SimPhy::push(int port, Packet *p) 
{
  if(port == PORT_MAC)
    process_mac(p);
  else if(port == PORT_CHANNEL)
    process_channel(p);

}

void
SimPhy::run_timer(Timer* )
{
  switch(_state) // if state was _state
  {
    case PHY_TRANSMIT:
    {
      _state = PHY_RECEIVE;

      if(_rcv_buf)
      {
        push_type(PORT_MAC, MPITYPE_RX_START);
        push_type(PORT_MAC, MPITYPE_CCA_BUSY);
        if(PAINT_ANNO(_rcv_buf) == SimChannel::RECV_DATA) 
        {
          click_chatter("%{element}:process_channel: in TX_range", this);
          push_packet(PORT_MAC, _rcv_buf);
        }
        push_type(PORT_MAC, MPITYPE_CCA_IDLE);

        _rcv_buf = 0;

        _state = PHY_TRANSMIT;
        push_type(PORT_MAC, MPITYPE_TX_REQ);
        _timer.schedule_after(_tx_tv);
      }
      else
      {
        _timer.schedule_after(_rx_tv);
      }
      break;
    }
    case PHY_RECEIVE:
    {
      _state = PHY_TRANSMIT;
      push_type(PORT_MAC, MPITYPE_TX_REQ);
      _timer.schedule_after(_tx_tv);
      break;
    }
    default:
    {
      click_chatter("%{element}::run-timer: never this state = %d", this, _state);
      break;
    }
  }
  
}

void
SimPhy::process_mac(Packet* p)
{
  assert(p);

  if(_state != PHY_TRANSMIT)
  {
    click_chatter("%{element}::process_mac: current is not TX state", this);
    p->kill();
    return; 
  }

  struct mpitype* mt = (struct mpitype *) p->data();  
  switch(mt->type)
  {
    // FIXME: which one first, push or run_timer
    case MPITYPE_TX_DATA:
    {
      WritablePacket* p_out = p->uniqueify();
      struct mpitype* mtype = (struct mpitype *) p_out->data();
      mtype->type = MPITYPE_RX_DATA;
      output(PORT_CHANNEL).push(p_out);

      push_type(PORT_MAC, MPITYPE_TX_DONE);

      _timer.unschedule();
      run_timer(&_timer);
      return;
    }
    case MPITYPE_TX_NODATA:
    {
      _timer.unschedule();
      run_timer(&_timer);
      break;
    }
    default:
    {
      click_chatter("%{element}::process_mac: never this type = %d", this, mt->type);
      break;
    }
  }

  p->kill();
  return;

}

void
SimPhy::process_channel(Packet* p)
{
  assert(p);

  struct mpitype* mt = (struct mpitype *) p->data();  
  switch(mt->type)
  {
    // FIXME: which one first, push or run_timer
    case MPITYPE_RX_DATA:
    {
      if(_state == PHY_RECEIVE)
      {
        push_type(PORT_MAC, MPITYPE_RX_START);
        push_type(PORT_MAC, MPITYPE_CCA_BUSY);
        if(PAINT_ANNO(p) == SimChannel::RECV_DATA) 
        {
          click_chatter("%{element}:process_channel: in TX_range", this);
          push_packet(PORT_MAC, p);
        }
        push_type(PORT_MAC, MPITYPE_CCA_IDLE);

        _timer.unschedule();
        run_timer(&_timer);
      }
      else //if(PHY_TRANSMIT)
      {
        if(!_rcv_buf) 
          _rcv_buf = p; 
      }
      break;
    }
    default:
      click_chatter("%{element}::process_channel: never this type = %d", this, mt->type);
      break;
  }

  return;

}

void
SimPhy::push_type(int port, uint8_t type)
{
  struct rxvector rv;

  rv.type = type;
  rv.rate = 0;
  rv.length = 0;
  rv.service = 0;
  // FIXME: what is the rssi for this case?
  rv.rssi = 0;
  rv.avg_snr = 10000; // -10db

  WritablePacket* p_out = Packet::make(4, (unsigned char *)&rv, sizeof(struct rxvector), 4);

  if(!p_out)
  {
    click_chatter("%{element}::push_type fail to make new packet", this);
    return;
  }

  output(port).push(p_out);

}


void
SimPhy::push_packet(int port, Packet* p)
{
  struct txvector* tv = (struct txvector *) p->data();
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
CLICK_ENDDECLS
EXPORT_ELEMENT(SimPhy)
