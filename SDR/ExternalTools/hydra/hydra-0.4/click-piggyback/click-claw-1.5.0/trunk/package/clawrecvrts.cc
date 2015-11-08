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

#include "clawrecvrts.hh"

using namespace std;

CLICK_DECLS

ClawRecvRTS::ClawRecvRTS() 
  : ClawAdaptor("RecvRTS", "Interceptor"), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawRecvRTS::~ClawRecvRTS()
{
}

void* ClawRecvRTS::cast(const char *n)
{
  if( strcmp(n, "ClawRecvRTS") == 0 )
    return (ClawRecvRTS *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return Element::cast(n);
}


int ClawRecvRTS::configure(Vector<String> &conf, ErrorHandler *errh)
{
  Element* mediator_e;
  // FIXME: These two arguments should not be optional.
  int res = cp_va_parse(conf, this, errh, 
		    cpElement, "inter layer connector", &mediator_e,
    		    cpOptional, cpKeywords,
                    "VERBOSE", cpUnsigned, "noisy?", &_verbose, 
		    cpEnd);
  
  if (res < 0)
    return res;

  if(mediator_e) 
    _mediator = (ClawMediator *) mediator_e->cast("ClawMediator");

  if(!_mediator) 
    return errh->error("ClawMediator must be defined");

  hydra_chatter(9, _verbose, "%{element}:configure: ClawMediator = 0x%x\n", this, _mediator);

  register_adaptor(new string("ILIB")); 

  return 0;
}


int ClawRecvRTS::claw_getinfo(ClawData* data)
{
  // need to change as the addhandler of Click
  int res  = -1;

  hydra_chatter(9, _verbose, "%{element}:get_info: %s\n", this, data->info_name().c_str());
  if( data->info_name() == "snr")
  {
    if(*data->info_len() != sizeof(double))
      return res;

    struct click_hydra_anno * cha = (struct click_hydra_anno *) _c_pkt->all_user_anno();
    *(double *) data->info_val() = cha->avg_snr;
    hydra_chatter(9, _verbose, "%{element}:get_info: avg_snr=%f\n", this, cha->avg_snr);
    hydra_chatter(9, _verbose, "%{element}:get_info: infoval=%f\n", this, *(double *) data->info_val());
    return res;
  }

  return res;
}

int ClawRecvRTS::claw_setinfo(ClawData*)
{
  int res  = -1;

  return res;
}


Packet* ClawRecvRTS::simple_action(Packet *pkt)
{
  // FIXME: add routine that checks intr_val with recved pkt

  _c_pkt = pkt->uniqueify();

  // It is blocking call, since it is synchronous event
  ClawEventSyn ces("Click", "ILIB", _name.c_str(), "snr", 0, 0, "RecvRTS");
  claw_notify_conn( &ces );

  return _c_pkt;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(ClawRecvRTS)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
