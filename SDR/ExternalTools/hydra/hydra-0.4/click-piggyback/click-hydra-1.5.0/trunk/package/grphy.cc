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


#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/error.hh>

#include <clicknet/mpi.h>
#include <click/hydra_anno.hh>
#include "grphy.hh"

#include <stdio.h>


CLICK_DECLS

// port 0  (from MAC to PHY) = MAC.TX_DATA
// port 1  (from PHY to MAC) = PHY.CS_BUSY, PHY.RX_DATA, PHY.TX_DONE 

GrPhy::GrPhy() : _cur_state(STATE_IDLE), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
  init_states();
}

GrPhy::~GrPhy()
{
}

void GrPhy::init_states()
{
  _states.clear();
  add_state(STATE_IDLE, &GrPhy::run_state_idle);
  add_state(STATE_TX, &GrPhy::run_state_tx);
  add_state(STATE_RX, &GrPhy::run_state_rx);
}

void GrPhy::add_state(MPIState state, run_state_fptr fptr)
{
  RunState temp;
  temp._state = state;
  temp._fptr = fptr;

  _states.push_back(temp);
};

void* GrPhy::cast(const char *n)
{
  if( strcmp(n, "GrPhy") == 0 )
    return (GrPhy *)this;
  else if( strcmp(n, "MACNotifier") == 0 )
    return (MACNotifier*) (this);

  return Element::cast(n);

}


int GrPhy::configure(Vector<String> &conf, ErrorHandler *errh)
{

  int res =  cp_va_parse(conf, this, errh,
		     cpOptional,
		     cpKeywords,
		     "VERBOSE", cpUnsigned, "noisy?", &_verbose,
                     cpEnd);

  if(res < 0)
    return -1;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

void GrPhy::push(int port, Packet *pkt) 
{ 
  hydra_chatter(9, _verbose, "%{element}::push: ", this);

  if (!pkt || !pkt->length()) 
  {
    hydra_chatter(1, _verbose, "%{element} :push invalid packet", this);
    if (pkt) pkt->kill(); 

    return;  
  }

  run_states(port, pkt);
}

void GrPhy::run_states(int port, Packet* pkt)
{
  struct mpitype* mt = (struct mpitype *) pkt->data();
  uint8_t type = mt->type;
  
  //struct timeval tv;
  //click_gettimeofday(&tv);

  // Basically, this checking routine is not needed.
  if(port == TO_PHY_PORT) 
  {
    // FIXME: remove following two lines after finishing
    //legacy_on_to_phy_message(pkt);
    //type = mt->type;
    if(type != MPIMSG_TX_DATA)
    {
      hydra_chatter(0, _verbose, "%{element} : run_states : never this case : wrong message type for to phy port\n", this);
      assert(false);
    }
  }
  else if(port == FROM_PHY_PORT)
  {
    if(type != MPIMSG_TX_DONE && type != MPIMSG_RX_DATA && type != MPIMSG_CS_BUSY)
    {
      hydra_chatter(0, _verbose, "%{element} : run_states : never this case : wrong message type for from phy port\n", this);
      assert(false);
    }
  }

/*
class Foo {        
public:                
  double One( long inVal );                
  double Two( long inVal );
};

void main( int argc, char **argv )
{	
  double (Foo::*funcPtr)( long ) = &Foo::One; 	
  Foo aFoo; 	
  double result =(aFoo.*funcPtr)( 2 );  	
  return 0;
}
*/

  // main state machine
  for(unsigned int i=0; i<_states.size(); i++)
  {
    using namespace std;
    if(_states[i]._state == _cur_state)
    {
      // FIXME: just change this function pointer to class
      run_state_fptr run = _states[i]._fptr;
      (this->*run) ((int) port, (Packet *) pkt, (uint8_t) type);
      //hydra_chatter(8, _verbose, "%{element} : run_states : run 0x%x _cur_state %d\n", this, (this->*run), _cur_state);
      
      return;
    }
  } 

   // if there is no matching state just drop
   hydra_chatter(0, _verbose, "%{element} : run_states : never this case\n", this);
   pkt->kill();
   assert(false);

}


void GrPhy::run_state_idle(int port, Packet* pkt, uint8_t type)
{
  struct timeval cur_tv;
  click_gettimeofday(&cur_tv);
  switch(type)
  {
    case MPIMSG_TX_DATA:
      hydra_chatter(9, _verbose, "%{element} : run_state_idle : TX_DATA %d.%06d\n", this, cur_tv.tv_sec, cur_tv.tv_usec);
      _cur_state = STATE_TX;
      _tx_pkt = pkt->clone();
      _debug_tv.tv_sec = cur_tv.tv_sec;
      _debug_tv.tv_usec = cur_tv.tv_usec;
      _debug_tx_len = pkt->length() - sizeof(struct gr_txvector_new);
      output(port).push(pkt);
      break;
    case MPIMSG_CS_BUSY:
      hydra_chatter(9, _verbose, "%{element} : run_state_idle : CS_BUSY %d.%06d\n", this, cur_tv.tv_sec, cur_tv.tv_usec);
      _cur_state = STATE_RX;
      _debug_tv.tv_sec = cur_tv.tv_sec;
      _debug_tv.tv_usec = cur_tv.tv_usec;
      // FIXME:
      //legacy_on_from_phy_message(pkt);
      output(port).push(pkt);
      break;
    case MPIMSG_RX_DATA:
      hydra_chatter(8, _verbose, "%{element} : run_state_idle : RX_DATA : normal race condition\n", this);
      pkt->kill();
      break;
    case MPIMSG_TX_DONE:
    default: 
      hydra_chatter(0, _verbose, "%{element} : run_state_idle : TX_DONE : never this case\n", this);
      pkt->kill();
      assert(false);
  } 

}

void GrPhy::run_state_tx(int , Packet* pkt, uint8_t type)
{
  struct timeval cur_tv, elap_tv;
  click_gettimeofday(&cur_tv);
  
  /*
  {
    struct timeval start, stop, elap;

    char* a = new char[0xfffff];
    char* b = new char[0xfffff];

    click_gettimeofday(&start);

    for(unsigned int i=0; i<0xfffff; i++)
      b[i] = a[i];
    //memcpy((void *)b, (void *)a, 0xfffff);

    click_gettimeofday(&stop);
    delete [] a;
    delete [] b;

    elap = stop - start;
    printf("cp_time:%d:%d.%06d\n", 0xfffff, (int) elap.tv_sec, (int) elap.tv_usec);
  }
*/
  switch(type)
  {
    case MPIMSG_TX_DONE:
      hydra_chatter(9, _verbose, "%{element} : run_state_idle : TX_DONE %d.%06d\n", this, cur_tv.tv_sec, cur_tv.tv_usec);
      elap_tv = cur_tv - _debug_tv;
      hydra_chatter(9, _verbose, "++> tx_processing time = %d.%06d\n", elap_tv.tv_sec, elap_tv.tv_usec);
      //printf("tx_time:%d:%d.%06d\n", _debug_tx_len, (int) elap_tv.tv_sec, (int) elap_tv.tv_usec);
      _cur_state = STATE_IDLE;
      notify(0, MACListener::TX_DONE, _tx_pkt);
      _tx_pkt->kill();
      pkt->kill();
      break;
    case MPIMSG_CS_BUSY:
      hydra_chatter(8, _verbose, "%{element} : run_state_tx : CS_BUSY : normal race condition\n", this);
      pkt->kill();
      break;
    case MPIMSG_RX_DATA:
      hydra_chatter(8, _verbose, "%{element} : run_state_tx : RX_DATA : normal race condition\n", this);
      pkt->kill();
      break;
    case MPIMSG_TX_DATA:
    default: 
      hydra_chatter(0, _verbose, "%{element} : run_state_tx : TX_DATA : never this case\n", this);
      pkt->kill();
      assert(false);
  }
}

void GrPhy::run_state_rx(int port, Packet* pkt, uint8_t type)
{
  struct timeval cur_tv, elap_tv;
  click_gettimeofday(&cur_tv);
 

  switch(type)
  {
    case MPIMSG_RX_DATA:
      // FIXME: no need in final version
      hydra_chatter(9, _verbose, "%{element} : run_state_idle : RX_DATA %d.%06d\n", this, cur_tv.tv_sec, cur_tv.tv_usec);
      elap_tv = cur_tv - _debug_tv;
      _debug_rx_len = pkt->length() - sizeof(struct gr_rxvector_new);
      hydra_chatter(9, _verbose, "++> rx_processing time = %d.%06d\n", elap_tv.tv_sec, elap_tv.tv_usec);
      //printf("rx_time:%d:%d.%06d\n", _debug_rx_len, (int) elap_tv.tv_sec, (int) elap_tv.tv_usec);
      //legacy_on_from_phy_message(pkt);
      output(port).push(pkt);
      // FIXME: need to remove a following line in final version.
      push_cs_idle(port);
      _cur_state = STATE_IDLE;
      break;
    case MPIMSG_TX_DATA:
      hydra_chatter(9, _verbose, "%{element} : run_state_rx : TX_DATA %d.%06d\n", this, cur_tv.tv_sec, cur_tv.tv_usec);
      _cur_state = STATE_TX;
      _tx_pkt = pkt->clone();
      _debug_tv.tv_sec = cur_tv.tv_sec;
      _debug_tv.tv_usec = cur_tv.tv_usec;
      _debug_tx_len = pkt->length() - sizeof(struct gr_txvector_new);
      output(port).push(pkt);
      break;
    case MPIMSG_CS_BUSY:
      hydra_chatter(9, _verbose, "%{element} : run_state_rx : CS_BUSY : never this case\n", this);
      pkt->kill();
      assert(false);
    case MPIMSG_TX_DONE:
    default: 
      hydra_chatter(0, _verbose, "%{element} : run_state_rx : TX_DONE : never this case\n", this);
      pkt->kill();
      assert(false);
  } 
}

// Mimic it send cs idle message from phy
void GrPhy::push_cs_idle(int port)
{
  struct gr_rxvector_new rxv;
  memset(&rxv, 0x00, sizeof(struct gr_rxvector_new)-1);
  rxv.rate = MPIRATE_6M;
  struct mpitype* mt = (struct mpitype *) &rxv;
  mt->type = MPIMSG_CS_IDLE;

  WritablePacket* pkt_out = Packet::make(HYDRA_HEADROOM,
                                       (unsigned char *) &rxv, 
                                       sizeof(rxv)-1, 
                                       HYDRA_TAILROOM);
  //legacy_on_from_phy_message(pkt_out);
  hydra_chatter(9, _verbose, "%{element} : push_cs_idle : \n", this);
  output(port).push(pkt_out);
}

void GrPhy::legacy_on_to_phy_message(Packet* pkt)
{
  struct mpitype* mt = (struct mpitype *) pkt->data();
  switch(mt->type)
  {
    case MPITYPE_TX_DATA:
      mt->type = MPIMSG_TX_DATA;
      break;
    default:
      hydra_chatter(0, _verbose, "%{element} : legacy_on_to_phy : never this case\n", this);
  }

}

void GrPhy::legacy_on_from_phy_message(Packet* pkt)
{
  struct mpitype* mt = (struct mpitype *) pkt->data();
  switch(mt->type)
  {
    case MPIMSG_CS_BUSY:
      mt->type = MPITYPE_CCA_BUSY;
      break;
    case MPIMSG_RX_DATA:
      mt->type = MPITYPE_RX_DATA;
      break;
    case MPIMSG_CS_IDLE:
      mt->type = MPITYPE_CCA_IDLE;
      break;
    case MPIMSG_TX_DONE:
      break;
    default:
      hydra_chatter(0, _verbose, "%{element} : legacy_on_from_phy_message : never this case\n", this);
      assert(false);
  }

}

enum {H_VERBOSE};

String GrPhy::read_param(Element *e, void *vparam)
{
  GrPhy *fp = (GrPhy *)e;
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
      return String(fp->_verbose);
    default:
      return String();
  }

  return String();
}

int GrPhy::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  GrPhy *fp = (GrPhy *) e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      fp->_verbose = verbose;
      break; 
    }
  }

  return 0;
}

void
GrPhy::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(GrPhy)
