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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/dcfscheduler.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/glue.hh>
#include <clicknet/ether.h>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "macframe.hh"
#include "collisionpolicy.hh"
#include "dcfscheduler.hh"
#include "setduration.hh"
#include "ratecontrol.hh"

CLICK_DECLS

static Packet *
clone_packet_with_agg_anno(Packet *p)
{
  // Clone the packet
  Packet *cloned = p->clone();
  struct click_hydra_anno *p_cha = 
                             (struct click_hydra_anno *) p->all_user_anno();
  struct click_hydra_anno *cloned_cha = 
                         (struct click_hydra_anno *) cloned->all_user_anno();

  // FIXME:temporary
  p_cha->agg_anno->len_metadata=0; 
  //printf("len_metadata=%d\n", p_cha->agg_anno->len_metadata);
  
  //printf("[%20s][%04d] p_cha->agg_anno: 0x%x\n", __FILE__, __LINE__,
   //     p_cha->agg_anno);
  // Allocate the new agg_anno memory in the cloned packet
  agg_anno_allocator(cloned, p_cha->agg_anno->len_metadata);
  //printf("[%20s][%04d] Here\n", __FILE__, __LINE__);

  // Copy the data structures over
  //  printf("[%20s][%04d] Here\n", __FILE__, __LINE__);
  memcpy(cloned_cha->agg_anno, p_cha->agg_anno, sizeof(*cloned_cha->agg_anno));
  //  printf("[%20s][%04d] Here\n", __FILE__, __LINE__);
  memcpy(cloned_cha->agg_anno->metadata, p_cha->agg_anno->metadata,
         p_cha->agg_anno->len_metadata);
  
  return cloned;
}

DCFScheduler::DCFScheduler() 
  : _rts_frame(0), _rts_rate(0), _aggregator(0),
    _duration(0), _state(DCF_IDLE), _pstate(DCF_IDLE), _regard_it_lost(false)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

DCFScheduler::~DCFScheduler()
{
}

void*
DCFScheduler::cast(const char *n)
{
  if( strcmp(n, "DCFScheduler") == 0 )
    return (DCFScheduler *) this;

  return MACTXScheduler::cast(n);
}

int
DCFScheduler::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element *cp_e = 0; 
  Element *rts_frame_e, *rts_rate_e = 0;
  Element *duration_e, *tx_notifier_e = 0;
  Element *aggregator_e = 0;

  // FIXME: change the macframe from element to supplment class
  int res = cp_va_parse(conf, this, errh,
 			cpElement, "collision reducing polocy", &cp_e,
			cpElement, "generate RTS packet", &rts_frame_e,
			cpElement, "data rate for RTS packet", &rts_rate_e,
 			cpElement, "duration calulator (nav, timeout)", &duration_e,
            cpElement, "aggregator", &aggregator_e,
                        //cpIgnore,
			cpOptional,
			cpKeywords,
			"NOTIFIER", cpElement, "mac notifier", &tx_notifier_e,
		        "TIMEOUT", cpSecondsAsMicro, "time between retried us)", &_timeout,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  if(cp_e) {
    _cp = (CollisionPolicy *) cp_e->cast("CollisionPolicy");
    if(!_cp) return errh->error("CollisionReducing policy must be set using CollisionPolicy");
  } else {
    return errh->error("Collision reducing policy must be specified");
  }

  if(rts_frame_e) {
    _rts_frame = (MACFrame *) rts_frame_e->cast("MACFrame");
    if(!_rts_frame) return errh->error("RTS frame must be set using MACFrame");
  } else {
    return errh->error("RTS Frame must be specified");
  }

  if(rts_rate_e) {
    _rts_rate = (RateControl *) rts_rate_e->cast("RateControl");
    if(!_rts_rate) return errh->error("RTS data rate must be set using RateControl");
  } else {
    return errh->error("RTS data rate must be specified");
  }

  if(duration_e) {
    _duration = (SetDuration *) duration_e->cast("SetDuration");
    if(!_duration) return errh->error("Duration calculator must be set using SetDuration");
  } else {
    return errh->error("Duration calculator must be specified");
  }

  if (aggregator_e) {
    _aggregator = (UBAggregator *) aggregator_e->cast("UBAggregator");
    if(!_aggregator) return errh->error("Aggregator must be set using UBAggregator");
  } else {
    return errh->error("Aggregator must be specified");
  }
 
  if(tx_notifier_e) {
    _tx_notifier = (MACNotifier *) tx_notifier_e->cast("MACNotifier");
    if(!_tx_notifier) return errh->error("NOTIFIER must be set using MACNotifier");
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

void
DCFScheduler::push(int port, Packet *p)
{
  struct timeval tv;
  bool kill_incoming = true;
  click_gettimeofday(&tv);
  hydra_chatter(8, _verbose, "%{element}::push: %d.%06d", this, tv.tv_sec, tv.tv_usec);

  // control port for feedback or scheduling info
  assert(port != 0);
  //check();

  NextTXPacket ntxp = process_control(p);

  switch(ntxp)
  {
    case NTXP_CTS:  // received RTS
    {
      _pstate = _state;
      hydra_chatter(8, _verbose, "%{element}::push: After recv RTS", this);
      set_state(DCF_WAITTX_CTS);
      push_type(3, HYDRA_CCA_BUSY);
      prep_cts(_control_packet);
      push_packet(2, _control_packet); _control_packet = 0;
      break;
    }
    case NTXP_RESPONSE: // received CTS
    {
      hydra_chatter(8, _verbose, "%{element}::push: After recv CTS", this);

      _timer.unschedule();
      //_cp->success(_trigger_packet);
       
      set_state(DCF_WAITTX_RESPONSE);

      // push the CTS-in packet to the waiting queue
      push_packet(4, p);
      kill_incoming = false;

      // push the data packet to the passcsi element
      prep_response(_waiting_packet);
      push_packet(1, _waiting_packet);
      _waiting_packet = 0;

      break;
    }
    case NTXP_ACK:  // received DATA
    {
      if(!is_state(DCF_TX_CTS)) _pstate = _state;

      hydra_chatter(8, _verbose, "%{element}::push: After recv DATA", this);

      if(_timer.scheduled());
        _timer.unschedule();

      set_state(DCF_WAITTX_ACK);
      push_type(3, HYDRA_CCA_BUSY);
      prep_ack(_control_packet);
      push_packet(2, _control_packet); _control_packet = 0;
      break;
    }
    case NTXP_DONE:  // received ACK
    {
      hydra_chatter(8, _verbose, "%{element}::push: After recv ACK", this);

      _timer.unschedule();
      _cp->success(_response_packet ? _response_packet : _trigger_packet);

      prep_done(_response_packet ? _response_packet : _trigger_packet);
     
      set_state(DCF_IDLE);
 
      if(_response_packet) 
      { 
	_response_packet->kill(); _response_packet = 0;
      }

      _trigger_packet->kill(); _trigger_packet = 0;

      _nonfull_notifier.wake(); 
      _task.reschedule(); 
      break;

    }
    case NTXP_NOTHING:
    default:
      break;
  }

  // Conditional remove the incoming packet
  if (kill_incoming)
    p->kill();
}

bool
DCFScheduler::run_task()
{
  hydra_chatter(9, _verbose, "%{element}::run_task: ", this);

  // if there is nothing to do, go to sleep
  if(!_nonempty_signal && !_nonfull_signal)
  {
    hydra_chatter(4, _verbose, "%{element}: sleep due to no signal", this);
    return false;
  }

  // first check if there is packet to push, 
  // assuming that only one packet can be sent
  if(is_state(DCF_WAITTX_TRIGGER) && _waiting_packet)
  {
    if(_nonfull_signal)
    {
      push_packet(0, _waiting_packet); _waiting_packet = 0;
      _task.fast_reschedule();
      return true;
    }

    // sleep until waiting packet is sent by task 
    hydra_chatter(4, _verbose, "%{element}: sleep due to trigger packet waited", this);
    return false;
  }


  // second, check if there is outstanding packet
  if(_state != DCF_IDLE)
  {
    // task should be waken up by timer when no more retransmission
    hydra_chatter(4, _verbose, "%{element}: sleep due to outstanding packet", this);
    return false;
  }

  Packet *p = input(0).pull();
  if(p)
  {
    hydra_chatter(8, _verbose, "%{element}::run_task: data pulled", this);

    // For unicast data of DCF mode, generate RTS and link RTS to _trigger_packet
    // For multicast data , just link Data to _trigger packet
    _trigger_packet = generate_trigger(p);
    if(!_trigger_packet)
    {
      p->kill();
      _task.fast_reschedule();
      return true;
    }

    _response_packet = generate_response(p);
    p->kill();

    // FIXME: sleep only listeners connected to input(0)
    _nonfull_notifier.sleep();

    _cp->reset_retry(_trigger_packet);
    // choiiss: 4. generate agg_anno only if _trigger_packet is data
    //             You can check it by looking at if you have _response_packet
    if (!_response_packet)
    {
      Packet *pb = _cp->set_control(_trigger_packet);
      _waiting_packet = clone_packet_with_agg_anno(pb);
    } else
    {
      _waiting_packet  = _cp->set_control(_trigger_packet)->clone();
    }

    set_state(DCF_WAITTX_TRIGGER);

    if(_nonfull_signal)
    {
      push_packet(0, _waiting_packet); _waiting_packet = 0;
    }
    // else 
    // if downstream element is full, wait until nonfull_signal is set in run_task
  }
  else if (!_nonempty_signal)
  {
    // upstream is empty
    // so, sleep until new packet is arrived in port 0 or downstream queue is not full
    hydra_chatter(4, _verbose, "%{element}: sleep due to no nonempty signal", this);
    return false;
  }

  _task.fast_reschedule();
  return true;
}


void
DCFScheduler::run_timer(Timer* )
{
  hydra_chatter(8, _verbose, "%{element}::run_timer: ", this);

  // prepare_packet generate new waiting packet by make or clone 
  if(is_state(DCF_TX_TRIGGER) || is_state(DCF_TX_RESPONSE))
    transmitter_timer();
  else if(is_state(DCF_TX_CTS) || is_state(DCF_TX_ACK))
    receiver_timer();   

  //check();
}

void
DCFScheduler::transmitter_timer()
{
  hydra_chatter(8, _verbose, "%{element}::transmitter_timer: ", this);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) _trigger_packet->all_user_anno();

  Packet* retry_packet = 0;

  // FIXME: push fail or success packet to the last output port.
  // wonsoo: -1 +1 : segmentation fault 
  //if(cha->agg_anno->has_unicast)
  if(!((cha->agg_anno) && (!cha->agg_anno->has_unicast)))
  {
    // CHECKME: changed for ARF -1 +4
    //_cp->fail(_trigger_packet);
    if( is_state(DCF_TX_RESPONSE))
      _cp->fail(_response_packet);
    else if(is_state(DCF_TX_TRIGGER)) 
      _cp->fail(_trigger_packet);

    retry_packet = _cp->set_control(_trigger_packet);
  }

  if(retry_packet)//retransmission
  {
    _waiting_packet = retry_packet->clone();
    struct common_80211 *cmm_hdr = (struct common_80211 *)_waiting_packet->data() ;
    struct click_hydra_anno *wp_cha 
       = (struct click_hydra_anno *)_waiting_packet->all_user_anno();
    //wonsoo +1 -1
    //if(wp_cha->agg_anno->has_unicast)
    if( (wp_cha->agg_anno) && (wp_cha->agg_anno->has_unicast))
    { // UNICAST without RTS
      prep_unicast(_waiting_packet);
    }
 
    printf("RETX\n");   
    hydra_chatter(8, _verbose, "%{element}: transmit_timer: retransmission", this);  
    set_state(DCF_WAITTX_TRIGGER);

    if(_nonfull_signal)
    {
        push_packet(0, _waiting_packet); _waiting_packet = 0;
    }
  }
  else // no retransmission, just drop
  {
    _cp->reset_retry(_trigger_packet);

    if(_response_packet) 
    {
      _response_packet->kill(); _response_packet = 0;
    }
    _trigger_packet->kill(); _trigger_packet = 0;   
  
    set_state(DCF_IDLE); 

    _nonfull_notifier.wake();
    _task.reschedule();
  }

}

void
DCFScheduler::receiver_timer()
{
  hydra_chatter(8, _verbose, "%{element}::receiver_timer: ", this);

  push_type(3, HYDRA_CCA_IDLE);

  // very special case
  if(_pstate == DCF_WAITTX_TRIGGER && _regard_it_lost)
  {
    _pstate = DCF_TX_TRIGGER;
    _regard_it_lost = false;
  }

  if(_pstate == DCF_WAITTX_TRIGGER)
  {
    set_state(_pstate);
    // recalculate timeout, since receiving process changes timeout.
    calculate_timeout(_trigger_packet);

    _task.reschedule();
    return;
  }
  else if(_pstate == DCF_TX_TRIGGER)
  {
    set_state(_pstate);

    // On purpose, invoke the timer
    // since this case is considered as collision.
    _timeout_val.tv_sec = _timeout_val.tv_usec = 0;
    _timer.schedule_after(_timeout_val);   

    return;
  }

  set_state(DCF_IDLE);
  _nonfull_notifier.wake(); 
  _task.reschedule();
}

void 
DCFScheduler::notified(uint32_t, MACEvent e, const Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::notifed: ", this);

  if( !p || (e != MACListener::TX_DONE && e != MACListener::TX_CHANGE) )
  {
    hydra_chatter(4, _verbose, "%{element}::notify: wrong event", this);
    return;
  }
  
  struct click_hydra_anno* cha = (struct click_hydra_anno *)p->all_user_anno();
  struct fc_80211 fc;
  memcpy(&fc, &cha->mac_type, sizeof(cha->mac_type));
  //printf("recover fc = 0x%x mac_type = 0x%x\n", *(uint16_t *)&fc, cha->mac_type);

  if( e == MACListener::TX_CHANGE)
  {
    if( (is_rts(fc) || is_data(fc)) && 
         (is_state(DCF_WAITTX_CTS) || is_state(DCF_WAITTX_ACK)))
    {
      // For this case, we consider it as collision occurred 
      // when sending TRIGGER.
      // So, just set the previous state as TRIGGER sent,
      // although TRIGGER is dropped in ToPhy.
      // Remain procedure will be done when receiving is finished.
      hydra_chatter(4, _verbose, "%{element}::notified change packet %d", this, _state);
      assert(_pstate == DCF_WAITTX_TRIGGER);
      _pstate = DCF_TX_TRIGGER;
      // choiiss: added after gnuradio deploied
      // this should not occur now.
      assert(false);
    }
    else 
    {
      hydra_chatter(4, _verbose, "%{element}::notified change wrong state %d", this, _state);
      assert(false);
    }
    return;
  }
  else //if(e == MACListener::TX_DONE)
  {
    // FIXME: check the packet type sent
    if(is_rts(fc) && is_state(DCF_WAITTX_TRIGGER)) 
    {
      set_state(DCF_TX_TRIGGER);
    }
    else if(is_cts(fc) && is_state(DCF_WAITTX_CTS)) 
    {
      set_state(DCF_TX_CTS);
    }
    else if(is_data(fc) && 
            (is_state(DCF_WAITTX_TRIGGER) || is_state(DCF_WAITTX_RESPONSE))) 
    {
      if(is_state(DCF_WAITTX_TRIGGER)) set_state(DCF_TX_TRIGGER);
      else  set_state(DCF_TX_RESPONSE);
    }
    else if(is_ack(fc) && is_state(DCF_WAITTX_ACK)) 
    {
      set_state(DCF_TX_ACK);
    }
    else 
    {
      // It's very special case, RTS is sent at ToLabviewPHY
      // but CTS is waiting on DCFTransmitter.
      // So, in this case, we should set that this RTS is lost and retransmit.
      if((is_rts(fc) || is_data(fc) )&& is_state(DCF_WAITTX_CTS))
      {
        hydra_chatter(4, _verbose, "%{element}::very special case", this);
        _regard_it_lost = true;
        // choiiss: added after gnuradio
        // this should not occurs now.
        assert(false);
      }
      else
      {
        hydra_chatter(4, _verbose, "%{element}::notified wrong state %d", this, _state);
        assert(false);
      }

      return;
    }
  }

  assert(!_timer.scheduled());
   
  // FIXME: if remain time is too short, just busy waiting
  hydra_chatter(4, _verbose, "%{element}::notified: start timer", this);
  _timer.schedule_after(_timeout_val);
}

Packet*
DCFScheduler::generate_trigger(Packet *p)
{
  assert(p);
  hydra_chatter(8, _verbose, "%{element}::generate_trigger: ", this);

  Packet* p_out = 0;
  struct common_80211 *cmm = (struct common_80211 *) p->data();
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();

  assert(cha->agg_anno);

  if( ! cha->agg_anno->has_unicast )
  { // BROADCAST
    fprintf(stderr, "[%20s][%04d] Here\n", __FILE__, __LINE__);
    p_out = clone_packet_with_agg_anno(p);
    prep_broadcast(p_out);
  }
  else if( !(cha->flags & HYDRA_DO_RTS) )
  { // UNICAST without RTS
    fprintf(stderr, "[%20s][%04d] Here\n", __FILE__, __LINE__);
    p_out = clone_packet_with_agg_anno(p);
    prep_unicast(p_out);
  }
  else
  { //RTS
    fprintf(stderr, "[%20s][%04d] Here\n", __FILE__, __LINE__);
    p_out = _rts_frame->prepare_packet(p); 
    if(!p_out)
      return p_out;
    _rts_rate->get_txrate(p_out);
    prep_rts(p_out);
  }

  _duration->calculate_duration(p_out, p); 

  return p_out;
}

Packet*
DCFScheduler::generate_response(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::generate_response: ", this);

  struct common_80211* cmm = (struct common_80211 *) p->data();
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();

  Packet* p_out;
  //DATA
  if( (cha->flags & HYDRA_DO_RTS) && (cha->agg_anno->has_unicast) )
  {
    printf("[%20s][%04d] Here\n", __FILE__, __LINE__);
    p_out = clone_packet_with_agg_anno(p);;
    _duration->calculate_duration(p_out, p); 
  }
  else
  {
    p_out = 0;
  }
  return p_out; 
}

DCFScheduler::NextTXPacket
DCFScheduler::process_control(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::process_control: cstate = %d", this, _state);

  // Assumes that Packet p's destination is me.
  struct common_80211 cmm_recv ;
  memcpy(&cmm_recv, p->data(), sizeof(struct common_80211));

  if( is_rts(cmm_recv.i_fc) && 
      (is_state(DCF_IDLE) || is_state(DCF_WAITTX_TRIGGER)) )
  {
      hydra_chatter(8, _verbose, "%{element}::process_control: recv RTS", this, _state);
     _control_packet = p->clone();
     return NTXP_CTS; 
  }
  else if( is_cts(cmm_recv.i_fc) && is_state(DCF_TX_TRIGGER) )
  {
      hydra_chatter(8, _verbose, "%{element}::process_control: recv CTS", this, _state);
      _waiting_packet = _response_packet->clone();

      // calculate nav for data
      //struct common_80211* cmm_send = (struct common_80211 *)_waiting_packet->data();
/*
#ifndef IEEE80211_LONG_TIMER
     memcpy(cmm_send->i_dur, cmm_recv.i_dur, sizeof(uint16_t));
#else
     memcpy(cmm_send->i_dur, cmm_recv.i_dur, sizeof(uint32_t));
#endif
*/
      return NTXP_RESPONSE;
  }
  // FIXME: Do we need to check the agg_anno->has_unicast field here?
  else if( is_unicast(cmm_recv) && 
           (is_state(DCF_TX_CTS) || is_state(DCF_IDLE)  || is_state(DCF_WAITTX_TRIGGER)))
  {
      hydra_chatter(8, _verbose, "%{element}::process_control: recv Data", this, _state);
      _control_packet = p->clone();
      return NTXP_ACK;
  }
  else if( is_ack(cmm_recv.i_fc) && 
          (is_state(DCF_TX_RESPONSE) || 
           (is_state(DCF_TX_TRIGGER) && !_response_packet)) )
  {
      hydra_chatter(8, _verbose, "%{element}::process_control: recv ACK", this, _state);
      return NTXP_DONE;
  }
  
  return NTXP_NOTHING;
}

void
DCFScheduler::push_packet(int port, Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::push_packet: ", this);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();
  struct fc_80211* fc = (struct fc_80211 *)p->data();

  memcpy(&cha->mac_type, fc, sizeof(cha->mac_type));
  //printf("copy fc = 0x%x mac_type = 0x%x\n", *(uint16_t *)fc, cha->mac_type);

  calculate_timeout(p);
  if(_tx_notifier == 0 )
  {
    MACListener::MACEvent e = MACListener::TX_DONE;
    notified(0, e, p);
  }
  output(port).push(p);
}

void
DCFScheduler::push_type(int port, int type)
{
  hydra_chatter(8, _verbose, "%{element}::push_type: ", this);

  Packet* p_out = Packet::make(HYDRA_HEADROOM, 0, 0, HYDRA_TAILROOM); 
  struct click_hydra_anno *cha = (struct click_hydra_anno *) p_out->all_user_anno();
  cha->type = type;

  output(port).push(p_out);
}

void 
DCFScheduler::calculate_timeout(const Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::calculate_timeout: ", this);

  uint32_t timeout = 0;
  if(_timeout)
  {
     struct common_80211* cmm = (struct common_80211 *)p->data();
     // FIXME: these values are for wired line test only.
     // please change this routine according to the PHY processing time.
     if( (is_state(DCF_WAITTX_TRIGGER) && is_broadcast(*cmm)) ||
          is_state(DCF_WAITTX_ACK) )
     {
       timeout = 0;
     }
     else if( is_state(DCF_WAITTX_TRIGGER) || is_state(DCF_WAITTX_CTS) || is_state(DCF_WAITTX_RESPONSE))
     {
       timeout = _timeout;
     }
     else
     {
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: wrong state %d", this, _state);
       assert(false);
     }
  }
  else if(_tx_notifier)
  {
    // calculate exact timeout value 
    // only when it can be notified the exact transmission time
    bool next = is_state(DCF_WAITTX_CTS) || is_state(DCF_WAITTX_ACK);
    timeout = _duration->calculate_timeout(p, next); 
  }
  else 
  {
    // if timeout is zero, set the timeout with enough duration
    timeout = 100000; // 100m sec
  }

  _timeout_val.tv_sec =  timeout/(1000000);
  _timeout_val.tv_usec = timeout%(1000000);

  hydra_chatter(8, _verbose, "%{element}::calculate_timeout: %d.%06d", this, _timeout_val.tv_sec, _timeout_val.tv_usec);
}


void
DCFScheduler::check()
{
  // check() should be called *before* most pushes() from element
  // functions, as each push may call back into the element.
  
}

void 
DCFScheduler::set_state(DCFState state)
{
  _state = state;
  switch(_state)
  {
    case DCF_IDLE: 
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_IDLE", this);
      break;
    case DCF_WAITTX_TRIGGER: 
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_WAITTX_TRIGGER", this);
      break;
    case DCF_WAITTX_RESPONSE: 
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_WAITTX_RESPONSE", this);
      break;
    case DCF_TX_TRIGGER: 
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_TX_TRIGGER", this);
      break;
    case DCF_TX_RESPONSE:
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_TX_RESPONSE", this);
      break;
    case DCF_WAITTX_CTS: 
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_WAITTX_CTS", this);
      break;
    case DCF_WAITTX_ACK: 
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_WAITTX_ACK", this);
      break;
    case DCF_TX_CTS: 
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_TX_CTS", this);
      break;
    case DCF_TX_ACK:
      hydra_chatter(8, _verbose, "%{element}::set_state: DCF_TX_ACK", this);
      break;
    default:
      hydra_chatter(8, _verbose, "%{element}::set_state: no such state", this);
      break;
  }

}

bool 
DCFScheduler::is_state(DCFState state)
{
  return _state == state;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(DCFScheduler)
