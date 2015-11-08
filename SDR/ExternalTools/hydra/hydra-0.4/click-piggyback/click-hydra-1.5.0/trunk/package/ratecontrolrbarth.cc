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

#include "macframertsrbar.hh"
#include "macframectsrbar.hh"

#include "ratecontrolrbarth.hh"

CLICK_DECLS

RateControlRbarTh::RateControlRbarTh()
  : _cutoff(0.001), _snr(0.0), _len(0), _rate(6), _rts_frame(0), _cts_frame(0), _is_per(false)
{
  //_cutoff_ber = 0.001;
  //_cutoff_per = 0.05;
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RateControlRbarTh::~RateControlRbarTh()
{
}

void*
RateControlRbarTh::cast(const char *n)
{
  if( strcmp(n, "RateControlRbarTh") == 0 )
    return (RateControlRbarTh *) this;
  else
    return RateControl::cast(n);
}

int
RateControlRbarTh::configure(Vector<String> &conf, ErrorHandler *errh)
{
  //hydra_chatter(9, 9, "%{element}::configure start", this);
  Element *rts_frame_e = 0;
  Element *cts_frame_e = 0;
  int threshold = 0;

  if (cp_va_parse(conf, this, errh,
                  cpElement, "RTS frame which carries snr", &rts_frame_e, 
                  cpElement, "CTS frame which carries rate", &cts_frame_e, 
 		  cpOptional,
		  cpKeywords, 
                  "PER", cpBool, "use PER?", &_is_per, 
                  "THRESHOLD", cpReal10, "decision threshold", 7,  &threshold, 
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

  _cutoff = ((float)threshold) / 10000000.0;
  printf("threshold = %f\n", _cutoff);
  _rate = 54;
  init_tables();

  hydra_chatter(9, _verbose, "%{element}::configure", this);

  return 0;
}

Packet *
RateControlRbarTh::simple_action(Packet *p_in)
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
RateControlRbarTh::select_txrate(Packet* p_in)
{
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_in->all_user_anno(); 
  _snr = cha->avg_snr; 
  _rts_frame->get_feedback(p_in, &_len);
/*
   if(_is_per)
    _rate = calculate_datarate_per(_snr, _len);
   else
    _rate = calculate_datarate_ber(_snr, _len);
*/
  // For VTC paper, selecting rate is hard coded since there is no good SNR estimator. 
  _rate = calculate_datarate_tmp(_snr);

  cha->rate_data_frame = _rate; 
  cha->len_data_frame = _len;

  hydra_chatter(8, _verbose, "%{element}::select_txrate: snr = %f, len = %d, rate = %d", this, _snr, _len, _rate);

}

unsigned short 
RateControlRbarTh::calculate_datarate_tmp(double snr)
{
/*
  if(snr>30.0)
    return 54;
  else if(snr>28.0)
    return 48;
  else if(snr>20.0)
    return 36;
  else if(snr>18.0)
    return 24;
  else if(snr>15.0)
    return 18;
  else if(snr>12.0)
    return 12;
  else if(snr>11.0)
    return 9;
  else
    return 6;
*/
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
}

void
RateControlRbarTh::set_txrate(Packet* p_in)
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
RateControlRbarTh::get_txrate(Packet* p_in)
{
  WritablePacket* p_out = p_in->uniqueify(); 
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();

  hydra_chatter(8, _verbose, "%{element}::get_txrate: _rate = %dM", this, _rate);
 
  // schoi for test -1 +1 
  cha->rate_mbps = _rate;
  //cha->rate_mbps = 9;

  return p_out;
}

uint8_t RateControlRbarTh::convert_rate(uint16_t anno_rate)
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

uint16_t RateControlRbarTh::unconvert_rate(uint8_t rbar_rate)
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

using namespace std;

#define SNR_COUNT 7 
#define MAX_SNR  30
#define RATE_COUNT 8
#define MAX_RATE  54


void RateControlRbarTh::init_tables()
{
  _snr_index.resize(SNR_COUNT, 0);
  _snr_index[0] = 0; _snr_index[1] = 5;
  _snr_index[2] = 10; _snr_index[3] = 15;
  _snr_index[4] = 20; _snr_index[5] = 25;
  _snr_index[6] = 30; 
/*
  _snr_index_rev(MAX_SNR+1, 0)
  _snr_index_rev[0] = 0; _snr_index_rev[5] = 1;
  _snr_index_rev[10] = 2; _snr_index_rev[15] = 3;
  _snr_index_rev[20] = 4; _snr_index_rev[25] = 5;
  _snr_index_rev[30] = 6;
*/
  // init rate index
  _rate_index.resize(RATE_COUNT, 0);
  _rate_index[0] = 6; _rate_index[1] = 9;
  _rate_index[2] = 12; _rate_index[3] = 18;
  _rate_index[4] = 24; _rate_index[5] = 36;
  _rate_index[6] = 48; _rate_index[7] = 54;

  // init rate index rev
/*
  _rate_index_rev(MAX_RATE+1, 0);
  _rate_index_rev[6]  = 0; _rate_index_rev[9]  = 1; 
  _rate_index_rev[12] = 2; _rate_index_rev[18] = 3; 
  _rate_index_rev[24] = 4; _rate_index_rev[36] = 5; 
  _rate_index_rev[48] = 6; _rate_index_rev[54] = 7; 
*/

  //init ber table
  int i;
  _ber.resize(SNR_COUNT*RATE_COUNT, 0.0);
  i=0; // 0 db
  _ber[i*RATE_COUNT + 0] = 2.84e-2; _ber[i*RATE_COUNT + 1] = 7.40e-2;
  _ber[i*RATE_COUNT + 2] = 1.74e-1; _ber[i*RATE_COUNT + 3] = 2.91e-1;
  _ber[i*RATE_COUNT + 4] = 3.47e-1; _ber[i*RATE_COUNT + 5] = 4.75e-1;
  _ber[i*RATE_COUNT + 6] = 4.97e-1; _ber[i*RATE_COUNT + 7] = 5.02e-1;
  i=1; // 5 db
  _ber[i*RATE_COUNT + 0] = 4.68e-3; _ber[i*RATE_COUNT + 1] = 5.63e-2;
  _ber[i*RATE_COUNT + 2] = 1.41e-1; _ber[i*RATE_COUNT + 3] = 2.74e-1;
  _ber[i*RATE_COUNT + 4] = 3.37e-1; _ber[i*RATE_COUNT + 5] = 4.69e-1;
  _ber[i*RATE_COUNT + 6] = 4.95e-1; _ber[i*RATE_COUNT + 7] = 4.97e-1;
  i=2; // 10 db
  _ber[i*RATE_COUNT + 0] = 4.38e-6; _ber[i*RATE_COUNT + 1] = 1.46e-3;
  _ber[i*RATE_COUNT + 2] = 6.82e-2; _ber[i*RATE_COUNT + 3] = 4.33e-3;
  _ber[i*RATE_COUNT + 4] = 1.76e-1; _ber[i*RATE_COUNT + 5] = 4.65e-1;
  _ber[i*RATE_COUNT + 6] = 4.95e-1; _ber[i*RATE_COUNT + 7] = 4.96e-1;
  i=3; // 15 db
  _ber[i*RATE_COUNT + 0] = 0.0; _ber[i*RATE_COUNT + 1] = 7.53e-5;
  _ber[i*RATE_COUNT + 2] = 2.81e-2; _ber[i*RATE_COUNT + 3] = 0.0;
  _ber[i*RATE_COUNT + 4] = 1.48e-4; _ber[i*RATE_COUNT + 5] = 5.86e-2;
  _ber[i*RATE_COUNT + 6] = 4.37e-1; _ber[i*RATE_COUNT + 7] = 4.37e-1;
  i=4; // 20 db
  _ber[i*RATE_COUNT + 0] = 0.0; _ber[i*RATE_COUNT + 1] = 0.0;
  _ber[i*RATE_COUNT + 2] = 1.00e-2; _ber[i*RATE_COUNT + 3] = 0.0;
  _ber[i*RATE_COUNT + 4] = 0.0; _ber[i*RATE_COUNT + 5] = 2.27e-3;
  _ber[i*RATE_COUNT + 6] = 2.63e-2; _ber[i*RATE_COUNT + 7] = 8.38e-2;
  i=5; // 25 db
  _ber[i*RATE_COUNT + 0] = 0.0; _ber[i*RATE_COUNT + 1] = 0.0;
  _ber[i*RATE_COUNT + 2] = 3.37e-3; _ber[i*RATE_COUNT + 3] = 0.0;
  _ber[i*RATE_COUNT + 4] = 0.0; _ber[i*RATE_COUNT + 5] = 0.0;
  _ber[i*RATE_COUNT + 6] = 0.0; _ber[i*RATE_COUNT + 7] = 2.5e-5;
  i=6; // 30 db
  _ber[i*RATE_COUNT + 0] = 0.0; _ber[i*RATE_COUNT + 1] = 0.0;
  _ber[i*RATE_COUNT + 2] = 3.15e-3; _ber[i*RATE_COUNT + 3] = 0.0;
  _ber[i*RATE_COUNT + 4] = 0.00; _ber[i*RATE_COUNT + 5] = 0.0;
  _ber[i*RATE_COUNT + 6] = 0.00; _ber[i*RATE_COUNT + 7] = 0.0;

}

void RateControlRbarTh::interpolate_bers(double snr, vector<double>& bers)
{
  hydra_chatter(9, _verbose, "%{element}:: interpolate: recv snr = %f\n", this, snr);
  if(snr <= (double)_snr_index[0])
  {
    //printf("snr[0] = %f\n", (double)_snr_index[0]);
    for(int i=0; i<RATE_COUNT; i++)
      bers[i] = _ber[i]; 
    return;
  }
  else if(snr >= (double)_snr_index[SNR_COUNT-1])
  {
    //printf("snr[SNR_COUNT-1] = %f\n", (double)_snr_index[SNR_COUNT-1]);
    for(int i=0; i<RATE_COUNT; i++)
      bers[i] = _ber[(SNR_COUNT-1)*RATE_COUNT+i]; 
    return;
  }

   int lower_index = (int)snr/5; 
   //printf("RateControlRbarTh:: interpolate: index = %d, lower_snr = %d\n", lower_index, _snr_index[lower_index]);
   //hydra_chatter(8, _verbose, "%{element}:interpolate_ber: snr = %f, lower_snr = %d\n", this, snr, _snr_index[lower_index]);

   double slope = 0.0;
   double lower_ber;
   double higher_ber;
   //printf("RateControlRbarTh:: interpolate: BERs");
   for(int i=0; i<RATE_COUNT; i++)
   {
     lower_ber = _ber[(lower_index + 0)*RATE_COUNT + i];
     higher_ber = _ber[(lower_index + 1)*RATE_COUNT + i];

     slope = (higher_ber - lower_ber)/5.0;
     bers[i] = lower_ber + slope*(snr - _snr_index[lower_index]);
     hydra_chatter(9, _verbose, "[rate=%d, ber=%f] ", _rate_index[i], bers[i]);
   } 
   hydra_chatter(9, _verbose, "\n");
} 

unsigned short 
RateControlRbarTh::calculate_bers(unsigned int , vector<double>& bers)
{
  unsigned short rate = 6;
  int i = RATE_COUNT-1;

  for(i = RATE_COUNT-1; i>0; i--)
  {
    if(bers[i] <= (double)_cutoff)
    {
      rate = _rate_index[i];
      break;
    }
  }

  printf("snr %f cutoff %f rate %d ber = %f\n", _snr, _cutoff, rate, bers[i]);

  hydra_chatter(9, _verbose, "%{element}:: calculate_bers rate = %d\n", this, rate);
  //hydra_chatter(9, _verbose, "\n");

  //printf("RateControlRbarTh:: cutoff = %f, per = %f, rate = %d\n", cutoff, per, rate);
  //hydra_chatter(8, _verbose, "%{element}:calculate_pers: per = %f rate = %d\n", this, per, rate); 
  return rate; 
}

unsigned short 
RateControlRbarTh::calculate_pers(unsigned int len, vector<double>& bers)
{
  unsigned short rate = 6;
  double per = 1.0;
  double bsr = 1.0;

  for(int i = RATE_COUNT-1; i>0; i--)
  {
    bsr = 1.0 - bers[i];
    per = 1.0;
    // FIXME: need to change to simple calculation such as taylor series
    for(unsigned int j=0; j<len; j++)
      per *= bsr; 
    per = 1 - per;

    hydra_chatter(9, _verbose, "[rate=%d, per=%f]", _rate_index[i], per);
    if(per <= (double) _cutoff)
    {
      rate = _rate_index[i];
      break;
    }
  }

  hydra_chatter(9, _verbose, "%{element}:: calculate_pers rate = %d\n", this, rate);

  //printf("RateControlRbarTh:: cutoff = %f, per = %f, rate = %d\n", cutoff, per, rate);
  //hydra_chatter(8, _verbose, "%{element}:calculate_pers: per = %f rate = %d\n", this, per, rate); 
  return rate; 
}
 
unsigned short 
RateControlRbarTh::calculate_datarate_ber(double snr, unsigned int data_length)
{
  vector<double> error_rates(RATE_COUNT); 

  interpolate_bers(snr, error_rates);

  return calculate_bers(data_length, error_rates);
}

unsigned short 
RateControlRbarTh::calculate_datarate_per(double snr, unsigned int data_length)
{
  vector<double> error_rates(RATE_COUNT); 

  interpolate_bers(snr, error_rates);

  return calculate_pers(data_length, error_rates);
}
/*
enum {H_VERBOSE};

String
RateControlRbarTh::read_handler(Element *e, void *thunk)
{
  RateControlRbarTh *src = (RateControlRbarTh *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
RateControlRbarTh::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  RateControlRbarTh *src = (RateControlRbarTh *) e;
  String s = cp_uncomment(arg);
  switch((int)vparam) {
  case H_VERBOSE: {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("verbose parameter must be unsigned");
    src->_verbose = m;
    break;
  }
  }
  return 0;
}

void
RateControlRbarTh::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}
*/

CLICK_ENDDECLS
EXPORT_ELEMENT(RateControlRbarTh)

