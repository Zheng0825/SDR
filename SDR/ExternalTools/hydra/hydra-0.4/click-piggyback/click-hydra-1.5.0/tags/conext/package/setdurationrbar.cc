/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
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
#include "setdurationrbar.hh"

CLICK_DECLS

SetDurationRbar::SetDurationRbar()
 // : Element(1, 1), _sifs_defer(0), _phy_time(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

SetDurationRbar::~SetDurationRbar()
{
}

void*
SetDurationRbar::cast(const char *n)
{
  if( strcmp(n, "SetDurationRbar") == 0 )
    return (SetDurationRbar *) this;


  return SetDuration1::cast(n);
}


Packet *
SetDurationRbar::simple_action(Packet *p)
{
  //hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if (p) 
  {
    adjust_duration(p);
  }

  return p;
}

// actually it just sets len and rate of data packet into header for RBAR
const Packet*
SetDurationRbar::adjust_duration(const Packet* p)
{
  struct common_rbar* common_hdr = (struct common_rbar *) p->data(); 
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();

  common_hdr->i_rate = convert_rate(cha->rate_data_frame);
  common_hdr->i_len = cha->len_data_frame; 

  hydra_chatter(8, _verbose, "%{element}::adjust_duration: data len=%d data rate = [%d]", this, common_hdr->i_len, common_hdr->i_rate);

  return p;
}

// actually it just sets len and rate of data packet into header for RBAR
const Packet* 
SetDurationRbar::calculate_duration(const Packet* p, const Packet* p_aux)
{

  struct common_rbar* cmm_hdr_p = (struct common_rbar*)p->data();
  unsigned char type_p = cmm_hdr_p->i_fc.type | (cmm_hdr_p->i_fc.subtype << 2);

  struct click_hydra_anno* cha_p = (struct click_hydra_anno *)p->all_user_anno();

  switch(type_p)
  {
    // RTS, so  p_aux in this case is data packet
    case (FC0_TYPE_CTL | (FC0_SUBTYPE_RTS << 2)) :
    {
      struct click_hydra_anno* cha_aux = (struct click_hydra_anno *) p_aux->all_user_anno();

      hydra_chatter(9, _verbose, "%{element}::calculate_duration for RTS data rate = %d, data rate = %dM", this, p_aux->length(), cha_aux->rate_mbps);
      cmm_hdr_p->i_rate = convert_rate(cha_aux->rate_mbps);
      cmm_hdr_p->i_len = p_aux->length(); 
      cha_p->rate_data_frame = cha_aux->rate_mbps;
      cha_p->len_data_frame = p_aux->length();
      break;
    }
    case (FC0_TYPE_DATA | (FC0_SUBTYPE_DATA << 2)) :  // data
    { 
      hydra_chatter(9, _verbose, "%{element}::calculate_duration for Data data rate = %d, data rate = %dM", this, p->length(), cha_p->rate_mbps);
      cmm_hdr_p->i_rate = convert_rate(cha_p->rate_mbps);
      cmm_hdr_p->i_len = p->length(); 
      cha_p->rate_data_frame = cha_p->rate_mbps;
      cha_p->len_data_frame = p->length();
      break;
    }
    default: 
    {
      hydra_chatter(0, _verbose, "%{element}::calculate_duration: never this case", this);
      break;
    }
  }

  cha_p->rate_data_frame = unconvert_rate(cmm_hdr_p->i_rate); 
  cha_p->len_data_frame = cmm_hdr_p->i_len; 

  //hydra_chatter(8, _verbose, "%{element}::calculate_duration: data rate = %d len = %d", this, cmm_hdr_p->i_rate, cmm_hdr_p->i_len);
  return p;

}

// It actually calculate nav
uint32_t 
SetDurationRbar::calculate_duration(const Packet* p)
{
  struct common_rbar* cmm_hdr = (struct common_rbar*)p->data();
  struct click_hydra_anno* cha = (struct click_hydra_anno *)p->all_user_anno();

  // FIXME: ACK duration is calculated using data rate
  uint16_t control_rate = cha->rate_mbps; 
  uint16_t data_rate = unconvert_rate(cmm_hdr->i_rate); 
  uint16_t data_len = cmm_hdr->i_len; 

  uint32_t sifs = 0;
  if(_sifs_defer)
    sifs = _sifs_defer->defer_time();

  if(_phy_time)
    sifs += _phy_time->mac_delay();

  uint32_t nav = 0;
  if(is_rts(cmm_hdr->i_fc))
  {
      nav = sifs + real_reachtime( sizeof(struct cts_80211), control_rate)  +
	    sifs + real_reachtime( data_len, data_rate) +
	    sifs + real_reachtime( sizeof(struct ack_80211), control_rate); 
  }
  else if(is_cts(cmm_hdr->i_fc))
  {
      nav = sifs + real_reachtime( data_len, data_rate) +
	    sifs + real_reachtime( sizeof(struct ack_80211), control_rate); 
  }
  else if(is_data(cmm_hdr->i_fc))
  { 
    if(!IS_BROADCAST(cmm_hdr->i_ra))
      nav = sifs + real_reachtime( sizeof(struct ack_80211), control_rate);
  }
  //else if(is_ack(cmm_hdr->i_fc || is_broadcast(*cmm_hdr)) { nav = 0;}
  else
  {
    hydra_chatter(0, _verbose, "%{element}::calculate_duration: never this case", this);
  }

  cha->rate_data_frame = cmm_hdr->i_rate; 
  cha->len_data_frame = cmm_hdr->i_len; 
  hydra_chatter(8, _verbose, "%{element}::calculate_duration: %d.%06d", this, nav/1000000, nav%1000000);
  return nav;

}

// It assume the timeout start after TX_DONE not TX_DATA
unsigned SetDurationRbar::calculate_timeout(const Packet* p, bool next)
{
  uint32_t timeout = 0;
  uint32_t sifs = 0;

  if(_sifs_defer)
    sifs = _sifs_defer->defer_time();
  if(_phy_time)
    sifs += _phy_time->mac_delay();

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();
  struct common_rbar* cmm_hdr = (struct common_rbar *)p->data();

  if(!next)  // Timeout for RTS and Data packet that needs next handshaking
  {
    if(is_rts(cmm_hdr->i_fc))
    {
       // It expects CTS is sent with the same rate of RTS
       timeout = real_airtime(p->length(), cha->rate_mbps) +
                 real_rxtime(p->length(), cha->rate_mbps) +
                 sifs + 
                 real_reachtime( sizeof(struct cts_rbar), cha->rate_mbps) ;
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for RTS %d", this, timeout);

    }
    else if(is_data(cmm_hdr->i_fc))
    {
      if(!IS_BROADCAST(cmm_hdr->i_ra))
      {
        // It expect ACK is sent with the same rate of Data
        timeout = real_airtime(p->length(), cha->rate_mbps) +
                  real_rxtime(p->length(), cha->rate_mbps) +
                  sifs + 
                  real_reachtime( sizeof(struct ack_rbar), cha->rate_mbps) ;
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for Data %d", this, timeout);
      }
      else 
      {
        timeout = real_airtime(p->length(), cha->rate_mbps) +
                  real_rxtime(p->length(), cha->rate_mbps);
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for Broadcast %d", this, timeout);
      }
    }
  }
  else  // Timeout for CTS and ACK responded to RTS and Data from peer 
  {
    if(is_rts(cmm_hdr->i_fc))
    {
      timeout = real_airtime(sizeof(struct cts_rbar), cha->rate_mbps) +
                real_rxtime(sizeof(struct cts_rbar), cha->rate_mbps) +
                sifs + 
                real_reachtime(cha->len_data_frame, cha->rate_data_frame) ;
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for CTS %d", this, timeout);
    }
    else if(is_data(cmm_hdr->i_fc))
    {
      if(!IS_BROADCAST(cmm_hdr->i_ra))
      {
        // FIXME: ACK is sent with rate of data packet
        timeout = real_airtime( sizeof(struct ack_rbar), cha->rate_mbps ) +
                  real_rxtime( sizeof(struct ack_rbar), cha->rate_mbps );
       hydra_chatter(8, _verbose, "%{element}::calculate_timeout: for ACK %d", this, timeout);
      }
    }
  }

  return timeout;
}

uint8_t SetDurationRbar::convert_rate(uint16_t anno_rate)
{
  uint8_t rbar_rate = 6;

  switch(anno_rate)
  {
    case  6: rbar_rate = RBAR_6M;  break;
    case  9: rbar_rate = RBAR_9M;  break;
    case 12: rbar_rate = RBAR_12M; break;
    case 18: rbar_rate = RBAR_18M; break;
    case 24: rbar_rate = RBAR_24M; break;
    case 36: rbar_rate = RBAR_36M; break;
    case 48: rbar_rate = RBAR_48M; break;
    case 54: rbar_rate = RBAR_54M; break;
  }

  return rbar_rate;

}

uint16_t SetDurationRbar::unconvert_rate(uint8_t rbar_rate)
{
  uint8_t anno_rate = 0;

  hydra_chatter(9, _verbose, "%{element}::unconvert_rate [%d]", this, rbar_rate);
  switch(rbar_rate)
  {
    case RBAR_6M:  anno_rate =  6; break;
    case RBAR_9M:  anno_rate =  9; break;
    case RBAR_12M: anno_rate = 12; break;
    case RBAR_18M: anno_rate = 18; break;
    case RBAR_24M: anno_rate = 24; break;
    case RBAR_36M: anno_rate = 36; break;
    case RBAR_48M: anno_rate = 48; break;
    case RBAR_54M: anno_rate = 54; break;
  }

  return anno_rate;

}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetDurationRbar)

