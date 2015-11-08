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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/phyprocesstime.cc,v $ 
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>

#include "phyprocesstime.hh"

CLICK_DECLS

PHYProcessTime::PHYProcessTime()
  : _tx_delay(0), _rx_delay(0), _mac_delay(0),
    _txe_slope(0), _txe_inter(0), _rxd_slope(0), _rxd_inter(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

PHYProcessTime::~PHYProcessTime()
{
}

void*
PHYProcessTime::cast(const char *n)
{
  if( strcmp(n, "PHYProcessTime") == 0 )
    return (PHYProcessTime *) this;

  return Element::cast(n);
}

int
PHYProcessTime::configure(Vector<String> &conf, ErrorHandler *errh)
{
  // Set constant delay in PHY/MAC layers. 
  if (cp_va_parse(conf, this, errh,
		  cpSecondsAsMicro, "Delay in TX processing", &_tx_delay,
		  cpSecondsAsMicro, "Delay in RX processing", &_rx_delay,
		  cpSecondsAsMicro, "Delay in MAC processing", &_mac_delay,
  		  cpIgnore, 
		  cpIgnore,
		  cpOptional,
		  cpKeywords,
 		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  Vector<String> txe_vec;
  Vector<String> rxd_vec;
 
  cp_spacevec(conf[3], txe_vec);
  cp_spacevec(conf[4], rxd_vec);
 
  if( (txe_vec.size() != 2) || (rxd_vec.size() != 2))
  {
    return errh->error("wrong parameter for slope and intercept");
  }

  // Get parameters for TX encoding time at PHY layer
  // It's composed of two parameters, 
  // tx_encoding_time = txe slope * (number of bytes of MAC frame) + txe intercept
  cp_unsigned(txe_vec[0], &_txe_slope);
  cp_unsigned(txe_vec[1], &_txe_inter);

  // Get parameters for RX decoding time at PHY layer
  cp_unsigned(rxd_vec[0], &_rxd_slope);
  cp_unsigned(rxd_vec[1], &_rxd_inter);

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
PHYProcessTime::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);
/*
  if (p) 
  {
    int len = p->length();
    struct click_hydra_anno *cha = (struct click_hydra_anno *) p->all_user_anno();
    cha->magic = HYDRA_MAGIC;

    _tx_delay + (_txe_slope * len + _txe_inter) + _mac_delay 
    _rx_delay + (_rxd_slope * len + _rxd_inter);
  }
*/
  return p;
}


enum {H_VERBOSE, H_TXDELAY, H_MACDELAY, H_TXSLOPE, H_TXINTER, H_RXDELAY, H_RXSLOPE, H_RXINTER};

String
PHYProcessTime::read_param(Element *e, void *thunk)
{
  PHYProcessTime *td = (PHYProcessTime *)e;
  switch ((uintptr_t) thunk) 
  {
    case H_VERBOSE:
      return String(td->_verbose) + "\n";
    case H_TXDELAY:
      return String(td->_tx_delay/1000000) + "\n";
    case H_TXSLOPE:
      return String(td->_txe_slope/1000000) + "\n";
    case H_TXINTER:
      return String(td->_txe_inter/1000000) + "\n";
    case H_RXDELAY:
      return String(td->_rx_delay/1000000) + "\n";
    case H_RXSLOPE:
      return String(td->_rxd_slope/1000000) + "\n";
    case H_RXINTER:
      return String(td->_rxd_inter/1000000) + "\n";
    case H_MACDELAY:
      return String(td->_mac_delay/1000000) + "\n";
    //default:
      //return String();
  }

  return String();

}

int 
PHYProcessTime::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  PHYProcessTime *f = (PHYProcessTime *)e;
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
    case H_TXDELAY: 
    {
      unsigned m;
      if (!cp_seconds_as(6, s, &m)) 
        return errh->error("stepup parameter must be seconds");
      f->_tx_delay = m;
      break;
    }
    case H_TXSLOPE: 
    {
      unsigned m;
      if (!cp_seconds_as(6, s, &m)) 
        return errh->error("stepup parameter must be seconds");
      f->_txe_slope = m;
      break;
    }
    case H_TXINTER: 
    {
      unsigned m;
      if (!cp_seconds_as(6, s, &m)) 
        return errh->error("stepup parameter must be seconds");
      f->_txe_inter = m;
      break;
    }
    case H_RXDELAY: 
    {
      unsigned m;
      if (!cp_seconds_as(6, s, &m)) 
        return errh->error("stepup parameter must be seconds");
      f->_rx_delay = m;
      break;
    }
    case H_RXSLOPE: 
    {
      unsigned m;
      if (!cp_seconds_as(6, s, &m)) 
        return errh->error("stepup parameter must be seconds");
      f->_rxd_slope = m;
      break;
    }
    case H_RXINTER: 
    {
      unsigned m;
      if (!cp_seconds_as(6, s, &m)) 
        return errh->error("stepup parameter must be seconds");
      f->_rxd_inter = m;
      break;
    }
    case H_MACDELAY: 
    {
      unsigned m;
      if (!cp_seconds_as(6, s, &m)) 
        return errh->error("stepup parameter must be seconds");
      f->_mac_delay = m;
      break;
    }
  }

  return 0;

}
void
PHYProcessTime::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
  add_read_handler("tx_delay", read_param, (void *) H_TXDELAY);
  add_write_handler("tx_delay", write_param, (void *) H_TXDELAY);
  add_read_handler("tx_slope", read_param, (void *) H_TXSLOPE);
  add_write_handler("tx_slope", write_param, (void *) H_TXSLOPE);
  add_read_handler("tx_inter", read_param, (void *) H_TXINTER);
  add_write_handler("tx_inter", write_param, (void *) H_TXINTER);
  add_read_handler("rx_delay", read_param, (void *) H_RXDELAY);
  add_write_handler("rx_delay", write_param, (void *) H_RXDELAY);
  add_read_handler("rx_slope", read_param, (void *) H_RXSLOPE);
  add_write_handler("rx_slope", write_param, (void *) H_RXSLOPE);
  add_read_handler("rx_inter", read_param, (void *) H_RXINTER);
  add_write_handler("rx_inter", write_param, (void *) H_RXINTER);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PHYProcessTime)

