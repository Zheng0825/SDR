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
#include <click/packet_anno.hh>
//#include <clicknet/ether.h>
//#include <click/etheraddress.hh>

#include <click/hydra_anno.hh>
#include <clicknet/80211.h>
#include "bebpolicyarf.hh"
#include "ratecontrolarf.hh"

CLICK_DECLS

BEBPolicyArf::BEBPolicyArf() : _ratecontrol(0)
//  : _max_cw(0xFFFFFFFF), _min_cw(1)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

BEBPolicyArf::~BEBPolicyArf()
{
}

void*
BEBPolicyArf::cast(const char *n)
{
    if (strcmp(n, "BEBPolicyArf") == 0)
	return (BEBPolicyArf *) this;

    return BEBPolicy::cast(n);
}

int
BEBPolicyArf::configure(Vector<String> &conf, ErrorHandler *errh)
{
  Element *rc_e = 0; 

  if (cp_va_parse(conf, this, errh,
 		  cpElement, "ARF rate control", &rc_e,
		  cpOptional,
		  cpKeywords, 
 		  "MAX_RETRIES", cpUnsigned, "max retry number", &_max_tries,
 		  "MAX_CW", cpUnsigned, "max contention window", &_max_cw,
 		  "MIN_CW", cpUnsigned, "min contention window", &_min_cw,
		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }


  if(rc_e) 
  {
    _ratecontrol = (RateControlArf *) rc_e->cast("RateControlArf");
    if(!_ratecontrol) 
      return errh->error("Rate control should be ARF");
  } 
  else 
  {
    return errh->error("Rate control must be specified");
  }


  _cw = _min_cw;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

void 
BEBPolicyArf::success(const Packet* p)
{
  BEBPolicy::success(p);

  struct fc_80211* fc = (struct fc_80211 *) p->data();
  if(is_data(*fc))
  {
    _ratecontrol->success();
  }
}

void 
BEBPolicyArf::fail(const Packet* p)
{
  BEBPolicy::fail(p);
  struct fc_80211* fc = (struct fc_80211 *) p->data();
  if(is_data(*fc))
  {
    _ratecontrol->fail();
  }
}


CLICK_ENDDECLS
EXPORT_ELEMENT(BEBPolicyArf)
