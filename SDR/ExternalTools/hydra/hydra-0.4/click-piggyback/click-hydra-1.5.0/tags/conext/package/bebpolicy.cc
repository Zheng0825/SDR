/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/bebpolicy.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/packet_anno.hh>
//#include <clicknet/ether.h>
//#include <click/etheraddress.hh>

#include <click/hydra_anno.hh>
#include <clicknet/80211.h>
#include "bebpolicy.hh"

CLICK_DECLS

BEBPolicy::BEBPolicy()
  : _max_cw(0xFFFFFFFF), _min_cw(1)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

BEBPolicy::~BEBPolicy()
{
}

void*
BEBPolicy::cast(const char *n)
{
    if (strcmp(n, "BEBPolicy") == 0)
	return (BEBPolicy *) this;

    return CollisionPolicy::cast(n);
}

int
BEBPolicy::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
		  cpOptional,
		  cpKeywords, 
 		  "MAX_RETRIES", cpUnsigned, "max retry number", &_max_tries,
 		  "MAX_CW", cpUnsigned, "max contention window", &_max_cw,
 		  "MIN_CW", cpUnsigned, "min contention window", &_min_cw,
		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  _cw = _min_cw;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
BEBPolicy::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  Packet* p_out = 0;

  if (p) 
  {
    //struct click_hydra_anno *cha = (struct click_hydra_anno *) p->all_user_anno();
    //cha->magic = HYDRA_MAGIC;

    p_out = set_control(p);
    if(p_out)
      _num_tx++;
    else
      p->kill();
  }

  //struct click_hydra_anno* debug = (struct click_hydra_anno*) p->all_user_anno();
  //printf("bebpolicy: 0x%x\n", debug->mac_type);
  return p_out;
}

void 
BEBPolicy::success(const Packet* )
{
  hydra_chatter(8, _verbose, "%{element}::success", this);

  _num_success++;
  _num_tries = 1;
 
  _cw = _min_cw;
  
}

void 
BEBPolicy::fail(const Packet* )
{
  hydra_chatter(8, _verbose, "%{element}::fail", this);

  _num_fail++;
  _num_tries++;

  if(_cw < _max_cw);
    _cw = (_cw << 1);
}

bool 
BEBPolicy::retry(const Packet* )
{
  //hydra_chatter(8, _verbose, "%{element}::retry", this);
  return (_num_tries <= _max_tries);
}

void 
BEBPolicy::reset_retry(const Packet* )
{
  hydra_chatter(8, _verbose, "%{element}::reset_retry", this);
  _num_tries = 1;
  _cw = _min_cw;
}


Packet* 
BEBPolicy::process_control(Packet*)
{
  return 0; 
}

Packet* 
BEBPolicy::set_control(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::set_control cw = %d", this, _cw);
  //Packet* p_out = 0; 

  if(!retry(0))
  {
    // FIXME: do I need to kill packet?
    return 0; 
  } 

  unsigned int backoff =  random() % _cw;

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();
  cha->slot = backoff;

  return p; 
}

String
BEBPolicy::read_param(Element *e, void *thunk)
{
  BEBPolicy *cp = (BEBPolicy *)e;
  switch ((uintptr_t) thunk) {
  case 0:
    return String(cp->_verbose) + "\n";
  default:
    return String();
  }
  
  return String();
}

int 
BEBPolicy::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  BEBPolicy *cp = (BEBPolicy *)e;
  String s = cp_uncomment(in_s);
  switch((int)vparam) {
  case 0: {
    unsigned verbose;
    if (!cp_unsigned(s, &verbose)) 
      return errh->error("verbose parameter must be boolean");
    cp->_verbose = verbose;
    break;
  }
  }
    return 0;

}
void
BEBPolicy::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(BEBPolicy)

