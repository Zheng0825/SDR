/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWRECVCTS_HH
#define CLICK_CLAWRECVCTS_HH

#include <click/element.hh>
#include <clawadaptor.hh>

CLICK_DECLS

class Packet;

class ClawRecvCTS : public Element, public ClawAdaptor {

public:
  ClawRecvCTS();
  ~ClawRecvCTS();

  // From Click
  const char *class_name() const { return "ClawRecvCTS"; }
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

private:
  int remove_snr_int(unsigned int& snr_int);

private:
  WritablePacket* _c_pkt;
  unsigned _verbose;

};

CLICK_ENDDECLS
#endif
