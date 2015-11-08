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
 * 
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>
#include <clicknet/80211.h>
#include <clicknet/ether.h>

#include "broadqueue.hh"

CLICK_DECLS

Broadqueue::Broadqueue()
  : _verbose(0), _plist(0), _qlist(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

Broadqueue::~Broadqueue()
{
}

void*
Broadqueue::cast(const char *n)
{
  if( strcmp(n, "Broadqueue") == 0 )
    return (Broadqueue *) this;
  else if (strcmp(n, Notifier::EMPTY_NOTIFIER) == 0)
    return static_cast<Notifier *>(&_empty_note); 
  else
  return Element::cast(n); 
}

int
Broadqueue::configure(Vector<String> &conf, ErrorHandler *errh)
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
Broadqueue::read_param(Element *e, void *thunk)
{
  Broadqueue *td = (Broadqueue *)e;
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
Broadqueue::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  Broadqueue *f = (Broadqueue *)e;
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
Broadqueue::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

void
Broadqueue::push(int port, Packet *p)
{

  // fifo_list
  _plist.push_back(p);
  _qlist.push_back(p->length()); 

  // Wake up any waiting elements
  if (!_empty_note.active())
    _empty_note.wake();
}

Packet *
Broadqueue::pull(int port)
{
  
  Packet *p_out; 	
  // First, check to see if we have any outstanding packets.
  if (_plist.size() == 0)
  {
    //printf("[%13s][%04d] Sleeping notifiers.\n", __FILE__, __LINE__);
    _empty_note.sleep();
    return NULL;
  }
  else // has packets to send
  {
    p_out = _plist.front();
    _plist.pop_front();
    _qlist.erase(_qlist.begin());  

  }

  return p_out;
}

int 
Broadqueue::queue_status(int max_size)
{
  int size=0;
  int mac_overhead=26;//BYTES  
  int n;  
  for(n=0; n<_qlist.size(); n++)
  {
    size=size+_qlist.at(n)+mac_overhead;
    if(size>=max_size)
       break;   
  }
	
  return n; 
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Broadqueue)
