/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macnotifier.cc,v $
 *  
 ***********************************************************/
#include <click/config.h>

#include <click/hydra_anno.hh>
#include "macnotifier.hh"
CLICK_DECLS

MACListener::MACListener()
{
}

MACListener::~MACListener()
{
}

void
MACListener::notified(uint32_t, MACEvent, const Packet *)
{
}


MACNotifier::MACNotifier()
{
  _listeners.clear();
}

MACNotifier::~MACNotifier()
{
}

void
MACNotifier::add_listener(MACListener *l)
{
    for (int i = 0; i < _listeners.size(); i++)
    {
        //hydra_chatter(8, 8, "MACNotifier::add_listener:for loop %d", i);
	if (_listeners[i] == l)
        {
	    return;
        }
    }  

    _listeners.push_back(l);
}

void
MACNotifier::remove_listener(MACListener *l)
{
    for (int i = 0; i < _listeners.size(); i++)
	if (_listeners[i] == l) {
	    _listeners[i] = _listeners.back();
	    _listeners.pop_back();
	    return;
	}
}

void
MACNotifier::notify(uint32_t agg, MACListener::MACEvent e, const Packet *p) const
{
    for (int i = 0; i < _listeners.size(); i++)
	_listeners[i]->notified(agg, e, p);
}

ELEMENT_REQUIRES(userlevel)
ELEMENT_PROVIDES(MACNotifier)
ELEMENT_PROVIDES(MACListener)

#include <click/vector.cc>
template class Vector<MACListener*>;
CLICK_ENDDECLS
