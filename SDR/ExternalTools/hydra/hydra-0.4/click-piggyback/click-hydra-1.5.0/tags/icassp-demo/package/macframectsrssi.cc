/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/macframectsrssi.cc,v $ 
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/hydra_anno.hh>

#include "macframectsrssi.hh"
CLICK_DECLS

MACFrameCTSRSSI::MACFrameCTSRSSI() : _cts_rssi_hdr_size(sizeof(struct cts_80211_rssi)), _rssi_len(CTS_RSSI_SIZE)
{

  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrameCTSRSSI::~MACFrameCTSRSSI()
{
}

void*
MACFrameCTSRSSI::cast(const char *n)
{
  if( strcmp(n, "MACFrameCTSRSSI") == 0 )
    return (MACFrameCTSRSSI *) this;

  return MACFrameCTS::cast(n);
}

int
MACFrameCTSRSSI::configure(Vector<String> &conf, ErrorHandler *errh)
{
  int res = MACFrameCTS::configure(conf, errh);
  if (res < 0)
    return res;

  memcpy((void *) &_cts_rssi_hdr, (void *) &_cts_hdr, _cts_hdr_size);
  memset((void *) &_cts_rssi_hdr.i_rssi, 0x0, _rssi_len);

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assume it receives complete data packet and generate cts frame
Packet *
MACFrameCTSRSSI::simple_action(Packet *p)
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

// expect RTS packet with full header
WritablePacket*
MACFrameCTSRSSI::prepare_packet(Packet* p)
{
  WritablePacket* p_out = MACFrameCTS::prepare_packet(p); 
  if(!p_out)
    return 0;

  p_out->put(_rssi_len); // make room for rssi
  
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p->all_user_anno();
  struct cts_80211_rssi* cts_rssi = (struct cts_80211_rssi *) p_out->data();

  hydra_chatter(8, _verbose, "%{element}::prepare_packet recved snr=%f", this, cha->avg_snr);
  // presentation formatting for inter node delivery.

  unsigned int rssi = (unsigned int) (cha->avg_snr * 100); 
  memcpy((void *)cts_rssi->i_rssi, (void *)&rssi, _rssi_len);
  hydra_chatter(8, _verbose, "%{element}::prepare_packet snr=%d", this, *(unsigned int *)cts_rssi->i_rssi);

  return p_out;
}

// Generate new packet 
WritablePacket*
MACFrameCTSRSSI::prepare_packet(Packet* p, EtherAddress& dst, double snr)
{
  assert(!p);

  WritablePacket* p_out = MACFrameCTS::prepare_packet(p, dst);
  if(!p_out)
    return 0;

  p_out->put(_rssi_len); // make room for rssi
  
  struct cts_80211_rssi* cts_rssi = (struct cts_80211_rssi *) p_out->data();

  hydra_chatter(8, _verbose, "%{element}::prepare_packet recved snr=%f", this, snr);
  // presentation formatting for inter node delivery.

  unsigned int rssi = (unsigned int) (snr * 100); 
  memcpy((void *)cts_rssi->i_rssi, (void *)&rssi, _rssi_len);
  hydra_chatter(8, _verbose, "%{element}::prepare_packet snr=%d", this, *(unsigned int *)cts_rssi->i_rssi);

  return p_out;
}


// set default header
WritablePacket* 
MACFrameCTSRSSI::set_header(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  p_out->push(_cts_rssi_hdr_size);
  memcpy(p_out->data(), &_cts_rssi_hdr, _cts_rssi_hdr_size);
  
  return p_out;
}

int 
MACFrameCTSRSSI::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _cts_rssi_hdr_size);
  return _cts_rssi_hdr_size;
}

int 
MACFrameCTSRSSI::get_feedback(Packet* p, void* feedback)
{
  if(!p || !feedback)
    return 0;

  struct cts_80211_rssi* cts_rssi = (struct cts_80211_rssi *) p->data();

  double snr = 0.0;
  unsigned int recv_rssi = 0;
  memcpy(&recv_rssi, cts_rssi->i_rssi, _rssi_len);
  snr = ((double) recv_rssi) /100;
  
  hydra_chatter(8, _verbose, "%{element}::get_feedback recved snr = %f\n", this, snr);
  memcpy(feedback, &snr, sizeof(double));

  return sizeof(double);
}

String
MACFrameCTSRSSI::read_param(Element *e, void *vparam)
{
  return MACFrameCTS::read_param(e, vparam);
}

int
MACFrameCTSRSSI::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  return MACFrameCTS::write_param(in_s, e, vparam, errh);
}

void
MACFrameCTSRSSI::add_handlers()
{
  MACFrameCTS::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrameCTSRSSI)
