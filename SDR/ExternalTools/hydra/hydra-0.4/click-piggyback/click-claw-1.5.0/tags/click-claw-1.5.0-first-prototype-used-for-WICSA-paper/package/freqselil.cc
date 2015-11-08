/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/glue.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/packet.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>
#include "freqselil.hh"

CLICK_DECLS

FreqSelIL::FreqSelIL() : _load_thresh(0.5), _cur_freq(400.0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

FreqSelIL::~FreqSelIL()
{
}

int
FreqSelIL::configure(Vector<String> &conf, ErrorHandler *errh)
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

int FreqSelIL::initialize(ErrorHandler *) 
{
  struct timeval now;
  click_gettimeofday(&now);

  change_state(CS_IDLE, now);
  
  return 0;
}

void
FreqSelIL::push(int port, Packet *pkt)
{
  hydra_chatter(8, _verbose, "%{element}::push:", this);
  if(port == LOAD_PORT)
  {
    process_load(port, pkt);
  }
  else if(port == FREQ_PORT)
  {
    process_freq(port, pkt);
  }

}

void FreqSelIL::process_load(int port, Packet* pkt)
{
  struct timeval now;
  click_gettimeofday(&now);

  struct click_hydra_anno *cha = (struct click_hydra_anno *) pkt->all_user_anno();
  // FIXME: It assume that CS_BUSY is followed by CS_IDLE and vice versa.
  //        It didn't consider the case CS_BUSY followed by CS_BUSY .
  if( cha->type == HYDRA_CCA_BUSY)
  {
    if(_cur_state == CS_BUSY)
      goto wrong_state;

    change_state(CS_BUSY, now);
  }
  else if (cha->type == HYDRA_CCA_IDLE)
  {
    if(_cur_state == CS_IDLE)
      goto wrong_state;

    change_state(CS_IDLE, now);
  }
  else
  {
    goto wrong_state;
  }

  output(LOAD_PORT).push(pkt);

  _load = calc_load();
  _cur_freq = check_load();
  return;

wrong_state:
   hydra_chatter(0, _verbose, "%{element}::push: invalid packet", this);
   pkt->kill();
   return;

}

void FreqSelIL::process_freq(int port, Packet* pkt)
{
  float freq = _cur_freq;
  unsigned char req_type = unmarshall_freq(pkt, &freq);

  if(req_type == GET_FREQ)
    _cur_freq = freq;

  pkt->kill();

}

float FreqSelIL::check_load()
{
  float freq = _cur_freq;

  if(_load > _load_thresh)
  {
    if(_cur_freq == 400.0)
      freq = 410.0;
    if(_cur_freq == 410.0)
      freq = 400.0;

    Packet* freq_pkt = marshall_freq(SET_FREQ, &freq);
    output(FREQ_PORT).push(freq_pkt);
  }

  return (_cur_freq = freq);
}

unsigned char FreqSelIL::unmarshall_freq(Packet* pkt, float* freq)
{
  char* buf = (char *) pkt->data();
  unsigned char req_type;

  sscanf(buf, "%02d%4.1f", req_type, *freq);

  return req_type;
}

Packet* FreqSelIL::marshall_freq(unsigned char req_type, float* freq)
{
  char buf[30];
  buf[0] = req_type;
  sprintf(buf, "%02d%4.1f\0", req_type, *freq);

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, (unsigned char *)buf, strlen(buf), HYDRA_TAILROOM);

  return (Packet*) p_out;
}

enum {H_VERBOSE};

String
FreqSelIL::read_param(Element *e, void *vparam)
{
  FreqSelIL *tp = (FreqSelIL *)e;
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
      return String(tp->_verbose);
    default:
      return "";
  }

  return String();
}

int
FreqSelIL::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  FreqSelIL *tp = (FreqSelIL *)e;
  String s = cp_uncomment(in_s);
  switch( (intptr_t)vparam )
  {
    case H_VERBOSE:
    {
      unsigned verbose;
      if(!cp_unsigned(s, &verbose))
	return errh->error("verbose must be boolean");
      tp->_verbose = verbose;
      break; 
    }
    default:
      break;
  }

  return 0;
}

void
FreqSelIL::add_handlers()
{
  add_read_handler("verbose", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose", write_param, (void *) H_VERBOSE);
}

EXPORT_ELEMENT(FreqSelIL)

#include <click/dequeue.cc>
CLICK_ENDDECLS
