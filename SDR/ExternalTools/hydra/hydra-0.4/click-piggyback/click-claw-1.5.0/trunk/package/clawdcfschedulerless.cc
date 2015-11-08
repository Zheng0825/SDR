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

#include "clawdcfschedulerless.hh"

CLICK_DECLS

using namespace std;

ClawDCFSchedulerLess::ClawDCFSchedulerLess() 
  : ClawAdaptor("DCFSchedulerLess", "AGNOSTIC")
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawDCFSchedulerLess::~ClawDCFSchedulerLess()
{
}

void* ClawDCFSchedulerLess::cast(const char *n)
{
  if( strcmp(n, "ClawDCFSchedulerLess") == 0 )
    return (ClawDCFSchedulerLess *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return DCFScheduler::cast(n);
}


int ClawDCFSchedulerLess::configure(Vector<String> &conf, ErrorHandler *errh)
{
  // configuration for claw adaptor is located at the head
  Vector<String> adaptor_conf;
  adaptor_conf.push_back(conf.front());  
  
  conf.erase(conf.begin());

  // First call configure of super class
  int res = 0;
  res = DCFScheduler::configure(conf, errh);
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

int ClawDCFSchedulerLess::claw_getinfo(ClawData* data)
{
  // need to change as the addhandler of Click
  int res  = -1;
  if( data->info_name() == "data_length" )
  {
    if( *data->info_len() != sizeof(uint32_t))
      return res;

    res = get_data_length((uint32_t *) data->info_val());

    return res;
  }

  return res;
}

int ClawDCFSchedulerLess::claw_setinfo(ClawData* data)
{
  int res  = -1;

  if( data->info_name() == "data_rate")
  {
    if(*data->info_len() != sizeof(uint16_t))
      return res;

    if(*(uint16_t *)data->info_val() > 54)
      return res;

    res = set_data_rate((uint16_t *) data->info_val());
    return res;
  }

  return res;
}


int ClawDCFSchedulerLess::get_data_length(uint32_t* len)
{
  Packet* pkt = 0;
  // if we use RTS/CTS
  if(_response_packet)
    pkt = _response_packet;
  else if(_trigger_packet)
    pkt =_trigger_packet;

  if(!pkt)
    return -1;

  *len = pkt->length();
  hydra_chatter(9, _verbose, "%{element}:get_data_length: %d\n", this, *len);

  return 0;
}

int ClawDCFSchedulerLess::set_data_rate(uint16_t* rate)
{
  // if we use RTS/CTS
  Packet* pkt = 0;
  if(_response_packet)
    pkt = _response_packet;
  else if(_trigger_packet)
    pkt = _trigger_packet; 

  if(!pkt)
    return -1;


  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();
  cha->rate_mbps = *rate;
  _duration->calculate_duration(pkt, 0); 

  hydra_chatter(9, _verbose, "%{element}:set_data_rate: %d\n", this, cha->rate_mbps);
  return 0;
}

// Additional modification for lessclaw implementation
DCFScheduler::NextTXPacket ClawDCFSchedulerLess::process_control(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::process_control: cstate = %d", this, _state);

  // Assumes that Packet p's destination is me.
  struct common_80211 cmm_recv ;
  memcpy(&cmm_recv, p->data(), sizeof(struct common_80211));

  if( is_rts(cmm_recv.i_fc) &&
      (is_state(DCF_IDLE) || is_state(DCF_WAITTX_TRIGGER)) )
  {
      hydra_chatter(8, _verbose, "%{element}::process_control: recv RTS", this, _state);
     _control_packet = p->clone();

     // Change for claw +1
     claw_process_rts(p);

     return NTXP_CTS;
  }
  else if( is_cts(cmm_recv.i_fc) && is_state(DCF_TX_TRIGGER) )
  {
      hydra_chatter(8, _verbose, "%{element}::process_control: recv CTS", this, _state);
      _waiting_packet = _response_packet->clone();

      claw_process_cts(p);
      return NTXP_RESPONSE;                                   
  }                                                           
  else if( is_unicast(cmm_recv) &&                            
           (is_state(DCF_TX_CTS) || is_state(DCF_IDLE)  || is_state(DCF_WAITTX_TRIGGER)))
  {                                                           
      hydra_chatter(8, _verbose, "%{element}::process_control: recv Data", this, _state);
      _control_packet = p->clone();                           
      return NTXP_ACK;                                        
  }                                                           
  else if( is_ack(cmm_recv.i_fc) &&                           
          (is_state(DCF_TX_RESPONSE) ||
           (is_state(DCF_TX_TRIGGER) && !_response_packet)) ) 
  {                                                           
      hydra_chatter(8, _verbose, "%{element}::process_control: recv ACK", this, _state);
      return NTXP_DONE;                                       
  }                                                           
                                                              
  return NTXP_NOTHING;                                        
}                                                             

void ClawDCFSchedulerLess::claw_process_rts(Packet*)
{
  ClawEventSyn ces("Click", "ILIB", _name.c_str(), "snr", 0, 0, "RecvRTS");
  claw_notify_conn( &ces );
}

void ClawDCFSchedulerLess::claw_process_cts(Packet* p)
{
  // We can reduce some overhead by piggybacking more info
  // and remove send data event
  // FIXME: how we can piggyback multiple data on an event?

  // Change for rc_il 1: Notify connector of RecvCTS event
  ClawEventSyn ces_recv_cts("Click", "ILIB", _name.c_str(), 
                            "snr", 0, 0, 
                            "RecvCTS");
  claw_notify_conn(&ces_recv_cts);
  // Changes for rc_il 1: end

  
  // Change for rc_in 1: Notify connector of RecvCTS event
  WritablePacket* pkt = p->uniqueify();
  unsigned int snr_int = 0;
  int org_len = pkt->length();
  unsigned char* pkt_data = pkt->data();
  memcpy(&snr_int, pkt_data + org_len - sizeof(snr_int), sizeof(snr_int));
  pkt->take(sizeof(snr_int));

  ClawEventSyn ces_recv_cts_in("Click", "INIB", _name.c_str(), 
                               "snr_int", sizeof(snr_int), &snr_int, 
                               "RecvCTS");
  claw_notify_conn(&ces_recv_cts_in);
  // Changes for rc_in 1: end
  

  // Change for rc_il 2: Notify connector of SendData event
  ClawEventSyn ces_send_data("Click", "ILIB", _name.c_str(), 
                             "data_rate", 0, 0, "SendData");
  claw_notify_conn(&ces_send_data);

  if( (*(int *)ces_send_data.info_len()) == sizeof(uint16_t)) 
  {
    uint16_t data_rate = *(uint16_t *)ces_send_data.info_val();
    if(data_rate < 54)
      set_data_rate(&data_rate);
  }
  // Changes for rc_il 2: end

}



CLICK_ENDDECLS
EXPORT_ELEMENT(ClawDCFSchedulerLess)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
