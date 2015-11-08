/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWDCFSCHEDULER_HH
#define CLICK_CLAWDCFSCHEDULER_HH

#include <click/element.hh>
#include <hydra/dcfscheduler.hh>

#include "clawadaptor.hh"

CLICK_DECLS

class ClawDCFScheduler : public DCFScheduler, public ClawAdaptor {

public:
  ClawDCFScheduler();
  ~ClawDCFScheduler();

  // From Click
  const char *class_name() const { return "ClawDCFScheduler"; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);

  // From Claw
  //int claw_regintr(ClawEvent* event);
  int claw_getinfo(ClawData* data);
  int claw_setinfo(ClawData* data);

protected:

protected:

private:
  int get_data_length(uint32_t* len);
  int set_data_rate(uint16_t* rate);
};

CLICK_ENDDECLS
#endif
