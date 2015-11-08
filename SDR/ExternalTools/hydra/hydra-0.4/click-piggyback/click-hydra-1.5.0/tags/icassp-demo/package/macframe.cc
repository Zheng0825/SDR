/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframe.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>

#include "macframe.hh"
CLICK_DECLS

MACFrame::MACFrame() : _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrame::~MACFrame()
{
}


void*
MACFrame::cast(const char *n)
{
    if (strcmp(n, "MACFrame") == 0)
	return (MACFrame *) this;

    return 0;
}

int
MACFrame::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = cp_va_parse(conf, this, errh,
 			cpOptional,
			cpKeywords,
			"MACADDR", cpEtherAddress, "local MAC address", &_mac_addr,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);
  
  if (res < 0)
    return res;

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}


Packet *
MACFrame::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!p || !p->length())
  {
    if(p) p->kill();
    return 0;
  }
  WritablePacket* p_out = prepare_packet(p);
  p->kill();

  return p_out;
}

WritablePacket*
MACFrame::prepare_packet(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  return p_out;
}

WritablePacket*
MACFrame::prepare_packet(Packet* p, EtherAddress&)
{
  WritablePacket* p_out = p->uniqueify();

  return p_out;
}

const unsigned char*
MACFrame::dst(Packet* )
{
  return 0;
}

const unsigned char*
MACFrame::src(Packet* )
{
  return 0;
}

WritablePacket* 
MACFrame::set_header(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  //p_out->push(size);

  //memset(p_out->data(), 0, size);
  //memcpy(p_out->data(), hdr, size);
  return p_out;
}

int 
MACFrame::get_header(Packet*, void*)
{
  int size = 0;
  // hdr = (struct mac_header *)p->data;
  // size = sizeof(struct mac_header);
  return size;
}

String
MACFrame::read_param(Element *e, void *vparam)
{
  MACFrame *mf = (MACFrame *)e;
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
MACFrame::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  MACFrame *mf = (MACFrame *)e;
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
MACFrame::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) 0);
  add_write_handler("verbose", write_param, (void *) 0);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrame)
