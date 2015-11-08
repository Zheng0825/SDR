/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setrtsthreshold.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_SETRTSTHRESHOLD_HH
#define CLICK_SETRTSTHRESHOLD_HH
#include <click/element.hh>
#include <click/glue.hh>
CLICK_DECLS

/*
=c

SetRTSThreshold(Unsigned)

=s Hydra

Enable/disable RTS/CTS for a packet

=d

Enable/disable RTS/CTS for a packet

=h rts read/write
Enable/disable rts/cts for a packet.

=a 
*/

class SetRTSThreshold : public Element { public:
  
  SetRTSThreshold();
  ~SetRTSThreshold();
  
  const char *class_name() const		{ return "SetRTSThreshold"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  void add_handlers();

private:
  unsigned _threshold; 
  unsigned _verbose;

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
