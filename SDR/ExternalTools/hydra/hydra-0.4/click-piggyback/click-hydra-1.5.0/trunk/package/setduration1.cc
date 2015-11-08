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
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>

#include <clicknet/80211.h>
#include "macframe80211.hh"

#include "setdefer.hh"
#include "phyprocesstime1.hh"
#include "setduration1.hh"

CLICK_DECLS

SetDuration1::SetDuration1()
 // : Element(1, 1), _sifs_defer(0), _phy_time(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

SetDuration1::~SetDuration1()
{
}

void*
SetDuration1::cast(const char *n)
{
  if( strcmp(n, "SetDuration1") == 0 )
    return (SetDuration1 *) this;


  return SetDuration::cast(n);
}

int
SetDuration1::configure(Vector<String> &conf, ErrorHandler *errh)
{

  hydra_chatter(9, 8, "%{element}::configure", this);
  Element* sifs_e = 0;
  Element* phy_time_e = 0;
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
		  "SIFS", cpElement, "SetDefer element SIFS", &sifs_e,
		  "PHYPROCESSTIME", cpElement, "PHYProcessTime element", &phy_time_e,
 		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  if(sifs_e) {
    _sifs_defer = (SetDefer *) sifs_e->cast("SetDefer");
    if(!_sifs_defer) return errh->error("SIFS must be set using SetDefer");
  } 

  if(phy_time_e) {
    _phy_time = (PHYProcessTime1 *) phy_time_e->cast("PHYProcessTime1");
    if(!_phy_time) return errh->error("PHYPROCESSTIME must be set using PHYProcessTime1 element");
  } 

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
SetDuration1::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if (p) 
  {
    adjust_duration(p);
  }

  return p;
}

// Adjust NAV for CTS and ACK based on that in RTS and DATA
const Packet*
SetDuration1::adjust_duration(const Packet* p)
{

  uint32_t sifs = 0;
  if(_sifs_defer)
    sifs = _sifs_defer->defer_time();

  if(_phy_time)
    sifs += _phy_time->mac_delay();

  struct common_80211* common_hdr = (struct common_80211 *) p->data(); 
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();
#ifndef IEEE80211_LONG_TIMER
  uint16_t* nav = (uint16_t *)common_hdr->i_dur;
#else
  uint32_t* nav = (uint32_t *)common_hdr->i_dur;
#endif

  hydra_chatter(8, _verbose, "%{element}::adjust_duration: org nav=%d.%06d", this, *nav/1000000, *nav%1000000);

  if( *nav > (sifs + real_reachtime(p->length(), cha->rate_mbps)) )
    *nav = *nav - sifs - real_reachtime(p->length(), cha->rate_mbps);
  else
    *nav = 0;

  hydra_chatter(8, _verbose, "%{element}::adjust_duration: chg nav=%d.%06d", this, *nav/1000000, *nav%1000000);

  return p;
}

// Calculate NAV for RTS and DATA packet of MAC frame
const Packet* 
SetDuration1::calculate_duration(const Packet* p, const Packet* p_aux)
{
#ifndef IEEE80211_LONG_TIMER
  uint16_t nav = 0;
#else
  uint32_t nav = 0;
#endif
  uint32_t sifs = 0;
  if(_sifs_defer)
    sifs = _sifs_defer->defer_time();

  if(_phy_time)
    sifs += _phy_time->mac_delay();

  struct common_80211* cmm_hdr = (struct common_80211*)p->data();
  unsigned char type = cmm_hdr->i_fc.type | (cmm_hdr->i_fc.subtype << 2);

  struct click_hydra_anno* cha = (struct click_hydra_anno *)p->all_user_anno();
  uint16_t rate = cha->rate_mbps; 

  // FIXME: Assume that ACK is sent with the rate of DATA
  switch(type)
  {
    // RTS
    // p_aux in this case is data packet
    case (FC0_TYPE_CTL | (FC0_SUBTYPE_RTS << 2)) :
    {
      assert(p_aux);
      struct click_hydra_anno* cha_aux = (struct click_hydra_anno *)p_aux->all_user_anno();

      nav = sifs + real_reachtime( sizeof(struct cts_80211), rate)  +
	    sifs + real_reachtime( p_aux->length(), cha_aux->rate_mbps) +
	    sifs + real_reachtime( sizeof(struct ack_80211), cha_aux->rate_mbps); 
      break;
    }
    case (FC0_TYPE_DATA | (FC0_SUBTYPE_DATA << 2)) :
    { 
      //if(IS_BROADCAST(cmm_hdr->i_ra)) 
      if(! cha->agg_anno->has_unicast ) 
	break;
      
      nav = sifs + real_reachtime( sizeof(struct ack_80211), rate);
      break;
    }
    default: 
    {
      hydra_chatter(0, _verbose, "%{element}::calculate_duration: never this case", this);
      break;
    }
  }

#ifndef IEEE80211_LONG_TIMER
  *(uint16_t *)cmm_hdr->i_dur = nav;
#else
  *(uint32_t *)cmm_hdr->i_dur = nav;
#endif

  hydra_chatter(8, _verbose, "%{element}::calculate_duration: %d.%06d", this, nav/1000000, nav%1000000);
  return p;

}

// It assume the timeout start after TX_DONE not TX_DATA
unsigned SetDuration1::calculate_timeout(const Packet* p, bool next)
{
  hydra_chatter(9, _verbose, "%{element}::calculate_timeout:", this);

  uint32_t timeout = 0;
  uint32_t sifs = 0;

  if(_sifs_defer)
    sifs = _sifs_defer->defer_time();
  if(_phy_time)
    sifs += _phy_time->mac_delay();

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();
  struct common_80211* cmm_hdr = (struct common_80211*)p->data();

  if(!next)  // Timeout for RTS and Data packet that needs next handshaking
  {
    if(is_rts(cmm_hdr->i_fc))
    {
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for RTS", this);
       // It expects CTS is sent with the same rate of RTS
       timeout = real_airtime(p->length(), cha->rate_mbps) +
                 real_rxtime(p->length(), cha->rate_mbps) +
                 sifs + 
                 real_reachtime( sizeof(struct cts_80211), cha->rate_mbps) ;

    }
/*
    else if(is_unicast(*cmm_hdr))
    {
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for Data", this);
       // It expect ACK is sent with the same rate of Data
       timeout = real_airtime(p->length(), cha->rate_mbps) +
                 real_rxtime(p->length(), cha->rate_mbps) +
                 sifs + 
                 real_reachtime( sizeof(struct ack_80211), cha->rate_mbps) ;
    }
    else if(is_broadcast(*cmm_hdr))
    {
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for Broadcast", this);
       timeout = real_airtime(p->length(), cha->rate_mbps) +
                 real_rxtime(p->length(), cha->rate_mbps);
    }
*/
    else if(is_data(*(struct fc_80211 *)cmm_hdr))
    {
      if((cha->agg_anno) && (cha->agg_anno->has_unicast))
      {
        hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for Data", this);
        // It expect ACK is sent with the same rate of Data
        timeout = real_airtime(p->length(), cha->rate_mbps) +
                  real_rxtime(p->length(), cha->rate_mbps) +
                  sifs + 
                  real_reachtime( sizeof(struct ack_80211), cha->rate_mbps) ;
      } 
      else
      {
         hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for Broadcast", this);
         timeout = real_airtime(p->length(), cha->rate_mbps) +
                   real_rxtime(p->length(), cha->rate_mbps);
      }
    }

  }
  else  // Timeout for CTS and ACK responded to RTS and Data from peer 
  {
    if(is_rts(cmm_hdr->i_fc))
    {
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for CTS", this);
#ifndef IEEE80211_LONG_TIMER
      uint16_t nav;
      memcpy(&nav, cmm_hdr->i_dur, sizeof(uint16_t));
#else
      uint32_t nav;
      memcpy(&nav, cmm_hdr->i_dur, sizeof(uint32_t));
#endif
      if(nav)
      {
        // Assume that ACK use same rate with RTS
        timeout = nav - 
                  sifs - real_txtime(sizeof(struct cts_80211), cha->rate_mbps) -
                  sifs - real_reachtime(sizeof(struct ack_80211), cha->rate_mbps); 
      }
      else 
      {  // when we don't have NAV, we can't calculate exact timeout
         // Just assume that data sent with MTU size
         // And assume that CTS uses the same rate with RTS
         timeout = real_airtime(sizeof(struct cts_80211), cha->rate_mbps) + 
                   real_rxtime(sizeof(struct cts_80211), cha->rate_mbps) + 
                   sifs + 
                   real_reachtime( sizeof(struct data_80211) + IEEE80211_MTU, cha->rate_mbps);
      }
    }
/*
    else if(is_unicast(*cmm_hdr))
    {
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for ACK", this);
       timeout = real_airtime( sizeof(struct ack_80211), cha->rate_mbps ) +
                 real_rxtime( sizeof(struct ack_80211), cha->rate_mbps );
    }
*/
    else if(is_data(*(struct fc_80211 *)cmm_hdr))
    {
      if((cha->agg_anno) && (cha->agg_anno->has_unicast))
      {
         hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for ACK", this);
         timeout = real_airtime( sizeof(struct ack_80211), cha->rate_mbps ) +
                   real_rxtime( sizeof(struct ack_80211), cha->rate_mbps );

      }
    }
  }

  return timeout;
}

uint32_t SetDuration1::real_txtime(int len, uint8_t)
{
  unsigned txtime = 0;

  if(_phy_time)
    txtime += _phy_time->tx_delay() + _phy_time->tx_encoding(len); 

  return txtime; 
}

uint32_t SetDuration1::real_rxtime(int len, uint8_t)
{
  unsigned rxtime = 0; 

  if(_phy_time)
    rxtime += _phy_time->rx_delay() + _phy_time->rx_decoding(len); 

  return rxtime; 
}

uint32_t SetDuration1::real_airtime(int len, uint8_t rate)
{
  unsigned airtime = txtime_usec_long(len, rate); 

  // FIXME: check following casting

  airtime *= ((unsigned) _phy_time->symbol_extension());

  return airtime; 
}

uint32_t SetDuration1::real_reachtime(int len, uint8_t rate)
{
  return real_txtime(len, rate) + real_airtime(len, rate) + real_rxtime(len, rate); 
}

/*
enum {H_VERBOSE};

String
SetDuration1::read_param(Element *e, void *thunk)
{
  SetDuration1 *td = (SetDuration1 *)e;
  switch ((uintptr_t) thunk) 
  {
    case H_VERBOSE:
      return String(td->_verbose) + "\n";
    default:
      return String();
  }

  return String();

}

int 
SetDuration1::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  SetDuration1 *f = (SetDuration1 *)e;
  String s = cp_uncomment(in_s);
  switch((int)vparam) 
  {
    case H_VERBOSE: 
    {
      unsigned m;
      if (!cp_unsigned(s, &m)) 
        return errh->error("stepup parameter must be unsigned");
      f->_verbose = m;
      break;
    }
  }

  return 0;

}
void
SetDuration1::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}
*/

CLICK_ENDDECLS
EXPORT_ELEMENT(SetDuration1)

