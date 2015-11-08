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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setdefer.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "setdefer.hh"

CLICK_DECLS

SetDefer::SetDefer()
  : _defer_time(0), _slot_time(0), 
    //_tx_processing(0), _rx_processing(0), 
    _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

SetDefer::~SetDefer()
{
}

void*
SetDefer::cast(const char *n)
{
  if( strcmp(n, "SetDefer") == 0 )
    return (SetDefer *) this;

  return Element::cast(n);
}

int
SetDefer::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpSecondsAsMicro, "defer time us", &_defer_time,
		  cpSecondsAsMicro, "backoff slot time us", &_slot_time,
		  cpOptional,
		  cpKeywords, 
		  //"TXP", cpSecondsAsMicro, "processing time for tx", &_tx_processing,
		  //"RXP", cpSecondsAsMicro, "processing time for rx", &_rx_processing,
 		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  hydra_chatter(8, _verbose, "%{element}::configure defer = %d", this, _defer_time);

  //unsigned max_processing = (_tx_processing + _rx_processing)*IEEE80211_MTU;
  //if(_defer_time < max_processing)
  //  return errh->error("Defer time is too shorts compare with processing time %d %d", _defer_time, max_processing);

  return 0;
}

Packet *
SetDefer::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);
  if (p) 
  {
    struct click_hydra_anno *cha = (struct click_hydra_anno *) p->all_user_anno();
    cha->magic = HYDRA_MAGIC;
    cha->defer_usec = real_defer(p->length());
    hydra_chatter(8, _verbose, "%{element}::simple_action: slot number = %d each slot_time = %d", this, cha->slot, _slot_time);
    cha->slot *= _slot_time;
  }

  //struct click_hydra_anno* debug = (struct click_hydra_anno*) p->all_user_anno();
  //printf("setdefer: 0x%x\n", debug->mac_type);
  return p;
}

unsigned 
SetDefer::real_defer(int len)
{
  int defer = _defer_time;
  //defer -= (_tx_processing + _rx_processing)*len; 

  hydra_chatter(8, _verbose, "%{element}::real_defer %d for len %d", this, defer, len);

  return (unsigned) defer;
}

enum {H_VERBOSE, H_DEFERTIME, H_SLOTTIME};

String
SetDefer::read_param(Element *e, void *thunk)
{
  SetDefer *td = (SetDefer *)e;
  switch ((uintptr_t) thunk) 
  {
    case H_VERBOSE:
      return String(td->_verbose) + "\n";
    case H_DEFERTIME:
      return String(td->_defer_time) + "\n";
    case H_SLOTTIME:
      return String(td->_slot_time) + "\n";
    default:
      return String();
  }

  return String();

}

int 
SetDefer::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  SetDefer *f = (SetDefer *)e;
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
    case H_DEFERTIME: 
    {
      unsigned m;
      if (!cp_unsigned(s, &m)) 
        return errh->error("stepup parameter must be unsigned");
      f->_defer_time = m;
      break;
    }
    case H_SLOTTIME: 
    {
      unsigned m;
      if (!cp_unsigned(s, &m)) 
        return errh->error("stepup parameter must be unsigned");
      f->_slot_time = m;
      break;
    }
  }

  return 0;

}
void
SetDefer::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
  add_read_handler("defer", read_param, (void *) H_DEFERTIME);
  add_write_handler("defer", write_param, (void *) H_DEFERTIME);
  add_read_handler("slot", read_param, (void *) H_SLOTTIME);
  add_write_handler("slot", write_param, (void *) H_SLOTTIME);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetDefer)

