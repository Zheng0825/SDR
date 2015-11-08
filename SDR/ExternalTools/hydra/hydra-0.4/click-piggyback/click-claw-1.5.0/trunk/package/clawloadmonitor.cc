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

using namespace std;

CLICK_DECLS

ClawLoadMonitor::ClawLoadMonitor() 
  : ClawAdaptor("Adaptor", "LoadMonitor")
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
  // configuration for claw adaptor is located at the head
  Vector<String> adaptor_conf;
  adaptor_conf.push_back(conf.front());  
  
  conf.erase(conf.begin());

  // First call configure of super class
  int res = 0;
  res = LoadMonitor::configure(conf, errh);
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

int ClawLoadMonitor::claw_getinfo(ClawData* data)
{
  // need to change as the addhandler of Click
  int res  = -1;
  if( data->info_name() == "load" )
  {
    if(*data->info_len() != sizeof(float))
      return res;

    (*(float *) data->info_val()) = calc_load();

    return 0;
  }

  return res;
}

int ClawLoadMonitor::claw_setinfo(ClawData* data)
{
  int res  = -1;

  if( data->info_name() == "flush_load" )
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
