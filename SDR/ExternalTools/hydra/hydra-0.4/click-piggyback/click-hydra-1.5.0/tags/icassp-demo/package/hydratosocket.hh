// -*- mode: c++; c-basic-offset: 2 -*-
#ifndef CLICK_HYDRATOSOCKET_HH
#define CLICK_HYDRATOSOCKET_HH
#include <click/element.hh>
#include <click/string.hh>
#include <click/task.hh>
#include <click/notifier.hh>
#include <sys/un.h>
CLICK_DECLS


class HydraToSocket : public Element { public:

  HydraToSocket();
  ~HydraToSocket();

  const char *class_name() const	{ return "HydraToSocket"; }
  const char *port_count() const	{ return "1/0"; }
  const char *processing() const	{ return AGNOSTIC; }
  
  int configure(Vector<String> &conf, ErrorHandler *);
  int initialize(ErrorHandler *);
  void cleanup(CleanupStage);
  void add_handlers();

  void selected(int);

  void push(int port, Packet *);
  bool run_task();

protected:
  Task _task;
  void send_packet(Packet *);

 private:

  bool _verbose;
  int _fd;
  int _active;
  int _family;
  int _socktype;
  int _protocol;
  IPAddress _ip;
  int _port;
  String _pathname;
  int _snaplen;
  union { struct sockaddr_in in; struct sockaddr_un un; } sa;
  socklen_t sa_len;
  NotifierSignal _signal;
  int _nodelay;
  bool _frame;

  int initialize_socket_error(ErrorHandler *, const char *);

};

CLICK_ENDDECLS
#endif
