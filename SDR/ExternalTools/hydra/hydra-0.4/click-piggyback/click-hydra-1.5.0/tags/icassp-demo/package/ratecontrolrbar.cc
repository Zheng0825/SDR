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

#include "ratecontrolrbar.hh"
#include "macframertsrbar.hh"
#include "macframectsrbar.hh"

CLICK_DECLS

RateControlRbar::RateControlRbar()
  : _snr(0.0), _rate(54), _rts_frame(0), _cts_frame(0), 
    _snr_vector(RBAR_Rate_Size - 1, 0.0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RateControlRbar::~RateControlRbar()
{
}

void*
RateControlRbar::cast(const char *n)
{
  if( strcmp(n, "RateControlRbar") == 0 )
    return (RateControlRbar *) this;
  else
    return RateControl::cast(n);
}

int
RateControlRbar::configure(Vector<String> &conf, ErrorHandler *errh)
{
  //hydra_chatter(9, 9, "%{element}::configure start", this);
  Element *rts_frame_e = 0;
  Element *cts_frame_e = 0;

  if (cp_va_parse(conf, this, errh,
                  cpElement, "RTS frame which carries snr", &rts_frame_e, 
                  cpElement, "CTS frame which carries rate", &cts_frame_e, 
                  cpIgnore,
 		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }

  if(rts_frame_e) 
  {
    _rts_frame = (MACFrameRTSRbar *) rts_frame_e->cast("MACFrameRTSRbar");
    if(!_rts_frame) 
      return errh->error("MAC frame type must be a MACFrameRTSRbar element");
  } 
  else 
  {
    return errh->error("MACFrame must be specified");
  }

  if(cts_frame_e) 
  {
    _cts_frame = (MACFrameCTSRbar *) cts_frame_e->cast("MACFrameCTSRbar");
    if(!_cts_frame) 
      return errh->error("MAC frame type must be a MACFrameCTSRbar element");
  } 
  else 
  {
    return errh->error("MACFrame must be specified");
  }

  // Get threshold for each rate
  Vector<String> snr_th_vec;
  cp_spacevec(conf[2], snr_th_vec);

  if( snr_th_vec.size() != (RBAR_Rate_Size -1))
  {
    return errh->error("wrong parameter size for snr threshold");
  }

  int snr_th_tmp;
  printf("SNR thresholds: ");
  for(int j=0; j<(RBAR_Rate_Size -1); j++)
  {
    if(!cp_real10(snr_th_vec[j], 3, &snr_th_tmp))
      return errh->error("could not parse snr from arg %d", j);

    _snr_vector[j] = ((float)snr_th_tmp)/1000.0;
    printf("%f ", _snr_vector[j]);
  }
  printf("\n");


  hydra_chatter(9, _verbose, "%{element}::configure", this);

  return 0;
}

Packet *
RateControlRbar::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  struct common_rbar* cmm_hdr = (struct common_rbar *) p_in->data();

  if(is_rts(cmm_hdr->i_fc))
    select_txrate(p_in);
  else if(is_cts(cmm_hdr->i_fc))
    set_txrate(p_in);

  return p_in;
}

void
RateControlRbar::select_txrate(Packet* p_in)
{
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_in->all_user_anno(); 
  uint16_t len;
  _rts_frame->get_feedback(p_in, &len);
  _snr = cha->avg_snr; 
  _rate = calculate_datarate(_snr);

  cha->rate_data_frame = _rate; 

  //hydra_chatter(8, _verbose, "%{element}::select_txrate: snr = %f, rate = %d", this, _snr, _rate);
  printf("RBAR: snr %05.02f, rate = %02d\n", _snr, _rate);

}


void
RateControlRbar::set_txrate(Packet* p_in)
{
  //hydra_chatter(8, _verbose, "%{element}::set_txrate:", this);

  // just setting the snr and calculate at get_txrate
  uint8_t rate_index = 0;
  _cts_frame->get_feedback(p_in, &rate_index);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_in->all_user_anno();
  hydra_chatter(8, _verbose, "%{element}::set_txrate: [%d]", this, rate_index);
  _rate = unconvert_rate(rate_index); 
  cha->rate_data_frame = _rate; 

  hydra_chatter(8, _verbose, "%{element}::set_txrate: %dM", this, _rate);
}


WritablePacket*
RateControlRbar::get_txrate(Packet* p_in)
{
  WritablePacket* p_out = p_in->uniqueify(); 
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();

  hydra_chatter(8, _verbose, "%{element}::get_txrate: _rate = %dM", this, _rate);
 
  cha->rate_mbps = _rate;

  return p_out;
}

unsigned short 
RateControlRbar::calculate_datarate(double snr)
{
/*
  if(snr>29.0)
    return 54;
  else if(snr>27.0)
    return 48;
  else if(snr>19.0)
    return 36;
  else if(snr>17.0)
    return 24;
  else if(snr>14.0)
    return 18;
  else if(snr>12.0)
    return 12;
  else if(snr>11.0)
    return 9;
  else
    return 6;
*/
  if(snr>_snr_vector[0])
    return 54;
  else if(snr>_snr_vector[1])
    return 48;
  else if(snr>_snr_vector[2])
    return 36;
  else if(snr>_snr_vector[3])
    return 24;
  else if(snr>_snr_vector[4])
    return 18;
  else if(snr>_snr_vector[5])
    return 12;
  else if(snr>_snr_vector[6])
    return 9;
  else
    return 6;

}

uint8_t RateControlRbar::convert_rate(uint16_t anno_rate)
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

uint16_t RateControlRbar::unconvert_rate(uint8_t rbar_rate)
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
EXPORT_ELEMENT(RateControlRbar)

