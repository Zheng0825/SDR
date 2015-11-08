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

#include <hydra/setduration.hh>

#include "clawmacframectsless.hh"

CLICK_DECLS

using namespace std;

ClawMACFrameCTSLess::ClawMACFrameCTSLess() 
  : ClawAdaptor("MACFrameCTSLess", "Interceptor")
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawMACFrameCTSLess::~ClawMACFrameCTSLess()
{
}

void* ClawMACFrameCTSLess::cast(const char *n)
{
  if( strcmp(n, "ClawMACFrameCTSLess") == 0 )
    return (ClawMACFrameCTSLess *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return MACFrameCTS::cast(n);
}


int ClawMACFrameCTSLess::configure(Vector<String> &conf, ErrorHandler *errh)
{
  // configuration for claw adaptor is located at the head
  Vector<String> adaptor_conf;
  adaptor_conf.push_back(conf.front());  
  
  conf.erase(conf.begin());

  // First call configure of super class
  int res = 0;
  res = MACFrameCTS::configure(conf, errh);
  if (res < 0)
    return res;

  Element* mediator_e = 0;
  res = cp_va_parse(adaptor_conf, this, errh, 
		    cpElement, "inter layer connector", &mediator_e,
		    cpEnd);

  
  if (res < 0)
    return res;
  
  if(mediator_e) 
    _mediator = (ClawMediator *) mediator_e->cast("ClawMediator");
    
  if(!_mediator) 
    return errh->error("ClawMediator must be defined");

  register_adaptor(new string("ILOOB"));

  hydra_chatter(9, _verbose, "%{element}:configure: ClawMediator = 0x%x\n", this, _mediator);
  return 0;

}


int ClawMACFrameCTSLess::claw_getinfo(ClawData* )
{
  return 0;
}

int ClawMACFrameCTSLess::claw_setinfo(ClawData* )
{
  return 0;
}

Packet* ClawMACFrameCTSLess::simple_action(Packet* pkt)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!pkt || !pkt->length())
  {
    if(pkt) pkt->kill();
    return 0;
  }

  WritablePacket* p_out = prepare_packet(pkt);  
  pkt->kill();

  add_snr_int(p_out);

  return p_out;

}

int ClawMACFrameCTSLess::add_snr_int(WritablePacket* p_out)
{
  // Changes for claw
  ClawEventSyn ces("Click", "INIB", _name.c_str(), "snr_int", 0, 0, "SendCTS");
  claw_notify_conn(&ces);

  unsigned char* pkt_data = p_out->data();
  int org_len = p_out->length();
  unsigned int snr_int = 0;

  if(*ces.info_len() != sizeof(snr_int))
    return 0;
    
  snr_int = *(unsigned int *)ces.info_val();
  p_out->put(sizeof(snr_int));
  memcpy(pkt_data+org_len, &snr_int, sizeof(snr_int));
                                                              
  hydra_chatter(9, _verbose, "%{element}::add_snr_int %d\n", this, snr_int);
  return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ClawMACFrameCTSLess)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
