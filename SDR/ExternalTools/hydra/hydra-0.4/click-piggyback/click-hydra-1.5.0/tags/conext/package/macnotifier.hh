/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macnotifier.hh,v $
 *  
 ***********************************************************/
#ifndef CLICK_MACNOTIFIER_HH
#define CLICK_MACNOTIFIER_HH
#include <click/vector.hh>
CLICK_DECLS
class Packet;

class MACListener { public:

    MACListener();	
    virtual ~MACListener();

    enum MACEvent {TX_DONE, TX_CHANGE};
    virtual void notified(uint32_t, MACEvent, const Packet *);
    
};

class MACNotifier { 
public:

    MACNotifier();		
    ~MACNotifier();	

    void add_listener(MACListener *);
    void remove_listener(MACListener *);

    void notify(uint32_t, MACListener::MACEvent, const Packet *) const;
    
  protected:

    Vector<MACListener *> _listeners;
    
};


CLICK_ENDDECLS
#endif
