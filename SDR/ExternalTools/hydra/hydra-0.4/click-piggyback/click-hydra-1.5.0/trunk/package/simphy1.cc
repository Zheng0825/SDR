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
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/simphy1.cc,v $ 
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/packet_anno.hh>
#include <click/standard/scheduleinfo.hh>

#include <clicknet/mpi.h>
#include <click/hydra_anno.hh>

#include "simphy1.hh"
#include "simchannel.hh"

CLICK_DECLS

SimPhy1::SimPhy1()
  : _task(this), _timer(this), _timeout(0),
    _waiting_packet(0)
{
}

SimPhy1::~SimPhy1()
{
}

int
SimPhy1::configure(Vector<String> &conf, ErrorHandler *errh)
{

  int res =  cp_va_parse(conf, this, errh,
		     cpSecondsAsMicro, "timeout for retx (us)", &_timeout,
                     cpOptional,
                     cpEnd);
  if( res < 0)
    return -1;

  if(!_timeout)
    _timeout = 100000; // 100ms

  click_chatter("%{element}::configure: timeout for retx = %d us", this, _timeout);
  return 0;
}

int
SimPhy1::initialize(ErrorHandler* errh)
{
  ScheduleInfo::join_scheduler(this, &_task, errh);
  _timer.initialize(this);

  _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);

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
SimPhy1::push(int port, Packet *p) 
{
  assert(port == PORT_CHANNEL);
  assert(p);
  if(_waiting_packet) 
  {
    click_chatter("%{element}:process_channel: drop due to packet already", this);
    p->kill();
    return;
  }

  struct mpitype* mt = (struct mpitype *) p->data();  
  switch(mt->type)
  {
    // FIXME: which one first, push or run_timer
    case MPITYPE_RX_DATA:
    {
      push_type(PORT_MAC, MPITYPE_CCA_BUSY);
      if(PAINT_ANNO(p) == SimChannel::RECV_DATA) 
      {
        click_chatter("%{element}:process_channel: in TX_range", this);
        _waiting_packet = p;
        output(PORT_MAC).push(_waiting_packet->clone());

        struct timeval tv;
        tv.tv_sec = _timeout/1000000; 
        tv.tv_usec = _timeout%1000000; 
        _timer.schedule_after(tv);
        
      }
      push_type(PORT_MAC, MPITYPE_CCA_IDLE);

      break;
    }
    default:
    {
      click_chatter("%{element}::process_channel: never this type = %d", this, mt->type);
      p->kill(); 
      break;
    }
  }

  return;

}

bool
SimPhy1::run_task()
{

  Packet *p = input(PORT_MAC).pull();

  if(p)
  {
    struct mpitype* mt = (struct mpitype *)p->data();
    switch(mt->type)
    {
      case MPITYPE_TX_DATA:
      {
        // first tx-ack
        push_type(PORT_MAC, MPITYPE_TX_ACK);

        // second forward to channel
        WritablePacket* p_out = p->uniqueify();
        struct mpitype* mtype = (struct mpitype *) p_out->data();
        mtype->type = MPITYPE_RX_DATA;
        output(PORT_CHANNEL).push(p_out);

        break;
      }
      case MPITYPE_RX_ACK:
      {
         if(_waiting_packet)
         {
           _timer.unschedule();
           _waiting_packet->kill(); _waiting_packet=0;
         }
         p->kill();
         break;
      }
      default:
      {
         click_chatter("%{element}::process_mac: never this type = %d", this, mt->type);
         p->kill();
         break;
      }
    }
  }
  else if(!_nonempty_signal)
  {
    // sleep until new packet is arrived in port 0
    return false;
  }

  _task.fast_reschedule();
  return (p!=0);

}

void
SimPhy1::run_timer(Timer* )
{
  click_chatter("%{element}:run_timer: retx", this);
  // for now, I just erase retransmission
  // because link is now reliable enough, I think.
  output(PORT_MAC).push(_waiting_packet->clone());

  struct timeval tv;
  tv.tv_sec = _timeout/1000000;
  tv.tv_usec = _timeout%1000000;
  _timer.schedule_after(tv);
}


void
SimPhy1::push_type(int port, uint8_t type)
{
  struct rxvector rv;

  rv.type = type;
  rv.rate = 0;
  rv.length = 0;
  rv.service = 0;
  // FIXME: what is the rssi for this case?
  rv.rssi = 0;

  WritablePacket* p_out = Packet::make(4, (unsigned char *)&rv, sizeof(struct rxvector), 4);

  if(!p_out)
  {
    click_chatter("%{element}::push_type fail to make new packet", this);
    return;
  }

  output(port).push(p_out);

}


CLICK_ENDDECLS
EXPORT_ELEMENT(SimPhy1)
