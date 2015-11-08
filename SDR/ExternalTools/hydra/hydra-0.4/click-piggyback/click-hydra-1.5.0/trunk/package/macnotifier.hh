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
