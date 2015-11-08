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

#include "ratecontroltbar.hh"

CLICK_DECLS

RateControlTbar::RateControlTbar()
  : _snr(0.0), _rate(54), 
    _snr_vector(TBAR_Rate_Size - 1, 0.0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RateControlTbar::~RateControlTbar()
{
}

void*
RateControlTbar::cast(const char *n)
{
  if( strcmp(n, "RateControlTbar") == 0 )
    return (RateControlTbar *) this;
  else
    return RateControl::cast(n);
}

int
RateControlTbar::configure(Vector<String> &conf, ErrorHandler *errh)
{
  //hydra_chatter(9, 9, "%{element}::configure start", this);

  if (cp_va_parse(conf, this, errh,
                  cpIgnore,
 		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }

  // Get threshold for each rate
  Vector<String> snr_th_vec;
  cp_spacevec(conf[0], snr_th_vec);

  if( snr_th_vec.size() != (TBAR_Rate_Size -1))
  {
    return errh->error("wrong parameter size for snr threshold");
  }

  int snr_th_tmp;
  printf("SNR thresholds: ");
  for(int j=0; j<(TBAR_Rate_Size -1); j++)
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
RateControlTbar::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  select_txrate(p_in);

  return p_in;
}

void
RateControlTbar::select_txrate(Packet* p_in)
{
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_in->all_user_anno(); 
  _snr = cha->avg_snr; 
  _rate = calculate_datarate(_snr);

  cha->rate_data_frame = _rate; 

  //hydra_chatter(8, _verbose, "%{element}::select_txrate: snr = %f, rate = %d", this, _snr, _rate);
  printf("TBAR: snr %05.02f, rate = %02d\n", _snr, _rate);

}


WritablePacket*
RateControlTbar::get_txrate(Packet* p_in)
{
  WritablePacket* p_out = p_in->uniqueify(); 
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();

  hydra_chatter(8, _verbose, "%{element}::get_txrate: _rate = %dM", this, _rate);
 
  cha->rate_mbps = _rate;

  return p_out;
}

unsigned short 
RateControlTbar::calculate_datarate(double snr)
{
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



CLICK_ENDDECLS
EXPORT_ELEMENT(RateControlTbar)

