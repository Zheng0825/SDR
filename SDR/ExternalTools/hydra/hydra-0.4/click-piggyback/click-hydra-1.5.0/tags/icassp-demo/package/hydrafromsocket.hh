// -*- mode: c++; c-basic-offset: 2 -*-
#ifndef CLICK_HYDRAFROMSOCKET_HH
#define CLICK_HYDRAFROMSOCKET_HH
#include <click/element.hh>
CLICK_DECLS

class HydraFromSocket : public Element { public:

  HydraFromSocket();
  ~HydraFromSocket();

  const char *class_name() const	{ return "HydraFromSocket"; }
  const char *port_count() const	{ return "0/1"; }

  int configure(Vector<String> &conf, ErrorHandler *);
  int initialize(ErrorHandler *);
  void cleanup(CleanupStage);

  void * handle(int);
  void selected(int);

 private:

  bool _verbose;
  int _fd;
  int _family;
  int _socktype;
  int _protocol;
  IPAddress _ip;
  unsigned short _port;
  String _pathname;
  unsigned _snaplen;
  bool _frame;

  Vector<WritablePacket *> _packets;
  Vector<int> _active;

  int initialize_socket_error(ErrorHandler *, const char *);

};

CLICK_ENDDECLS
#endif
