/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 * 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
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
