/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWMACFRAMECTSLESS_HH
#define CLICK_CLAWMACFRAMECTSLESS_HH

#include <click/element.hh>
#include <hydra/macframects.hh>

#include "clawadaptor.hh"

CLICK_DECLS

class ClawMACFrameCTSLess : public MACFrameCTS, public ClawAdaptor {

public:
  ClawMACFrameCTSLess();
  ~ClawMACFrameCTSLess();

  // From Click
  const char *class_name() const { return "ClawMACFrameCTSLess"; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);

  //From MACFrameCTS
  Packet* simple_action(Packet* pkt);

  // From Claw
  //int claw_regintr(ClawEvent* event);
  int claw_getinfo(ClawData* data);
  int claw_setinfo(ClawData* data);

protected:

protected:

private:
  int add_snr_int(WritablePacket* p_out);

};

CLICK_ENDDECLS
#endif
