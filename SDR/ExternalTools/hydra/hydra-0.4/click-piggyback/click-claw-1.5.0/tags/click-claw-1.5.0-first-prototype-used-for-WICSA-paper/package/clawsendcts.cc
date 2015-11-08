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

CLICK_DECLS

ClawSendCTS::ClawSendCTS() : _verbose(0)
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
  Element* conn_e;
  // FIXME: These two arguments should not be optional.
  int res = cp_va_parse(conf, this, errh, 
		    cpElement, "inter layer connector", &conn_e,
    		    cpOptional, cpKeywords,
                    "VERBOSE", cpUnsigned, "noisy?", &_verbose, 
		    cpEnd);
  
  if (res < 0)
    return res;

  ClawConnProtModule* conn = 0;
  if(conn_e) 
    conn = (ClawConnProtModule *) conn_e->cast("ClawConnProtModule");

  if(!conn) 
    return errh->error("ClawConn must be defined as an optional configuration");

  hydra_chatter(9, _verbose, "%{element}:configure: ClawConn = 0x%x\n", this, conn);
  claw_set_conn(conn); 

  return 0;
}


int ClawSendCTS::claw_regintr(int , char* , int )
{
  int res = -1;

  // FIXME: add intrval = (offset, packet pattern) to DB
 
  return res;
}

int ClawSendCTS::claw_getinfo(const char* , void* , int* )
{
  // need to change as the addhandler of Click
  int res  = -1;

  return res;
}

int ClawSendCTS::claw_setinfo(const char* infoname, void* infoval, int* infolen)
{
  int res  = -1;

  if( strcmp(infoname, "snr_int") == 0 )
  {
    if(*infolen != sizeof(unsigned int))
      return res;
    
    unsigned int snr_int = *(unsigned int *)infoval;
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

  return 0;
}

Packet* ClawSendCTS::simple_action(Packet *pkt)
{
  // FIXME: add routine that checks intr_val with recved pkt

  _c_pkt = pkt->uniqueify();

  // It is blocking call, since it is synchronous event
  claw_notify_conn();

  return _c_pkt;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(ClawSendCTS)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
