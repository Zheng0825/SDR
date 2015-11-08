/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/glue.hh>
#include <clicknet/ether.h>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "macframe.hh"
#include "collisionpolicy.hh"
#include "dcfscheduler1.hh"
#include "setduration.hh"
#include "ratecontrol.hh"

CLICK_DECLS

DCFScheduler1::DCFScheduler1() 
  : _data_rate(0) 
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

DCFScheduler1::~DCFScheduler1()
{
}

void*
DCFScheduler1::cast(const char *n)
{
  if( strcmp(n, "DCFScheduler1") == 0 )
    return (DCFScheduler1 *) this;

  return DCFScheduler::cast(n);
}


int
DCFScheduler1::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element *cp_e = 0; 
  Element *rts_frame_e = 0;
  Element *rts_rate_e = 0; 
  Element *data_rate_e = 0;
  Element *duration_e = 0; 
  Element *tx_notifier_e = 0;

  // FIXME: change the macframe from element to supplment class
  int res = cp_va_parse(conf, this, errh,
 			cpElement, "collision reducing polocy", &cp_e,
			cpElement, "generate RTS packet", &rts_frame_e,
			cpElement, "data rate for RTS packet", &rts_rate_e,
			cpElement, "data rate for data packet", &data_rate_e,
 			cpElement, "duration calulator (nav, timeout)", &duration_e,
			cpOptional,
			cpKeywords,
			"NOTIFIER", cpElement, "mac notifier", &tx_notifier_e,
		        "TIMEOUT", cpSecondsAsMicro, "time between retried us)", &_timeout,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  if(cp_e) {
    _cp = (CollisionPolicy *) cp_e->cast("CollisionPolicy");
    if(!_cp) return errh->error("CollisionReducing policy must be set using CollisionPolicy");
  } else {
    return errh->error("Collision reducing policy must be specified");
  }

  if(rts_frame_e) {
    _rts_frame = (MACFrame *) rts_frame_e->cast("MACFrame");
    if(!_rts_frame) return errh->error("RTS frame must be set using MACFrame");
  } else {
    return errh->error("RTS Frame must be specified");
  }

  if(rts_rate_e) {
    _rts_rate = (RateControl *) rts_rate_e->cast("RateControl");
    if(!_rts_rate) return errh->error("RTS data rate must be set using RateControl");
  } else {
    return errh->error("RTS data rate must be specified");
  }

  if(data_rate_e) {
    _data_rate = (RateControl *) data_rate_e->cast("RateControl");
    if(!_data_rate) return errh->error("data rate of Data must be set using RateControl");
  } else {
    return errh->error("data rate of Data must be specified");
  }

  if(duration_e) {
    _duration = (SetDuration *) duration_e->cast("SetDuration");
    if(!_duration) return errh->error("Duration calculator must be set using SetDuration");
  } else {
    return errh->error("Duration calculator must be specified");
  }
 
  if(tx_notifier_e) {
    _tx_notifier = (MACNotifier *) tx_notifier_e->cast("MACNotifier");
    if(!_tx_notifier) return errh->error("NOTIFIER must be set using MACNotifier");
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

void 
DCFScheduler1::prep_response(Packet* pkt) 
{
   // FIXME: May be need writable packet that is retured from get_txrate function.
    hydra_chatter(9, _verbose, "%{element}::prep_response", this);
    _data_rate->get_txrate(pkt);
    _duration->calculate_duration(pkt, 0); 
    //calculate_timeout(p);
}


CLICK_ENDDECLS
EXPORT_ELEMENT(DCFScheduler1)
