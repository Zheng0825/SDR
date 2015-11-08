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
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/simchannel.hh,v $ 
 *  
 ***********************************************************/
#ifndef CLICK_SIMCHANNEL_HH
#define CLICK_SIMCHANNEL_HH

/*
 * =c
 * SimChannel([keywords,] [lat1 lon1, lat2 lon2, ...])
 * =s Grid
 * simulates reachability and broadcast in an 802.11-like radio network
 * =d
 * SimChannel simulates reachability and broadcast in an 802.11-like
 * radio network.
 *
 * Each corresponding input/output pair corresponds to one node.
 * Each node has a latitude/longitude, given by the <i>th
 * configuration argument.
 *
 * When node <i> sends a packet into SimChannel's input <i>,
 * SimChannel sends a copy to each output whose node is
 * within 250 meters of node <i>.
 *
 * Inputs are pull, outputs are push. Services inputs in round
 * robin order.
 *
 * Keyword:
 *
 * =over 8
 * 
 * =item USE_XY
 *
 * Boolean.  Defaults to false.  Use x,y coordinates in metres instead
 * of lat,lon in degrees.  lat is treated as x, and lon is treated as
 * y.
 *
 * =back
 *
 * The loc read/write handler format is
 *   node-index latitude longitude */

#include <click/element.hh>
#include <click/vector.hh>

CLICK_DECLS

class SimChannel : public Element {
  
 public:
  
  SimChannel();
  ~SimChannel();
  
  const char *class_name() const		{ return "SimChannel"; }
  const char *port_count() const		{ return "-/-"; }
  const char *processing() const		{ return PUSH; }
  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *errh);
  void add_handlers();

  void push(int port, Packet* p);

  enum {RECV_DATA, RECV_CS};
protected:

  struct Node {
    double _lat;
    double _lon;
    Node(double la, double lo) : _lat(la), _lon(lo) { }
    Node() : _lat(0), _lon(0) { }
  };

  Node get_node_loc(int i);
  void set_node_loc(int i, double lat, double lon);
  int nnodes() { return(_nodes.size()); }

  static int rs_write_handler(const String &, Element *, void *, ErrorHandler *);
  static String rs_read_handler(Element *, void *);

  Vector<Node> _nodes;
 
  int _tx_range; 
  int _cs_range; 
};

CLICK_ENDDECLS
#endif
