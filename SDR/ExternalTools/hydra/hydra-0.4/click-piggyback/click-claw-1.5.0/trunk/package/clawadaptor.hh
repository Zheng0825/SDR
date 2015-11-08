#ifndef CLICK_CLAWADAPTOR_HH
#define CLICK_CLAWADAPTOR_HH

#include <string>

#include <claw/claw_data.h>
#include <claw/claw_event_syn.h>
#include <claw/claw_event_asyn.h>

#include "clawmediator.hh"

class ClawAdaptor {

public:
  ClawAdaptor(std::string name, std::string type) 
    : _mediator(0), _name(name), _type(type) {}
  virtual ~ClawAdaptor() {}

  std::string& adaptor_name() { return _name; }
  std::string& adaptor_type() { return _type; } 
  bool is_name(std::string& name) { return (_name == name); }
  bool is_type(std::string& type) 
  { 
    if(_type == "AGNOSTIC")
      return true;

    return (_type == type); 
  }
  bool is_same(ClawAdaptor* rhs) 
  { 
    return (is_type(rhs->_type) && is_name(rhs->_name)); 
  } 

  // intr_type = sync, async
  // intrval = (sync, packet format), (async, infoname)
  //virtual int claw_regintr(int intrtype, char* intrval, int intrlen) = 0;
  //virtual int claw_getinfo(const char* infoname, void* infoval, int* infolen) = 0;
  //virtual int claw_setinfo(const char* infoname, void* infoval, int* infolen) = 0;
  //virtual int claw_regintr(ClawEvent* event) = 0;
  virtual int claw_getinfo(ClawData* data)= 0;
  virtual int claw_setinfo(ClawData* data) = 0;

protected:
/*
  void claw_set_mediator(ClawMediator* mediator)
  {
    _mediator = mediator;
  }
*/
  void register_adaptor(std::string* lconn_name)
  {
    if(_mediator)
      _mediator->register_adaptor(*lconn_name, (ClawAdaptor*) this);
  }

  // Fix me need to deliver event type as an argument
  void claw_notify_conn(ClawEvent* event)
  {
    if(_mediator)
      _mediator->notify_from_adaptor((ClawAdaptor *) this, event);
  }

protected:
  ClawMediator* _mediator;

  std::string _type;
  std::string _name;

private:
  ClawAdaptor() : _mediator(0) {}
};

#endif
