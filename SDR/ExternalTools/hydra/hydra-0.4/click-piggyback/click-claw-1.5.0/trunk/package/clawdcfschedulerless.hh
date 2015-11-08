/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWDCFSCHEDULERLESS_HH
#define CLICK_CLAWDCFSCHEDULERLESS_HH

#include <click/element.hh>
#include <hydra/dcfscheduler.hh>

#include "clawadaptor.hh"

CLICK_DECLS

class ClawDCFSchedulerLess : public DCFScheduler, public ClawAdaptor {

public:
  ClawDCFSchedulerLess();
  ~ClawDCFSchedulerLess();

  // From Click
  const char *class_name() const { return "ClawDCFSchedulerLess"; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);

  // From Claw
  //int claw_regintr(ClawEvent* event);
  int claw_getinfo(ClawData* data);
  int claw_setinfo(ClawData* data);

protected:
  // From DCFScheduler
  NextTXPacket process_control(Packet* pkt); 
  void claw_process_rts(Packet* pkt);
  void claw_process_cts(Packet* pkt);

protected:

private:
  int get_data_length(uint32_t* len);
  int set_data_rate(uint16_t* rate);
};

CLICK_ENDDECLS
#endif
