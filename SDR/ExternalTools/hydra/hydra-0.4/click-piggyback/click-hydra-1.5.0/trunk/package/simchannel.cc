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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/simchannel.cc,v $ 
 *  
 ***********************************************************/
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/packet_anno.hh>
#include <math.h>
//#include "elements/grid/filterbyrange.hh"
#include <click/router.hh>

#include "simchannel.hh"
CLICK_DECLS

SimChannel::SimChannel()
  : _tx_range(200), _cs_range(400)
{
}

SimChannel::~SimChannel()
{
}

int
SimChannel::configure(Vector<String> &conf, ErrorHandler *errh)
{
  // test router
/* 
  int num_e = router()->nelements();
  Vector<Element*> e_vec = router()->elements();
  click_chatter("this = 0x%x, e_vec = 0x%x", this, e_vec[eindex()]);
  click_chatter("+++++++++++++++++++++++++++++++++++");
  click_chatter("nelement = %d", num_e);
  click_chatter("+++++++++++++++++++++++++++++++++++");
  click_chatter("ename = %s", router()->ename(eindex()).data());
  click_chatter("+++++++++++++++++++++++++++++++++++");
  click_chatter("elandmark = %s", router()->elandmark(eindex()).data());
  click_chatter("+++++++++++++++++++++++++++++++++++");
  click_chatter("configuration = %s", router()->default_configuration_string(eindex()).data());
  click_chatter("+++++++++++++++++++++++++++++++++++");
*/

  int i;

  _nodes.clear();
  for(i = 0; i < conf.size(); i++)
  {
    Vector<String> words;
    cp_spacevec(conf[i], words);
    if(words.size() != 2)
      return errh->error("argument %d doesn't have lat and lon", i);
    int xlat, xlon;
    if(!cp_real10(words[0], 5, &xlat) || !cp_real10(words[1], 5, &xlon))
      return errh->error("could not parse lat or lon from arg %d", i);
    Node n;
    n._lat = ((double)xlat) / 100000.0;
    n._lon = ((double)xlon) / 100000.0;
    _nodes.push_back(n);
  }

  click_chatter("%{element}: tx_range = %dm, cs_range = %dm\n", this, _tx_range, _cs_range);
  return 0;
}


int
SimChannel::initialize(ErrorHandler *)
{
  int n, i;
  
  n = ninputs();
  assert(n == noutputs());
  for(i = _nodes.size(); i < n; i++){
    Node no;
    no._lat = 0;
    no._lon = 0;
    _nodes.push_back(no);
  }

  click_chatter("%{element}: SimChannel started..", this);
  return 0;
}

void
SimChannel::push(int port, Packet* p)
{
  assert(p);
  int in = port;

  for(int out = 0; out < noutputs(); out++)
  {
    if( in == out)
      continue;

    double r;
    double dx = _nodes[in]._lat - _nodes[out]._lat;
    double dy = _nodes[in]._lon - _nodes[out]._lon;
    r = sqrt(dx*dx + dy*dy);

    if (r < _tx_range)
    {
      //click_chatter("%{element}:process_channel: in TX_range", this);
      Packet* p_out = p->clone();
      SET_PAINT_ANNO(p_out, RECV_DATA);
      output(out).push(p_out);
    }
    else if (r < _cs_range)
    {
      //click_chatter("%{element}:process_channel: in CS_range", this);
      Packet* p_out = p->clone();
      SET_PAINT_ANNO(p_out, RECV_CS);
      output(out).push(p_out);
    }
  }
  p->kill();

  return;
}

SimChannel::Node
SimChannel::get_node_loc(int i)
{
  if(i >= 0 && i < _nodes.size())
    return _nodes[i];
  else 
    return Node(0, 0);
}

void
SimChannel::set_node_loc(int i, double lat, double lon)
{
  if(i >= 0 && i < _nodes.size()){
    _nodes[i]._lat = lat;
    _nodes[i]._lon = lon;
  }
}

// Expects a line that looks like
// node-index latitude longitude
int
SimChannel::rs_write_handler(const String &arg, Element *element,
			   void *, ErrorHandler *errh)
{
  SimChannel *l = (SimChannel *) element;
  Vector<String> words;
  cp_spacevec(arg, words);
  int xi, xlat, xlon;
  if(words.size() != 3 ||
     !cp_integer(words[0], 10, &xi) ||
     !cp_real10(words[1], 5, &xlat) ||
     !cp_real10(words[2], 5, &xlon))
    return errh->error("%s: expecting node-index lat lon", l->name().c_str());
  if(xi >= 0 && xi < l->nnodes()){
    double lat = ((double)xlat) / 100000.0;
    double lon = ((double)xlon) / 100000.0;
    l->set_node_loc(xi, lat, lon);
    return(0);
  } else {
    return errh->error("%s: illegal index %d", l->name().c_str(), xi);
  }
}

String
SimChannel::rs_read_handler(Element *f, void *)
{
  SimChannel *l = (SimChannel *) f;
  String s;
  int i, n;

  n = l->nnodes();
  for(i = 0; i < n; i++){
    Node n = l->get_node_loc(i);
    const int BUFSZ = 255;
    char buf[BUFSZ];
    snprintf(buf, BUFSZ, "%d %f %f\n", i, n._lat, n._lon);
    s += buf;
  }
  return s;
}

void
SimChannel::add_handlers()
{
  add_write_handler("loc", rs_write_handler, (void *) 0);
  add_read_handler("loc", rs_read_handler, (void *) 0);
}

//ELEMENT_REQUIRES(FilterByRange)
EXPORT_ELEMENT(SimChannel)

#include <click/vector.cc>
template class Vector<SimChannel::Node>;
CLICK_ENDDECLS
