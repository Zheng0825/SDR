/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/packet_anno.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>

#include "setpower.hh"
#include "powercontrol.hh"

CLICK_DECLS

SetPower::SetPower()
  : _power_control(0), _verbose(0), 
    _packet_count(0), _power_probe(3000)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

SetPower::~SetPower()
{
}

int
SetPower::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* e = 0;
  if (cp_va_parse(conf, this, errh,
  		  cpElement, "power control algorithm", &e,
		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }

  if (e) { 
    _power_control = (PowerControl *) e->cast("PowerControl");
    if(!_power_control) return errh->error("PowerControl must be set using PowerControl");
  } else {
    return errh->error("PowerControl must be defined");
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
SetPower::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  WritablePacket* p_out = _power_control->get_power(p_in);

  hydra_chatter(9, _verbose, "%{element}::simple_action p_in=0x%x, p_out=0x%x", this, p_in, p_out);

  return p_out;
}

enum {H_VERBOSE};

String
SetPower::read_handler(Element *e, void *thunk)
{
  SetPower *src = (SetPower *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
SetPower::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  SetPower *src = (SetPower *) e;
  String s = cp_uncomment(arg);
  switch((int)vparam) {
  case H_VERBOSE: {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("verbose parameter must be unsigned");
    src->_verbose = m;
    break;
  }
  }
  return 0;
}

void
SetPower::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetPower)

