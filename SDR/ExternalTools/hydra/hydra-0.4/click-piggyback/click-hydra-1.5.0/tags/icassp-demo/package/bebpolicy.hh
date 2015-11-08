/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/bebpolicy.hh,v $
 *  
 ***********************************************************/
#ifndef CLICK_BEBPOLICY_HH
#define CLICK_BEBPOLICY_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "collisionpolicy.hh"
CLICK_DECLS

/*
* =c
* 
* BEBPolicy
* 
* =s Hydra
* 
* Binary exponential backoff scheme for collision reducing with ACK retry
* 
* =d
* 
* Description goes here
* 
* Keyword arguments are:
*
* =over 8
*
* =item MAX_RETRY
*
* maximum number of retransmission before dropping packet, default is 7
*
* =item TIMEOUT
*
* Timeout for retransmission
*
* =h verbose read/write
* Enable/disable rts/cts for a packet.
* 
* =a MACTXScheduler */

class BEBPolicy : public CollisionPolicy { 
public:
  
  BEBPolicy();
  ~BEBPolicy();
  
  const char *class_name() const		{ return "BEBPolicy"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  void add_handlers();

  // for retransmission of MACTXScheduler
  void success(const Packet* p);
  void fail(const Packet* p);
  bool retry(const Packet* p);
  void reset_retry(const Packet* p);

  // for control message such as backoff slot number or time slot
  // return packet must be uniquefied.
  Packet* process_control(Packet* p);
  Packet* set_control(Packet* p);

protected:
  static String read_param(Element*, void*);
  static int write_param(const String&, Element*, void*, ErrorHandler*);
  //unsigned int _max_tries;
  //unsigned int _num_tries;

  //int _num_success;
  //int _num_fail;
  //int _num_tx;

  //unsigned _verbose;

protected:
  unsigned int _max_cw;
  unsigned int _min_cw;
  unsigned int _cw;
};

CLICK_ENDDECLS
#endif
