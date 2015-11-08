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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setrtsthreshold.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
//#include <click/packet_anno.hh>
//#include <clicknet/ether.h>
//#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "passcsi.hh"

CLICK_DECLS

PassCSI::PassCSI()
    : _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

PassCSI::~PassCSI()
{
}

int
PassCSI::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
 		  "VERBOSE", cpUnsigned, "verbose noise?", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
PassCSI::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if (p) 
  {
    // p is the data out packet.  We also need to go get the CTS-in packet
    // from port 1.
    Packet *cts_in = input(1).pull();
    assert(cts_in);
    uint32_t metadata_length = cts_in->length()-12;
    const unsigned char *metadata = (unsigned char *)(cts_in->data()+12);

    struct click_hydra_anno *data_cha =
                                (struct click_hydra_anno *) p->all_user_anno();

    // Because of the nifty dynamic memory games we need to play, here we copy
    // the data-out aggregation annotation into a temporary structure, and then
    // allocate a new one for it.
    struct click_agg_anno *tmp_agg_anno =
                        (struct click_agg_anno *) malloc(sizeof(*tmp_agg_anno));
    memcpy(tmp_agg_anno, data_cha->agg_anno, sizeof(*tmp_agg_anno));
    //agg_anno_destructor(p);

    // Create the new aggregation field
    agg_anno_allocator(p, metadata_length);
    memcpy(data_cha->agg_anno, tmp_agg_anno, sizeof(*tmp_agg_anno));
    free(tmp_agg_anno);

    // Copy over the new metadata
    data_cha->agg_anno->len_metadata = metadata_length;
    memcpy(data_cha->agg_anno->metadata, metadata, metadata_length);

    // Remove CTS-in packet
    cts_in->kill();
  }
  return p;
}

enum {H_VERBOSE, H_THRESHOLD};

String
PassCSI::read_param(Element *e, void *thunk)
{
  PassCSI *td = (PassCSI *)e;
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
PassCSI::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  PassCSI *f = (PassCSI *)e;
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
PassCSI::add_handlers()
{
  add_read_handler("verbose?", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose?", write_param, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PassCSI)

