/*
 * probe.{cc,hh}
 * Wonsoo Kim
 *
 * Copyright (C) 2009  The University of Texas at Austin.
 * Copyright (c) 1999-2003 Massachusetts Institute of Technology
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
 */

#include <click/config.h>
#include <click/confparse.hh>
#include <clicknet/ether.h>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/timer.hh>
#include <click/straccum.hh>
#include <elements/grid/grid.hh>
#include "probe.hh"
#include <elements/grid/timeutils.hh>
CLICK_DECLS

Probe::Probe()
  :_period(1000),_probe_size(1000),_send_timer(0) 
{
}

Probe::~Probe()
{
}

int
Probe::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
			cpKeywords,
			"ETH", cpEtherAddress, "Source Ethernet address", &_eth,
			"PERIOD", cpUnsigned, "Probe broadcast period (msecs)", &_period,
			"SIZE", cpUnsigned, "Probe size (bytes)", &_probe_size,
			cpEnd);
  if (res < 0)
    return res;
 
  //14: src_ether(6)+dst_ether(6)+type(2)
  unsigned min_sz = sizeof(click_ether); 
  
  if (_probe_size < min_sz)
    return errh->error("Specified packet size is less than the minimum probe size of %u", min_sz);

  return res;
}

int
Probe::initialize(ErrorHandler *errh)
{
  if (noutputs() > 0) {
    if (!_eth) 
      return errh->error("Source Ethernet address must be specified to send probes");
    _send_timer = new Timer(static_send_hook, this);
    _send_timer->initialize(this);
    _next_bcast = Timestamp::now() + Timestamp(1, 0);
    _send_timer->schedule_at(_next_bcast);
  }
  return 0;
}

void
Probe::send_hook() 
{
  WritablePacket *p = Packet::make(_probe_size+2); // +2 for alignment
  if (p == 0) {
    click_chatter("Probe %s: cannot make packet!", name().c_str());
    return;
  }
  ASSERT_ALIGNED(p->data());
  p->pull(2);
  memset(p->data(), 0, p->length()); // data is all zeros.

  // fill in ethernet header 
  click_ether *eh = (click_ether *) p->data();
  memset(eh->ether_dhost, 0xff, 6); // broadcast
  eh->ether_type = htons(ETHERTYPE_PROBE); //0x7ffe
  memcpy(eh->ether_shost, _eth.data(), 6); // src ether_addr

  // set next timeout value  
  unsigned max_jitter=_period/10;
  long r2 = random();
  unsigned j = (unsigned) ((r2>>1)%(max_jitter+1));
  _next_bcast += Timestamp::make_usec(1000*((r2 & 1) ? _period-j:_period+j));
  _send_timer->schedule_at(_next_bcast);

  // send probe packet
  output(0).push(p);

  return;
}

/*
void
Probe::push(int port, Packet *p)
{
  // when receiving probe packets, drop it. 
  p->kill();
  return;
}
*/

EXPORT_ELEMENT(Probe)
CLICK_ENDDECLS
