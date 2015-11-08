/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/mactimer.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_MACTIMER_HH
#define CLICK_MACTIMER_HH
#include <click/element.hh>
#include <click/timer.hh>

CLICK_DECLS


class MACTimer : public Timer {
public:
  MACTimer(TimerHook, void *);
  MACTimer(Element *);
  MACTimer(Task *);
  ~MACTimer();

  bool start_timer(unsigned int delay, struct timeval& stamp);
  void pause_timer();
  bool resume_timer();
  bool resume_timer(unsigned slot_time);
  void stop_timer();
  inline bool paused() {return _paused;}
  inline bool busy() {return scheduled();}
  inline struct timeval& remains() {return _remain_val;} 

protected:
  struct timeval _delay_val; 
  struct timeval _start_val; 
  struct timeval _remain_val;

  bool _paused;

  inline void clear_timeval();

};


CLICK_ENDDECLS
#endif
