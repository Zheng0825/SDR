/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWSENDCTS_HH
#define CLICK_CLAWSENDCTS_HH

#include <click/element.hh>
#include <clawadaptor.hh>

CLICK_DECLS

class Packet;

class ClawSendCTS : public Element, public ClawAdaptor {

public:
  ClawSendCTS();
  ~ClawSendCTS();

  // From Click
  const char *class_name() const { return "ClawSendCTS"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return AGNOSTIC; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  Packet *simple_action(Packet *pkt);

  // From Claw
  int claw_regintr(ClawEvent* event);
  int claw_getinfo(ClawData* data);
  int claw_setinfo(ClawData* data);

protected:

protected:
  unsigned _verbose;

private:
  int add_snr_int(unsigned int snr_int);

private:
  WritablePacket* _c_pkt;

};

CLICK_ENDDECLS
#endif
