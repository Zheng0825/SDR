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

#include "clawsenddata.hh"

using namespace std;

CLICK_DECLS

ClawSendData::ClawSendData() 
  : ClawAdaptor("SendData", "Interceptor"), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawSendData::~ClawSendData()
{
}

void* ClawSendData::cast(const char *n)
{
  if( strcmp(n, "ClawSendData") == 0 )
    return (ClawSendData *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return Element::cast(n);
}


int ClawSendData::configure(Vector<String> &conf, ErrorHandler *errh)
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
    return errh->error("ClawMediator must be defined as an optional configuration");

  hydra_chatter(9, _verbose, "%{element}:configure: ClawMediator = 0x%x\n", this, _mediator);
  register_adaptor(new string("ILIB")); 

  return 0;
}


int ClawSendData::claw_getinfo(ClawData* )
{
  // need to change as the addhandler of Click
  int res  = -1;

  return res;
}

int ClawSendData::claw_setinfo(ClawData*)
{
  int res  = -1;


  return res;
}


Packet* ClawSendData::simple_action(Packet *pkt)
{
  // FIXME: add routine that checks intr_val with recved pkt

  _c_pkt = pkt->uniqueify();

  // It is blocking call, since it is synchronous event
  ClawEventSyn ces("Click", "ILIB", _name.c_str(), "rate", 0, 0, "SendData");
  claw_notify_conn(&ces);

  return _c_pkt;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(ClawSendData)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
