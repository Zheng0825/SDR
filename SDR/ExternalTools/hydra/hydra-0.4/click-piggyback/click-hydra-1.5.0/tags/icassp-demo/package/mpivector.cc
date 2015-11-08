/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/mpivector.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>

#include "mpivector.hh"

CLICK_DECLS

MPIVector::MPIVector()
  : _hdr_size(0), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MPIVector::~MPIVector()
{
}

void*
MPIVector::cast(const char *n)
{
    if (strcmp(n, "MPIVector") == 0)
	return (MPIVector *) this;

    return 0;
}

int
MPIVector::configure(Vector<String> &conf, ErrorHandler *errh)
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
MPIVector::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!p || !p->length())
  {
    if(p) p->kill();
    return 0;
  }
  //WritablePacket* p_out = prepare_packet(p);
  Packet* p_out = prepare_packet(p);

  return p_out;
}

//WritablePacket*
Packet*
MPIVector::prepare_packet(Packet* p)
{
  //WritablePacket* p_out = p->uniqueify();
  Packet* p_out = p;

  return p_out;
}

String
MPIVector::read_param(Element *e, void *vparam)
{
  MPIVector *mf = (MPIVector *)e;
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
MPIVector::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  MPIVector *mf = (MPIVector *)e;
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
MPIVector::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MPIVector)
