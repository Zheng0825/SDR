/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWLCONN_HH
#define CLICK_CLAWLCONN_HH

#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "clawadaptor.hh"
//#include "clawadaptorcontext.hh"

#include <claw/claw_data.h>
#include <claw/claw_event.h>
#include <claw/claw_format.h>


class ClawLConn {

public:
  ClawLConn(std::string pm_name, std::string name) : _pm_name(pm_name), _name(name)
  {
    _format = new ClawFormat();
    _msg = new char [_format->MAX_FORMAT_LEN];
  }

  ~ClawLConn()
  {
    delete [] _msg;
    delete _format;
  }

  // For Claw
  virtual ClawLConn* lconn_cast() { return (ClawLConn *) this; }
  virtual bool is_name(std::string& conn_name) = 0;
  virtual bool is_same(ClawLConn* rhs) = 0;

  virtual void register_adaptor(ClawAdaptor* adaptor) = 0;
  //virtual void unregister_adaptor(ClawAdaptor* adaptor) = 0;
  virtual void process_adaptor_event(ClawAdaptor* adaptor, ClawEvent* event) = 0;

protected:
  virtual int get_info(ClawData* data) = 0;
  virtual int set_info(ClawData* data) = 0;

  // These should be block-call that pauses the adaptor.
  virtual int send_to_gconn(int conn_type,  int size) = 0;
  virtual int recv_from_gconn(int* conn_type) = 0;

  virtual ClawAdaptor* find_adaptor(std::string& name) = 0;
  virtual ClawAdaptor* find_adaptor(ClawAdaptor* adaptor) = 0;
  virtual void insert_adaptor(ClawAdaptor* adaptor) = 0;
  //virtual void remove_adaptor(std::string& type, std::string& name) = 0;


protected:
  std::string _pm_name;
  std::string _name;
  std::vector<std::string> _aliase_names;
  ClawFormat* _format;
  std::vector<ClawAdaptor*> _adaptors;
  char* _msg;

private:

private:

};


#endif
