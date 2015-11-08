/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWRECVRTS_HH
#define CLICK_CLAWRECVRTS_HH

#include <click/element.hh>
#include <clawadaptor.hh>

CLICK_DECLS

class Packet;

class ClawRecvRTS : public Element, public ClawAdaptor {

public:
  ClawRecvRTS();
  ~ClawRecvRTS();

  // From Click
  const char *class_name() const { return "ClawRecvRTS"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return AGNOSTIC; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  Packet *simple_action(Packet *pkt);

  // From Claw
  //int claw_regintr(ClawEvent* event);
  int claw_getinfo(ClawData* data);
  int claw_setinfo(ClawData* data);

protected:

protected:
  unsigned _verbose;
private:
  WritablePacket* _c_pkt;

};

CLICK_ENDDECLS
#endif
