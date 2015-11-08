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

#include "clawdcfscheduler.hh"

CLICK_DECLS

ClawDCFScheduler::ClawDCFScheduler() 
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawDCFScheduler::~ClawDCFScheduler()
{
}

void* ClawDCFScheduler::cast(const char *n)
{
  if( strcmp(n, "ClawDCFScheduler") == 0 )
    return (ClawDCFScheduler *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return DCFScheduler::cast(n);
}


int ClawDCFScheduler::configure(Vector<String> &conf, ErrorHandler *errh)
{
  Vector<String> adaptor_conf;
  adaptor_conf.push_back(conf.front());  
  
  conf.erase(conf.begin());
/*
  printf("conf ==========>\n");
  for(int i=0; i<conf.size(); i++)
  {
    printf("<size = %d : ", conf[i].length());
    for(int j=0; j<conf[i].length(); j++)
      printf("%c", conf[i].data()[j]);

    printf(" >\n");
  }

  printf("adaptor_conf ==========>\n");
  for(int i=0; i<adaptor_conf.size(); i++)
  {
    printf("<size = %d : ", adaptor_conf[i].length());
    for(int j=0; j<adaptor_conf[i].length(); j++)
      printf("%c", adaptor_conf[i].data()[j]);

    printf(" >\n");
  }
*/
  int res = 0;
  res = DCFScheduler::configure(conf, errh);
  if (res < 0)
    return res;

  Element* conn_e = 0;
  res = cp_va_parse(adaptor_conf, this, errh, 
		    cpElement, "inter layer connector", &conn_e,
		    cpEnd);

  
  if (res < 0)
    return res;
  
  ClawConnProtModule* conn = 0;
  if(conn_e) 
    conn = (ClawConnProtModule *) conn_e->cast("ClawConnProtModule");
    
  if(!conn) 
    return errh->error("ClawConn must be defined");

  hydra_chatter(9, _verbose, "%{element}:configure: ClawConn = 0x%x\n", this, conn);
  claw_set_conn(conn); 

  return 0;

}

int ClawDCFScheduler::claw_getinfo(const char* infoname, void* infoval, int* infolen)
{
  // need to change as the addhandler of Click
  int res  = -1;
  if( strcmp(infoname, "data_length") == 0 )
  {
    if(*infolen != sizeof(uint32_t))
      return res;

    res = get_data_length((uint32_t *) infoval);

    return res;
  }

  return res;
}

int ClawDCFScheduler::claw_setinfo(const char* infoname, void* infoval, int* infolen)
{
  int res  = -1;

  if( strcmp(infoname, "data_rate") == 0 )
  {
    if(*(uint16_t *)infolen != sizeof(uint16_t))
      return res;

    if(*(uint16_t *)infoval > 54)
      return res;

    res = set_data_rate((uint16_t *) infoval);
    return res;
  }

  return res;
}


int ClawDCFScheduler::get_data_length(uint32_t* len)
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

int ClawDCFScheduler::set_data_rate(uint16_t* rate)
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

CLICK_ENDDECLS
EXPORT_ELEMENT(ClawDCFScheduler)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
