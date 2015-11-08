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
#include <stddef.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <click/standard/scheduleinfo.hh>
#include <click/router.hh>
#include <click/element.hh>
#include <click/glue.hh>
#include <click/straccum.hh>
#include <click/packet_anno.hh>
#include "staticroutetable.hh"

//#include <elements/grid/dsdvroutetable.hh>
//#include <elements/grid/linkstat.hh>
//#include <elements/grid/gridgatewayinfo.hh>
//#include <elements/grid/timeutils.hh>
//#include <elements/wifi/txfeedbackstats.hh>


CLICK_DECLS

StaticRouteTable::StaticRouteTable() : _verbose(8)
{
}

StaticRouteTable::~StaticRouteTable()
{
}

void * StaticRouteTable::cast(const char *n)
{
  if (strcmp(n, "StaticRouteTable") == 0)
    return (StaticRouteTable *) this;
  else if (strcmp(n, "GridGenericRouteTable") == 0)
    return (GridGenericRouteTable *) this;
  else
    return 0;
}

int StaticRouteTable::configure(Vector<String> &conf, ErrorHandler *errh)
{
  //int before = errh->nerrors();
/*
  int res = cp_va_parse(conf, this, errh,
			cpEthernetAddress, "source Ethernet address", &_eth,
			cpIPAddress, "source IP address", &_ip,
			cpUnsigned, "verbose", &_verbose,
			cpEnd);

  if (res < 0)
    errh->error("argument should be started with`MY_IP MY_ETH VERBOSE");
*/

  int res = -1;

  for (int i = 0; i < conf.size(); i++) 
  {
    Vector<String> words;
    cp_spacevec(conf[i], words);

    IPAddress dest_ip;
    EtherAddress next_hop_eth; 
    IPAddress next_hop_ip; 
    unsigned num_hops;

    if( i == 0 && words.size() == 2)
    {
      if( cp_ethernet_address(words[0], &_eth, this) && 
          cp_ip_address(words[1], &_ip, this) ) 
          continue;
     
      errh->error("argument %d should be `EtherAddress(MY_ETH) IPAddress(MY_IP) not %s %s", i+1, words[0].c_str(), words[1].c_str());
      return res;
    } 
    else if( i == 1 && words.size() == 1)
    {
      if( cp_unsigned(words[0], &_verbose) )
      {
        res = 0;
        continue;
      }
      errh->error("argument %d should be Unsigned(VERBOSE)", i+1);
      return res;
    }
    else if( i>1 && words.size() == 4)
    {
      //if(cp_ip_address(words[0], &dst_ip, this);  
      if( cp_ip_address(words[0], &dest_ip, this) && 
          cp_ethernet_address(words[1], &next_hop_eth, this) && 
          cp_ip_address(words[2], &next_hop_ip, this) && 
          cp_unsigned(words[3], &num_hops) )
      {
        if (num_hops <= 0xff && num_hops >0) 
        {
          insert_route(dest_ip, next_hop_eth, next_hop_ip, (unsigned char) num_hops);
          continue;
        }
      }

      errh->error("argument %d should be `DST_IP NEXT_ETH NEXT_IP HOP(1~255)'", i+1);
      return res;
    }                                                     
                                                              
  }                                                         
                                                              
  if (_rtes.size() == 0)
    errh->warning("No static route is defined. All packets will be dropped");
  else
    click_chatter("%s", print_rtes(this, 0).c_str());
  return res;
}

void StaticRouteTable::insert_route(IPAddress& dest_ip, EtherAddress& next_hop_eth, IPAddress& next_hop_ip, unsigned char num_hops)
{
  if(_rtes.findp(dest_ip))
    _rtes.remove(dest_ip);

  grid_location dummy;

  RouteEntry rte(dest_ip, false, (unsigned short)0, dummy, next_hop_eth, next_hop_ip, (unsigned char)0, (unsigned int)0, num_hops);

  _rtes.insert(dest_ip, rte);
}

bool StaticRouteTable::lookup_route(const IPAddress &dest_ip, RouteEntry &rte) 
{

  RouteEntry *found_rte = _rtes.findp(dest_ip);
  if (found_rte == 0)
    return false;

  rte = *found_rte;
  return true;  
}

bool StaticRouteTable::get_one_entry(const IPAddress &dest_ip, RouteEntry &rte)
{
  return lookup_route(dest_ip, rte);
}


void StaticRouteTable::get_all_entries(Vector<RouteEntry> &vec)
{

  for(RTIter iter = _rtes.begin(); iter; iter++)
  {
    RouteEntry rte = iter.value();
    vec.push_back(rte);
  }

  return;
}

unsigned StaticRouteTable::get_number_direct_neigbors()
{
  Vector<RouteEntry> rtev;
  
  get_all_entries(rtev);

  int num_nbrs = 0;
  for (int i = 0; i < rtev.size(); i++)
    if (rtev[i].num_hops() == 1 && rtev[i].good())
      num_nbrs++;

  return num_nbrs;
}

Packet* StaticRouteTable::simple_action(Packet *packet)
{
  // just forward 
  return packet;
}


void StaticRouteTable::add_handlers()
{
  add_read_handler("rtes", print_rtes, 0);
  add_read_handler("ip", print_ip, 0);
  add_read_handler("eth", print_eth, 0);
  add_read_handler("verbose", read_verbose, (void *) 0);
  add_write_handler("verbose", write_verbose, (void *) 0);
}


String StaticRouteTable::print_rtes(Element *e, void *)
{
  StaticRouteTable *n = (StaticRouteTable *) e;

  String s;
  for (RTIter i = n->_rtes.begin(); i; i++) 
  {
    RouteEntry f = i.value();
    s += f.dest_ip.s() 
      + " next_ip=" + f.next_hop_ip.s() 
      + " next_eth=" + f.next_hop_eth.s() 
      + " hops=" + String((int) f.num_hops()) 
      + "\n";
  }
  
  return s;
}

String StaticRouteTable::print_ip(Element *e, void *)
{
  StaticRouteTable *n = (StaticRouteTable *) e;
  return n->_ip.s();
}

String StaticRouteTable::print_eth(Element *e, void *)
{
  StaticRouteTable *n = (StaticRouteTable *) e;
  return n->_eth.s();
}

String StaticRouteTable::read_verbose(Element *e, void *)
{
  StaticRouteTable *n = (StaticRouteTable *) e;

  return String(n->_verbose);
}

int StaticRouteTable::write_verbose(const String &in_s, Element *e, void *, ErrorHandler *errh)
{
  StaticRouteTable *n = (StaticRouteTable *) e;
  String s = cp_uncomment(in_s);

  unsigned verbose;
  if(!cp_unsigned(s, &verbose))
    return errh->error("verbose must be boolean");
  n->_verbose = verbose;
  return 0;
}

ELEMENT_PROVIDES(GridGenericRouteTable)
EXPORT_ELEMENT(StaticRouteTable)

#include <click/bighashmap.cc>
#include <click/vector.cc>
template class HashMap<IPAddress, StaticRouteTable::RouteEntry>;
template class Vector<StaticRouteTable::RouteEntry>;

CLICK_ENDDECLS
