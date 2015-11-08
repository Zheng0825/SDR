/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#ifndef CLICK_CLAWMEDIATOR_HH
#define CLICK_CLAWMEDIATOR_HH

#include <string>
#include <vector>

#include <click/element.hh>

CLICK_DECLS

class Packet;

class ClawAdaptor;
class ClawLConn;

class ClawMediator : public Element {

public:
  ClawMediator();
  ~ClawMediator();

  // From Click
  const char *class_name() const { return "ClawMediator"; }
  const char *port_count() const { return "0/0"; }
  const char *processing() const { return AGNOSTIC; }
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  //Packet* simple_action(Packet *pkt);

  // For claw
  void register_adaptor(std::string& lconn_name, ClawAdaptor* adaptor);
  void notify_from_adaptor(ClawAdaptor* adaptor, ClawEvent* event);

protected:

private:
  ClawLConn* find_lconn(std::string& lconn_name);
  ClawLConn* find_lconn(ClawLConn* lconn);
  void add_lconn(ClawLConn* lconn);

private:
  std::vector<ClawLConn*> _lconns;
  unsigned _verbose;

};

CLICK_ENDDECLS
#endif
