/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/mactimer.cc,v $
 *  
 ***********************************************************/


#include <click/config.h>
#include <click/timer.hh>
#include <click/element.hh>
#include <click/router.hh>
#include <click/master.hh>
#include <click/routerthread.hh>
#include <click/task.hh>

#include "mactimer.hh"

CLICK_DECLS

MACTimer::MACTimer(TimerHook hook, void * thunk)
  : Timer(hook, thunk),  _paused(false)
{
  clear_timeval();
}

MACTimer::MACTimer(Element *e)
  : Timer(e),  _paused(false)
{
  clear_timeval();
}

MACTimer::MACTimer(Task *task)
  : Timer(task),  _paused(false)
{
  clear_timeval();
}

MACTimer::~MACTimer()
{
  if(scheduled()) unschedule();
}


bool
MACTimer::start_timer(unsigned int time, struct timeval& stamp)
{
  bool is_now = false;
  struct timeval elapsed;
  struct timeval now;

  _paused = false; 
  click_gettimeofday(&now);

  _delay_val.tv_sec = 0; _delay_val.tv_usec = time;
  _start_val = now;
  _remain_val = _delay_val;

  // assume time is stamped at upperlayer
  // and it's the reference time for delaying.
  if(stamp.tv_sec)
  {
    elapsed = now - stamp;
    if( elapsed >= _delay_val)
    {
      is_now = true;
      _remain_val.tv_sec=0; _remain_val.tv_usec=0; 
    }
    else
      _remain_val = _delay_val - elapsed;
  }

  schedule_after(_remain_val);

  return is_now;
}


void
MACTimer::pause_timer()
{
  struct timeval elapsed;
  struct timeval now;

  click_gettimeofday(&now);

  unschedule();

  elapsed = now - _start_val;
  if(elapsed >= _remain_val)
  {
    _remain_val.tv_sec=0; _remain_val.tv_usec=0; 
  } 
  else
  {
    _remain_val -= elapsed;
  }
  _paused = true; 
}

bool
MACTimer::resume_timer()
{
  assert (_paused);

  bool is_now = false;
  struct timeval now;

  click_gettimeofday(&now);
  _start_val = now;

  if( (_remain_val.tv_sec > 0) || 
      (_remain_val.tv_sec ==0 && _remain_val.tv_usec > 0) )
  {
    schedule_after(_remain_val); 
  }
  else
  {
    schedule_after(_remain_val);
    is_now = true;
  }

  _paused = false; 

  return is_now;
}

bool
MACTimer::resume_timer(unsigned slot_time)
{
  assert (_paused);

  bool is_now = false;
  struct timeval now;

  if( slot_time == 0 || 
      (_remain_val.tv_sec == 0 && _remain_val.tv_usec ==0) )
    return resume_timer();

  click_gettimeofday(&now);
  _start_val = now;

  unsigned remain_time = _remain_val.tv_sec * 1000000 + _remain_val.tv_usec;
  remain_time = ((remain_time / slot_time) + 1) * slot_time;
  _remain_val.tv_sec = remain_time / (1000000);
  _remain_val.tv_sec = remain_time % (1000000);

  schedule_after(_remain_val); 

  _paused = false; 

  return is_now;
}

void
MACTimer::stop_timer()
{
  unschedule();

  _paused = false;
  clear_timeval();
}

inline void 
MACTimer::clear_timeval()
{
  memset(&_delay_val, 0, sizeof(struct timeval)); 
  memset(&_start_val, 0, sizeof(struct timeval)); 
  memset(&_remain_val, 0, sizeof(struct timeval)); 
}
CLICK_ENDDECLS
ELEMENT_PROVIDES(MACTimer)
