/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWLCONNUDP_HH
#define CLICK_CLAWLCONNUDP_HH

#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <click/element.hh>
#include "clawlconn.hh"

class ClawLConnUDP : public ClawLConn, public Element {

public:
  ClawLConnUDP();
  ~ClawLConnUDP();

  // From Click
  const char *class_name() const { return "ClawLConnUDP"; }
  const char *port_count() const { return "0/0"; }
  const char *processing() const { return AGNOSTIC; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  void selected(int fd);
  //Packet* simple_action(Packet *pkt);

  // For Claw
  //ClawLConn* lconn_cast() { return (ClawLConn *) this; }
  bool is_name(std::string& conn_name);
  bool is_same(ClawLConn* lconn);

  void register_adaptor(ClawAdaptor* adaptor);
  //void unregister_adaptor(ClawAdaptor* adaptor);
  void process_adaptor_event(ClawAdaptor* adaptor, ClawEvent* event);

protected:
  int get_info(ClawData* data);
  int set_info(ClawData* data);

  // These should be block-call that pauses the adaptor.
  int process_gconn_req(unsigned char info_type, int size);
  int send_to_gconn(int conn_type,  int size);
  int recv_from_gconn(int* conn_type);

  ClawAdaptor* find_adaptor(std::string& name);
  ClawAdaptor* find_adaptor(ClawAdaptor* adaptor);
  void insert_adaptor(ClawAdaptor* adaptor);
  //void remove_adaptor(std::string& type, std::string& name);

protected:
  //std::string _pm_name;
  //ClawFormat* _format;
  //std::vector<ClawAdaptor*> _adaptors;
  //char* _msg;

private:
  void init_sockets();
  void terminate_sockets();
  int conn_type_int(std::string& conn_name);

private:
  // This local conntector uses Socket for IPC
  unsigned short _lconn_port;
  int _lconn_sk;

  enum {ILIB=0, ILOOB, INIB, INOOB, NUM_GCONNS};
  std::vector<unsigned short> _gconn_ports;
  struct sockaddr_in _gconn_sin;

  unsigned _verbose;
};



#endif
