/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
#ifndef CLICK_STATICROUTETABLE_HH
#define CLICK_STATICROUTETABLE_HH
#include <click/bighashmap.hh>
#include <click/dequeue.hh>
#include <click/etheraddress.hh>
#include <click/ipaddress.hh>
#include <clicknet/ether.h>
#include <elements/grid/grid.hh>
#include <elements/grid/gridgenericrt.hh>

//#include <click/timer.hh>
//#include <elements/grid/gridgenericmetric.hh>
//#include <elements/grid/gridgenericlogger.hh>
CLICK_DECLS

/*
struct StaticRouteEntry {
public:
  StaticRouteEntry(const IPAddress &dst, const EtherAddress &nhe, 
                   const IPAddress &nhi, unsigned char nh) :
    _dest_ip(dst), _next_hop_eth(nhe), _next_hop_ip(nhi), _num_hops(nh) { }

  StaticRouteEntry() :  _num_hops(0) { }
  
public:
  class IPAddress      _dest_ip;      		// IP address of this destination
  class EtherAddress   _next_hop_eth; 		// hardware address of next hop
  class IPAddress      _next_hop_ip;  		// IP address of next hop
  unsigned char        _num_hops;
};
*/

class StaticRouteTable : public GridGenericRouteTable {

public:
  StaticRouteTable();
  ~StaticRouteTable();

  const char *class_name() const		{ return "StaticRouteTable"; }
  void *cast(const char *);
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return "h/h"; }
  const char *flow_code() const                 { return "x/y"; }
  
  int configure(Vector<String> &, ErrorHandler *);
  //int initialize(ErrorHandler *);

  Packet *simple_action(Packet *);

  void add_handlers();

  // generic rt methods
  bool current_gateway(RouteEntry&) {return false;} 
  // return false if there is no entry for this dest, else fill in entry
  bool get_one_entry(const IPAddress &dest_ip, RouteEntry &entry);
  // append all the current route entries to vec.  You should clear
  // vec before calling this method, if desired.
  void get_all_entries(Vector<RouteEntry> &vec);
  // return the number of neighbors we can hear from directly.  This
  // may be larger than (but never less than) the number of nodes with
  // 1-hop route, because some neighbors aren't their own best next
  // hop.
  unsigned get_number_direct_neigbors(); 

private:  
  void insert_route(IPAddress& dest_ip, EtherAddress& next_hop_eth, IPAddress& next_hop_ip, unsigned char num_hops);
  bool lookup_route(const IPAddress &dest_ip, RouteEntry &rte); 

  static String print_rtes(Element *e, void *);
  static String print_ip(Element *e, void *);
  static String print_eth(Element *e, void *);
  static String read_verbose(Element *e, void *);
  static int write_verbose(const String &in_s, Element *e, void *, ErrorHandler *errh);

private:
  EtherAddress _eth;
  IPAddress _ip;
  unsigned _verbose;

  typedef HashMap<IPAddress, RouteEntry> RTable;
  typedef RTable::const_iterator RTIter;

  RTable _rtes;

};


CLICK_ENDDECLS
#endif
