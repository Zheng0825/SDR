/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/packet.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>
#include "loadmonitor.hh"

CLICK_DECLS

LoadMonitor::LoadMonitor() : _cur_state(CS_IDLE), _load(0.0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

LoadMonitor::~LoadMonitor()
{
}

int
LoadMonitor::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  if (res < 0)
    return res;


  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int LoadMonitor::initialize(ErrorHandler *) 
{
  struct timeval now;
  click_gettimeofday(&now);

  change_state(CS_IDLE, now);
  return 0;
}

void
LoadMonitor::push(int port, Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::push:", this);

  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno *cha = (struct click_hydra_anno *) p->all_user_anno();
  // FIXME: It assume that CS_BUSY is followed by CS_IDLE and vice versa.
  //        It didn't consider the case CS_BUSY followed by CS_BUSY .
  if( cha->type == HYDRA_CCA_BUSY)
  {
    if(_cur_state == CS_BUSY)
      goto wrong_state;

    change_state(CS_BUSY, now);
  }
  else if (cha->type == HYDRA_CCA_IDLE)
  {
    if(_cur_state == CS_IDLE)
      goto wrong_state;

    change_state(CS_IDLE, now);
  }
  else
  {
    goto wrong_state;
  }

  _load = calc_load();

  output(0).push(p);
  return;

wrong_state:
   hydra_chatter(0, _verbose, "%{element}::push: invalid packet", this);
   p->kill();
   return;
}

void LoadMonitor::change_state(unsigned char state, struct timeval tv)
{
  struct edge init_edge;

  init_edge._tv = tv;
  _cur_state = init_edge._type = state;
  _edges.push_back(init_edge);

}

float LoadMonitor::calc_load()
{
  struct timeval now;
  click_gettimeofday(&now);

  struct timeval win_left = now - _window_tv;

  // Prune edges outside of window
  struct edge prev = _edges.front();
  _edges.pop_front();
  while(1)
  {
    if((_edges.size() == 0) || (_edges.front()._tv >= win_left) )
    {
      _edges.push_front(prev);
      break;  
    }
    prev = _edges.front();
    _edges.pop_front();
  }

  // There was no change inside window
  if(_edges.size() == 1)
  {
    if(_edges.front()._type == RISING) return (_load = 1.0);
    else return (_load = 0.0);
  }

  struct timeval dur_idle = make_timeval(0,0);
  struct timeval dur_busy = make_timeval(0,0);
  struct timeval left = make_timeval(0,0);
  struct timeval right = make_timeval(0,0);
  int num_edges = _edges.size();

  for(int i=0; i<num_edges; i++)
  {
    if(i==0) left = win_left;
    else left = _edges[i]._tv;

    if(i==(num_edges -1)) right = now; 
    else right = _edges[i+1]._tv;

    struct timeval dur = right - left;
    if(_edges[i]._type == RISING) dur_busy += dur;
    else dur_idle += dur;
  }

  // FIXME: change it to calculate more correct load
  float dur_busy_ms = (float)(dur_busy.tv_sec * 1000 + dur_busy.tv_usec/1000);
  float dur_idle_ms = (float)(dur_idle.tv_sec * 1000 + dur_idle.tv_usec/1000);

  return (_load = (dur_busy_ms)/(dur_busy_ms + dur_idle_ms));
}

enum {H_VERBOSE};

String
LoadMonitor::read_param(Element *e, void *vparam)
{
  LoadMonitor *tp = (LoadMonitor *)e;
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
      return String(tp->_verbose);
    default:
      return "";
  }

  return String();
}

int
LoadMonitor::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  LoadMonitor *tp = (LoadMonitor *)e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      tp->_verbose = verbose;
      break; 
    }
    default:
      break;
  }

  return 0;
}

void
LoadMonitor::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

EXPORT_ELEMENT(LoadMonitor)

#include <click/dequeue.cc>
template class DEQueue<LoadMonitor::edge>;
CLICK_ENDDECLS
