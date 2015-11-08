/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWCONNCLICK_HH
#define CLICK_CLAWCONNCLICK_HH

#include <click/element.hh>
#include <claw/claw_conn_protmodule.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

CLICK_DECLS

class Packet;

class ClawConnClick : public Element, public ClawConnProtModule {

public:
  ClawConnClick();
  ~ClawConnClick();

  // From Click
  const char *class_name() const { return "ClawConnClick"; }
  const char *port_count() const { return "0/0"; }
  const char *processing() const { return AGNOSTIC; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  Packet* simple_action(Packet *pkt);

  //virtual void register_adaptor(ClawAdaptor* adaptor);
  //virtual void notify_from_adaptor(ClawAdaptor* adaptor);

  // From Claw
  void send_to_conn(int* size, char* msg);
  void recv_from_conn(int* size, char* msg);

protected:
  //int claw_getinfo(const char* adaptor_type, const char* adaptor_name,
  //                 const char* infoname, void* infoval, int* infolen);

  //int claw_setinfo(const char* adaptor_type, const char* adaptor_name,
  //                 const char* infoname, void* infoval, int* infolen);

  //ClawAdaptor* find_adaptor(std::string& name);
  //void insert_adaptor(std::string& type, std::string& name, void* ptr);
protected:
  //std::vector<ClawAdaptorContext> _c_adaptor_contexts;
  //char* _c_msg;
  unsigned _verbose;

private:
  void init_my_sockets();
  void init_conn_sockets();

private:
  int _recv_sk;
  int _send_sk;

  struct sockaddr_in _dest_address;

};

CLICK_ENDDECLS
#endif
