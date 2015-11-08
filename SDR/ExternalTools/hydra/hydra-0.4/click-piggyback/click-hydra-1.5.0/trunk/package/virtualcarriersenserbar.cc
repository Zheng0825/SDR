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
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>

#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "virtualcarriersenserbar.hh"
#include "setdurationrbar.hh"

CLICK_DECLS

VirtualCarrierSenseRbar::VirtualCarrierSenseRbar() : VirtualCarrierSense(), _duration(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

VirtualCarrierSenseRbar::~VirtualCarrierSenseRbar()
{
}


int
VirtualCarrierSenseRbar::configure(Vector<String> &conf, ErrorHandler *errh)
{
  //hydra_chatter(9, 9, "%{element}::configure start", this);
  Element* duration_e = 0;
  int res = cp_va_parse(conf, this, errh,
                        cpElement, "duration calulator (nav)", &duration_e,
			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return -1;

  if(duration_e) 
  {                                              
    _duration = (SetDurationRbar *) duration_e->cast("SetDuration");
    if(!_duration) 
      return errh->error("Duration calculator must be set using SetDuration");
  } 
  else 
  {
    return errh->error("Duration calculator must be specified");
  }            

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}


void
VirtualCarrierSenseRbar::push(int, Packet *p)
{

  assert(p);

  //struct common_rbar* common_hdr = (struct common_rbar *) p->data();
  //uint32_t duration = *(uint32_t *)common_hdr->i_dur;

  uint32_t duration = _duration->calculate_duration(p); 
  
  hydra_chatter(8, _verbose, "%{element}::push: nav = %d.%06d\n", this, duration/1000000, duration%1000000);
  
  if( duration == 0)
  {
    goto end;
  }

  struct timeval now;
  click_gettimeofday(&now);

  if(_cs_state == CS_IDLE)
  {
    assert(!_timer.busy());

    _cs_state = CS_BUSY;
    _timer.start_timer(duration, now);
    Packet* p_out = prepare_packet(HYDRA_CCA_BUSY);
    output(0).push(p_out);
  }
  else if(_cs_state == CS_BUSY)  
  {
    _timer.pause_timer();

    if( ( (duration/1000000) > (unsigned int)_timer.remains().tv_sec ) || 
        ( (duration/1000000 == (unsigned int)_timer.remains().tv_sec) && 
          (duration%1000000 > (unsigned int)_timer.remains().tv_usec) ) )
    {
      _timer.stop_timer();
      _timer.start_timer(duration, now);
    }
    else
      _timer.resume_timer();
  }

end:
  if(noutputs() > 1)
    output(1).push(p);
  else
    p->kill();

  return;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(VirtualCarrierSenseRbar)
