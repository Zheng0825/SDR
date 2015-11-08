/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setdefer.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_SETDEFER_HH
#define CLICK_SETDEFER_HH
#include <click/element.hh>
#include <click/glue.hh>
CLICK_DECLS

/*
=c

SetDefer(Unsigned)

=s Hydra

set defer

=d

=h defer_time read/write

=a 
*/

class SetDefer : public Element { public:
  
  SetDefer();
  ~SetDefer();
  
  const char *class_name() const		{ return "SetDefer"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  inline unsigned defer_time() {return _defer_time;}
  inline unsigned slot_time() {return _slot_time;}
  //inline unsigned tx_processing(){return _tx_processing;}
  //inline unsigned rx_processing() {return _rx_processing;}

  unsigned real_defer(int len);

  void add_handlers();

private:
  unsigned _defer_time; 
  unsigned _slot_time; 
  //unsigned _tx_processing;
  //unsigned _rx_processing;
  unsigned _verbose;

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
