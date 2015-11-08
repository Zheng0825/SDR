/*********************************************************
 * 
 *  
 ***********************************************************/

#ifndef CLICK_PRIORITIZER_HH
#define CLICK_PRIORITIZER_HH
#include <list>
#include <map>
#include <click/element.hh>
#include <click/glue.hh>
#include <click/notifier.hh>
#include <click/etheraddress.hh>
CLICK_DECLS

class Prioritizer : public Element { public:
  
  Prioritizer();
  ~Prioritizer();
  
  const char *class_name() const		{ return "Prioritizer"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return PUSH_TO_PULL; }

  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);

  void add_handlers();

  void push(int port, Packet *p);
  // Override the pull method to support prioritizing
  Packet *pull(int port);

private:
  struct strCmp {
    bool operator()( const String &s1, const String &s2 ) const {
      return s1 < s2;
    }
  };

  unsigned _verbose;

  std::list<Packet *> *_cur_plist;
  std::map<EtherAddress, std::list<Packet *>, strCmp > _pmap;
  std::list<std::list<Packet *> *> _fifo_list;

  ActiveNotifier _empty_note;

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);
};

CLICK_ENDDECLS
#endif
