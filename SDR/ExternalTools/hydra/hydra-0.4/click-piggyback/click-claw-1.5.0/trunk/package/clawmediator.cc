/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#include <stdio.h>
 
#include <click/config.h>
#include <click/glue.hh>
#include <clicknet/ether.h>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>

#include "clawadaptor.hh"
#include "clawlconn.hh"

#include "clawmediator.hh"

using namespace std;

CLICK_DECLS

ClawMediator::ClawMediator() : _verbose(0)
{
}

ClawMediator::~ClawMediator()
{
}

void* ClawMediator::cast(const char *n)
{
  if( strcmp(n, "ClawMediator") == 0 )
    return (ClawMediator *) this;

  return Element::cast(n);
}

int ClawMediator::configure(Vector<String> &conf, ErrorHandler *errh)
{

  for(int i = 0; i < conf.size(); i++)                              
  {                                                             
    Vector<String> words;                                       

    cp_spacevec(conf[i], words);                                
    if(words.size() == 1)                                       
    {
      Element* lconn_e = cp_element(words[0], this, errh);
      if(lconn_e) {
        ClawLConn* lconn = (ClawLConn *) lconn_e->cast("ClawLConn");
        _lconns.push_back(lconn);
      } else {
        return errh->error("ClawLconn should be defined");
      }
    }
    else if((words.size() == 2) && (words[0] == String("VERBOSE")))
    {
      if(!cp_unsigned(words[1], &_verbose))
        return errh->error("VERBOSE should be unsigned");

    }
    else
    {
      return errh->error("argument %d should have two elements", i);          
    }

    return 0;
  }
}

/*
Packet* ClawMediator::simple_action(Packet *pkt)
{
  return pkt;
}
*/

void ClawMediator::register_adaptor(string& lconn_name, ClawAdaptor* adaptor)
{

  ClawLConn* lconn = find_lconn(lconn_name); 
  if(lconn)
    lconn->register_adaptor(adaptor);
  
}

void ClawMediator::notify_from_adaptor(ClawAdaptor* adaptor, ClawEvent* event)
{
  ClawLConn* lconn = find_lconn(event->conn_name()); 
  if(lconn)
    lconn->process_adaptor_event(adaptor, event);

}

ClawLConn* ClawMediator::find_lconn(string& lconn_name)
{
  int num_lconns = _lconns.size();
  for(int i = 0; i < num_lconns; i++)
  {
    if( ((ClawLConn*) _lconns[i])->is_name(lconn_name) )
      return (ClawLConn *) _lconns[i];

  }

  return 0;
}

ClawLConn* ClawMediator::find_lconn(ClawLConn* lconn)
{
  int num_lconns = _lconns.size();
  for(int i = 0; i < num_lconns; i++)
  {
    if( ((ClawLConn*) _lconns[i])->is_same(lconn) )
      return (ClawLConn *) _lconns[i];

  }

  return 0;
}

void ClawMediator::add_lconn(ClawLConn* lconn)
{
  ClawLConn* prev = find_lconn(lconn);
  if(prev)
    return;

  _lconns.push_back(lconn);
}

         
CLICK_ENDDECLS
EXPORT_ELEMENT(ClawMediator)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
