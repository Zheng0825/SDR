/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_LOGTCP_HH
#define CLICK_LOGTCP_HH
#include <click/element.hh>
#include <click/atomic.hh>

CLICK_DECLS

class LogTCP: public Element{
public:
  LogTCP();
  ~LogTCP();

  const char *class_name() const		{ return "LogTCP"; }
  const char *port_count() const    { return "1/1"; }
  const char *processing() const		{ return PUSH; }

  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);

  //Packet *simple_action(Packet *);
  void push(int, Packet*); 

protected:
  String _logfilename; 
  FILE* _fptr_log;
  int _verbose;
  int _logtype; //0: tcpwin 1: tcp ack 

  void process_data(Packet* pkt, int log_type);
};

CLICK_ENDDECLS
#endif
