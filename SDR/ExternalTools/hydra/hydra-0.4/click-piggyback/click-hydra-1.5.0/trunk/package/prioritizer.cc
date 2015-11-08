/*********************************************************
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
#include <clicknet/ether.h>

#include "prioritizer.hh"

CLICK_DECLS

Prioritizer::Prioritizer()
  : _verbose(0), _cur_plist(0), _pmap(), _fifo_list()
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

Prioritizer::~Prioritizer()
{
}

void*
Prioritizer::cast(const char *n)
{
  if( strcmp(n, "Prioritizer") == 0 )
    return (Prioritizer *) this;
  else if (strcmp(n, Notifier::EMPTY_NOTIFIER) == 0)
    return static_cast<Notifier *>(&_empty_note);
  else 
    return Element::cast(n);
}

int
Prioritizer::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
 		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  _empty_note.initialize(router());

  hydra_chatter(9, _verbose, "%{element}::configure", this);

  return 0;
}

enum {H_VERBOSE};

String
Prioritizer::read_param(Element *e, void *thunk)
{
  Prioritizer *td = (Prioritizer *)e;
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
Prioritizer::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  Prioritizer *f = (Prioritizer *)e;
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
Prioritizer::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

void
Prioritizer::push(int port, Packet *p)
{
  click_ether *eh = (click_ether *)p->data();
  EtherAddress dst(eh->ether_dhost);

//printf("[%13s][%04d] Pushing: %s\n", __FILE__, __LINE__, dst.unparse().data());

  // If the address-specific list is empty, we need to add it to the 
  // fifo_list
  if (_pmap[dst].size() == 0)
    _fifo_list.push_back(&_pmap[dst]);

  // Push the packet onto the address-specific list
  _pmap[dst].push_back(p);
   num_packets=_pmap[dst].size(); 
//printf("[%13s][%04d] list: 0x%x\n", __FILE__, __LINE__, &_pmap[dst]);
//  printf("[%13s][%04d] plist size: %d\n", __FILE__, __LINE__, _pmap[dst].size());

  // Wake up any waiting elements
  if (!_empty_note.active())
    _empty_note.wake();
}

Packet *
Prioritizer::pull(int port)
{
  //printf("[%13s][%04d] Pulling from prioritizer\n", __FILE__, __LINE__);
  // First, check to see if we have any outstanding packets.
  if (_fifo_list.size() == 0)
  {
    //printf("[%13s][%04d] Sleeping notifiers.\n", __FILE__, __LINE__);
    _empty_note.sleep();
    return NULL;
  }

  // Next, check to see if we should return a prioritzed packet.
  if (_cur_plist == NULL)
    _cur_plist = _fifo_list.front();

  //printf("[%13s][%04d] list: 0x%x\n", __FILE__, __LINE__, _cur_plist);
  //printf("[%13s][%04d] Curlist size: %d\n", __FILE__, __LINE__, _cur_plist->size());

  // If the current list is NULL, return NULL, and rotate to a new list
  if (_cur_plist->size() == 0)
  {
    _fifo_list.pop_front();
    //printf("[%13s][%04d] fifolist size: %d\n", __FILE__, __LINE__, _fifo_list.size());
    if (_fifo_list.size() == 0)
      _empty_note.sleep();

    // schoi +1: Should reset to zero to prevent following lookup 
    _cur_plist = NULL;

    return NULL;
  }

  // Return the oldest packet for the prioritized address, or NULL
  Packet *p_out = _cur_plist->front();
  _cur_plist->pop_front();
  click_ether *eh = (click_ether *)p_out->data();
  EtherAddress dst(eh->ether_dhost);

//printf("[%13s][%04d] Pulling: %s\n", __FILE__, __LINE__, dst.unparse().data());

  return p_out;
}


int 
Prioritizer::queue_size()
{
  return num_packets; 
}



CLICK_ENDDECLS
EXPORT_ELEMENT(Prioritizer)
