/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWLOADMONITOR_HH
#define CLICK_CLAWLOADMONITOR_HH

#include <click/element.hh>
#include <clawadaptor.hh>
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
  int claw_getinfo(ClawData* data);
  int claw_setinfo(ClawData* data);
  //int claw_regintr(ClawEvent* event);

protected:

protected:

private:
};

CLICK_ENDDECLS
#endif
