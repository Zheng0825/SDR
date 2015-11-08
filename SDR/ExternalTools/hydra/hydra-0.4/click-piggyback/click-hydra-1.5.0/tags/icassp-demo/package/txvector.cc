/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/txvector.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>

#include <clicknet/mpi.h>
#include "txvector.hh"
CLICK_DECLS

TXVector::TXVector()
  : _hdr_size(sizeof(struct txvector))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

TXVector::~TXVector()
{
}

void*
TXVector::cast(const char *n)
{
    if (strcmp(n, "TXVector") == 0)
	return (TXVector *) this;
    
    return MPIVector::cast(n);
}

int
TXVector::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
 			cpOptional,
			cpKeywords,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}


Packet *
TXVector::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!p || !p->length())
  {
    hydra_chatter(0, _verbose, "%{element}::simple_action: wrong packet", this);
    if(p) p->kill();
    return 0;
  }

  //WritablePacket* p_out = prepare_packet(p);
  Packet* p_out = prepare_packet(p);
  // never use manipulate or free a packet after calling its uniqueify 
  //p->kill();

  return p_out;
}

//WritablePacket*
Packet*
TXVector::prepare_packet(Packet* p)
{
  hydra_chatter(8, _verbose, "%{element}::prepare_packet:", this);
  //WritablePacket* p_out = p->uniqueify();
  Packet* p_out = p;
  
  p_out->push(_hdr_size);
  struct txvector* txv = (struct txvector *) p_out->data();

  struct click_hydra_anno* cha = 
		(struct click_hydra_anno *) p_out->all_user_anno();

  txv->type = convert_type(cha->type);
  txv->length = p_out->length() - _hdr_size;
  txv->rate = convert_rate(cha->rate_mbps); 
  txv->service = 0;
  //txv->txpwr = cha->power;  
  txv->txpwr = 0;

  //FIXME: I change the bit alignment based on MPI vis in LABVIEW.
  //       For baseband test just use this, 
  //       but eventually LABVIEW must be changed to above alignment.
  //       Or, just use 8bit boundary for simplicity.
/*  
  uint8_t* tmp = (uint8_t *) txv;
  tmp[1] = ((p_out->length() - _hdr_size) >> 4 ) & 0xff;
  tmp[2] = (((p_out->length() - _hdr_size) << 4 ) & 0xf0) | 
           (convert_rate(cha->rate_mbps) & 0x0f);
*/  
  return p_out;
}

uint8_t
TXVector::convert_rate(uint16_t anno_rate)
{
  uint16_t rate = 0;

  switch(anno_rate)
  {
    case 6: rate = MPIRATE_6M; break;
    case 9: rate = MPIRATE_9M; break;
    case 12: rate = MPIRATE_12M; break;
    case 18: rate = MPIRATE_18M; break;
    case 24: rate = MPIRATE_24M; break;
    case 36: rate = MPIRATE_36M; break;
    case 48: rate = MPIRATE_48M; break;
    case 54: rate = MPIRATE_54M; break;
  }
  return rate;
}

uint8_t
TXVector::convert_type(uint8_t anno_type)
{
  uint8_t txv_type = 0;
  switch(anno_type)
  {
    // FIXME: some element must change RX_DATA to TX_DATA
    case HYDRA_RX_DATA: 
    case HYDRA_TX_DATA: 
	txv_type = MPITYPE_TX_DATA; 
	break;
    default:
      hydra_chatter(0, _verbose, "%{element}::convert_type: never this case %d", this, anno_type);
      break;
  }
  return txv_type;
}

String
TXVector::read_param(Element *e, void *vparam)
{
  TXVector *mf = (TXVector *)e;
  switch( (intptr_t)vparam )
  {
    case 0:
      return String(mf->_verbose);
    default:
      return String();
  }
  return String();
}

int
TXVector::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  TXVector *mf = (TXVector *)e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case 0:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      mf->_verbose = verbose;
      break; 
    }
  }

  return 0;
}

void
TXVector::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TXVector)
