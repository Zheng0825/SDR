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


#include "clawloadmonitor.hh"

CLICK_DECLS

ClawLoadMonitor::ClawLoadMonitor() 
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawLoadMonitor::~ClawLoadMonitor()
{
}

void* ClawLoadMonitor::cast(const char *n)
{
  if( strcmp(n, "ClawLoadMonitor") == 0 )
    return (ClawLoadMonitor *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return LoadMonitor::cast(n);
}


int ClawLoadMonitor::configure(Vector<String> &conf, ErrorHandler *errh)
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
  res = LoadMonitor::configure(conf, errh);
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

int ClawLoadMonitor::claw_getinfo(const char* infoname, void* infoval, int* infolen)
{
  // need to change as the addhandler of Click
  int res  = -1;
  if( strcmp(infoname, "load") == 0 )
  {
    if(*infolen != sizeof(float))
      return res;

    float load = calc_load();
    memcpy(infoval, &load, *infolen);

    return 0;
  }

  return res;
}

int ClawLoadMonitor::claw_setinfo(const char* infoname, void* infoval, int* infolen)
{
  int res  = -1;

  if( strcmp(infoname, "flush_load") == 0 )
  {
    _edges.clear();

    struct timeval now;
    click_gettimeofday(&now);
    
    struct edge init_edge;
    init_edge._tv = now;
    init_edge._type = _cur_state;
        
    return 0;
  }

  return res;
}



CLICK_ENDDECLS
EXPORT_ELEMENT(ClawLoadMonitor)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
