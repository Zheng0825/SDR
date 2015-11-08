/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/simchannelper.cc,v $ 
 *  
 ***********************************************************/
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/packet_anno.hh>
#include <math.h>
//#include "elements/grid/filterbyrange.hh"

#include "simchannelper.hh"
CLICK_DECLS

SimChannelPER::SimChannelPER()
  :  _ber(0.0), _per(0.0), _success(0), _drop(0)
{
}

SimChannelPER::~SimChannelPER()
{
}

int
SimChannelPER::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int i;

  _nodes.clear();
  for(i = 0; i < conf.size(); i++)
  {
    String kw;
    String rest;
    if(cp_keyword(conf[i], &kw, &rest))
    {
      if(kw == "BER" || kw =="PER")
      {
        int error_rate = 0;
        if(!cp_real10(rest, 7, &error_rate) )
          return errh->error("could not parse error rate");

        if(kw == "BER") _ber = ((double)error_rate) / 10000000.0;
        else _per = ((double)error_rate) / 10000000.0;

        continue;
      }
    }

    Vector<String> words;
    cp_spacevec(conf[i], words);
    if(words.size() != 2)
      return errh->error("argument %d doesn't have lat and lon, it's size = %d", i, words.size() );

    int xlat, xlon;
    if(!cp_real10(words[0], 5, &xlat) || !cp_real10(words[1], 5, &xlon))
      return errh->error("could not parse lat or lon from arg %d", i);
    Node n;
    n._lat = ((double)xlat) / 100000.0;
    n._lon = ((double)xlon) / 100000.0;
    _nodes.push_back(n);
   
  }

  return 0;
}


int
SimChannelPER::initialize(ErrorHandler *errh)
{
  if(_ber)
    click_chatter("BER = %f", _ber);
  else if(_per)
    click_chatter("PER = %f", _per);

  click_random_srandom();

  return SimChannel::initialize(errh);
}

void
SimChannelPER::push(int port, Packet* p)
{
  assert(p);
  int in = port;

  for(int out = 0; out < noutputs(); out++)
  {
    if( in == out)
      continue;

    double r;
    double dx = _nodes[in]._lat - _nodes[out]._lat;
    double dy = _nodes[in]._lon - _nodes[out]._lon;
    r = sqrt(dx*dx + dy*dy);

    if (r < _tx_range)
    {
      Packet* p_out = p->clone();
      
      if(reception(p_out))
      {
        SET_PAINT_ANNO(p_out, RECV_DATA);
        _success++;
      }
      else
      {
        SET_PAINT_ANNO(p_out, RECV_CS);
        _drop++;
      }

      click_chatter("forward = %d drop = %d, tot = %d", _success, _drop, _success + _drop);
      output(out).push(p_out);
    }
    else if (r < _cs_range)
    {
      Packet* p_out = p->clone();
      SET_PAINT_ANNO(p_out, RECV_CS);
      output(out).push(p_out);
    }
  }
  p->kill();

  return;
}

bool
SimChannelPER::reception(Packet* p)
{
  double per = _per;

  if(per == 0.0)
  {
    per = ((double)(p->length() << 3)) * _ber;
  }

  double rv = ((double) (random())) / ((double)0x8fffffff);
  click_chatter("random number = %f", rv);
  
  return (rv >= per) ? true : false ;
}

// Expects a line that looks like
// node-index latitude longitude
int
SimChannelPER::rs_write_handler(const String &arg, Element *element,
			   void * dummy, ErrorHandler *errh)
{
  return SimChannel::rs_write_handler(arg, element, dummy, errh);
}

String
SimChannelPER::rs_read_handler(Element *f, void *dummy)
{
  return SimChannel::rs_read_handler(f, dummy);
}

void
SimChannelPER::add_handlers()
{
  add_write_handler("loc", rs_write_handler, (void *) 0);
  add_read_handler("loc", rs_read_handler, (void *) 0);
  //add_read_handler("stat", rs_stat_handler, (void *) 0);
}

//ELEMENT_REQUIRES(FilterByRange)
EXPORT_ELEMENT(SimChannelPER)

CLICK_ENDDECLS
