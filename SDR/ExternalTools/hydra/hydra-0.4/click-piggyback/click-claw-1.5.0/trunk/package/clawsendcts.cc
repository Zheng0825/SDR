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

#include "clawsendcts.hh"

using namespace std;

CLICK_DECLS

ClawSendCTS::ClawSendCTS() 
  : ClawAdaptor("SendCTS", "Interceptor"), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawSendCTS::~ClawSendCTS()
{
}

void* ClawSendCTS::cast(const char *n)
{
  if( strcmp(n, "ClawSendCTS") == 0 )
    return (ClawSendCTS *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return Element::cast(n);
}


int ClawSendCTS::configure(Vector<String> &conf, ErrorHandler *errh)
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
  register_adaptor(new string("INIB")); 

  return 0;
}


int ClawSendCTS::claw_getinfo(ClawData* )
{
  // need to change as the addhandler of Click
  int res  = -1;

  return res;
}

int ClawSendCTS::claw_setinfo(ClawData* data)
{
  int res  = -1;

  if( data->info_name() == "snr_int")
  {
    if(*data->info_len() != sizeof(unsigned int))
      return res;
    
    unsigned int snr_int = *(unsigned int *)data->info_val();
    return add_snr_int(snr_int);
  }

  return res;
}


int ClawSendCTS::add_snr_int(unsigned int snr_int)
{
  int org_len = _c_pkt->length();
  _c_pkt->put(sizeof(snr_int));

  unsigned char* data = _c_pkt->data();
  memcpy(data+org_len, &snr_int, sizeof(snr_int));

  hydra_chatter(9, _verbose, "%{element}::add_snr_int %d\n", this, snr_int);
  return 0;
}

Packet* ClawSendCTS::simple_action(Packet *pkt)
{
  // FIXME: add routine that checks intr_val with recved pkt

  _c_pkt = pkt->uniqueify();

  // It is blocking call, since it is synchronous event
  ClawEventSyn ces("Click", "INIB", _name.c_str(), "snr_int", 0, 0, "SendCTS");
  claw_notify_conn(&ces);

  if(*ces.info_len() != sizeof(unsigned int))
    return _c_pkt;
    
  unsigned int snr_int = *(unsigned int *)ces.info_val();
  add_snr_int(snr_int);

  return _c_pkt;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(ClawSendCTS)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
