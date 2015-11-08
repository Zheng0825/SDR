/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setdefer.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>
#include <click/standard/scheduleinfo.hh>
#include <clicknet/80211.h>

#include "pullpusher.hh"

CLICK_DECLS

PullPusher::PullPusher()
  : _verbose(0), _task(this)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

PullPusher::~PullPusher()
{
}

void*
PullPusher::cast(const char *n)
{
  if( strcmp(n, "PullPusher") == 0 )
    return (PullPusher *) this;

  return Element::cast(n);
}

int
PullPusher::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
 		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);

  return 0;
}

int
PullPusher::initialize(ErrorHandler *errh)
{
  hydra_chatter(9, _verbose, "%{element}::initialize", this);
  ScheduleInfo::join_scheduler(this, &_task, errh);

  _nonempty_signal = Notifier::upstream_empty_signal(this, 0, &_task);
  return 0;
}

bool
PullPusher::run_task()
{
  bool work_done = false;

  hydra_chatter(9, _verbose, "%{element}::run_task", this);

  Packet *p = input(0).pull();
  while(p)
  {
    work_done = true;
    output(0).push(p);
    p = input(0).pull();
  }

  _task.fast_reschedule();
  return work_done;
}

enum {H_VERBOSE};

String
PullPusher::read_param(Element *e, void *thunk)
{
  PullPusher *td = (PullPusher *)e;
  switch ((uintptr_t) thunk) 
  {
    case H_VERBOSE:
      return String(td->_verbose) + "\n";
    default:
      return String();
  }

  return String();

}

int 
PullPusher::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  PullPusher *f = (PullPusher *)e;
  String s = cp_uncomment(in_s);
  switch((int)vparam) 
  {
    case H_VERBOSE: 
    {
      unsigned m;
      if (!cp_unsigned(s, &m)) 
        return errh->error("stepup parameter must be unsigned");
      f->_verbose = m;
      break;
    }
  }

  return 0;

}
void
PullPusher::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PullPusher)

