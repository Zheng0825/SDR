/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 * 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
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
