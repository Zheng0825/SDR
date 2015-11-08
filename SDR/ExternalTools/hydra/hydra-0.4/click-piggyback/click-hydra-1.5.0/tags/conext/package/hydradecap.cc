/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/hydradecap.cc,v $ 
 *  
 ***********************************************************/
#include <click/config.h>
#include <click/etheraddress.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <clicknet/llc.h>

#include <click/hydra_anno.hh>

#include "macframe.hh"
#include "hydradecap.hh"
CLICK_DECLS

HydraDecap::HydraDecap() 
  : _mac_frame(0), _strict(false), _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

HydraDecap::~HydraDecap()
{
}

int 
HydraDecap::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* e = 0;
  if (cp_va_parse(conf, this, errh,
		  cpElement, "generate data header for mac protocol", &e,
		  /* not required */
		  cpOptional,
		  cpKeywords,
		  "VERBOSE", cpUnsigned, "for debug", &_verbose,
		  "STRICT", cpBool, "strict header check", &_strict,
		  cpEnd) < 0)
    return -1;

  if(e) {
    _mac_frame = (MACFrame *) e->cast("MACFrame");
    if(!_mac_frame) return errh->error("mac frame generator must be set using MACFrame"); 
  } else { 
    return errh->error("mac frame generater must be defined");
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
HydraDecap::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  
  if (p->length() <  _mac_frame->header_size() + sizeof(struct click_llc)) 
  {
    p->kill();
    return 0;
  }

   
  struct click_hydra_anno *cha = (struct click_hydra_anno *) p->all_user_anno();
  memset(cha, 0, sizeof(struct click_hydra_anno));

  EtherAddress dst =  EtherAddress(_mac_frame->dst(p));
  EtherAddress src = EtherAddress(_mac_frame->src(p));

  WritablePacket *p_out = p->uniqueify();
  if (!p_out) 
  {
    return 0;
  }

  p_out->pull(_mac_frame->header_size());
  
  struct click_llc *llc = (struct click_llc *) p_out->data();

  uint16_t ether_type;
  if (!_strict || 
      (llc->llc_dsap == LLC_SNAP_LSAP && llc->llc_ssap == LLC_SNAP_LSAP &&
       llc->llc_un.type_u.control == LLC_UI && llc->llc_un.type_snap.org_code[0] == 0 &&
       llc->llc_un.type_snap.org_code[1] == 0 && llc->llc_un.type_snap.org_code[2] == 0)) {
    ether_type = llc->llc_un.type_snap.ether_type;
    p_out->pull(sizeof(struct click_llc));
  } else {
    p_out->kill();
    return 0;
  }

  p_out = p_out->push_mac_header(14);
  if (!p_out) {
    return 0;
  }

  memcpy(p_out->data(), dst.data(), 6);
  memcpy(p_out->data() + 6, src.data(), 6);
  memcpy(p_out->data() + 12, &ether_type, 2);


  hydra_chatter(5, _verbose, "%{element}: src %s dst %s ethtype %d\n",
		  this,
		  src.s().c_str(),
		  dst.s().c_str(),
		  ether_type);

  return p_out;
}


enum {H_VERBOSE};

String 
HydraDecap::read_param(Element *e, void *thunk)
{
  HydraDecap *td = (HydraDecap *)e;

  switch ((uintptr_t) thunk) 
  {
    case H_VERBOSE:
	return String(td->_verbose) + "\n";
    default:
      return String();
  }

  return String();
}

int 
HydraDecap::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  HydraDecap *f = (HydraDecap *)e;
  String s = cp_uncomment(in_s);
  switch((int)vparam) 
  {
    case H_VERBOSE: 
    {    
      unsigned verbose;
      if (!cp_unsigned(s, &verbose)) 
        return errh->error("verbose parameter must be boolean");
      f->_verbose = verbose;
      break;
    } 
  }
  return 0;
}
 
void
HydraDecap::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);

  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}
CLICK_ENDDECLS
EXPORT_ELEMENT(HydraDecap)
