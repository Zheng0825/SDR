/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWRECVRTS_HH
#define CLICK_CLAWRECVRTS_HH

#include <click/element.hh>
#include <claw/claw_adaptor.h>

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
  const char* claw_layer() const { return "MAC"; }
  const char* claw_type() const { return "Interceptor"; }
  const char* claw_name() const { return "RecvRTS"; }
  int claw_regintr(int intr_type, char* intrval, int intrlen);
  int claw_getinfo(const char* infoname, void* infoval, int* infolen);
  int claw_setinfo(const char* infoname, void* infoval, int* infolen);

protected:
  // void claw_set_conn(void* conn);
  // void claw_notify_conn(void* conn);

protected:
  // ClawConnProtModule* _c_conn;
  unsigned _verbose;
private:
  WritablePacket* _c_pkt;

};

CLICK_ENDDECLS
#endif
