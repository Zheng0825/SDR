/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWLOADMONITOR_HH
#define CLICK_CLAWLOADMONITOR_HH

#include <click/element.hh>
#include <claw/claw_adaptor.h>
#include "loadmonitor.hh"

CLICK_DECLS

class ClawLoadMonitor : public LoadMonitor, public ClawAdaptor {

public:
  ClawLoadMonitor();
  ~ClawLoadMonitor();

  // From Click
  const char *class_name() const { return "ClawLoadMonitor"; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);

  // From Claw
  const char* claw_layer() const { return "MAC"; }
  const char* claw_type() const { return "Adaptor"; }
  const char* claw_name() const { return "LoadMonitor"; }
  //int claw_regintr(int intr_type, char* intrval, int intrlen);
  int claw_getinfo(const char* infoname, void* infoval, int *infolen);
  int claw_setinfo(const char* infoname, void* infoval, int *infolen);

protected:
  // void claw_set_conn(void* conn);
  // void claw_notify_conn(void* conn);

protected:
  // ClawConnProtModule* _c_conn;

private:
};

CLICK_ENDDECLS
#endif
