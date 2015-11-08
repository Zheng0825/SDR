/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/setrtsthreshold.cc,v $
 *  
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>
#include <click/crc32.h>
#include <clicknet/ether.h>
#include <clicknet/80211.h>
//schoi +1
#include <clicknet/llc.h>

#include "ubaggregator.hh"

CLICK_DECLS

UBAggregator::UBAggregator()
    : _verbose(0), _max_broadcast(0), _max_unicast(0), _unicast_len(0),
      _broadcast_len(0), _prioritizer(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

UBAggregator::~UBAggregator()
{
  free(_broadcast_buffer);
  free(_unicast_buffer);
}

int
UBAggregator::configure(Vector<String> &conf, ErrorHandler *errh)
{
  Element *prioritizer_e;

  if (cp_va_parse(conf, this, errh,
          cpElement,  "packet prioritizer", &prioritizer_e,
          cpUnsigned, "maximum broadcast packets", &_max_broadcast,
          cpUnsigned, "maximum unicast packets", &_max_unicast,
		  cpOptional,
		  cpKeywords, 
 		  "VERBOSE", cpUnsigned, "verbose noise?", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  if (prioritizer_e) {
    _prioritizer = (Prioritizer *) prioritizer_e->cast("Prioritizer");
    if (!_prioritizer) return errh->error("Prioritizer must be uset using Prioritizer");
  } else {
    return errh->error("Prioritizer must be specified");
  }

  hydra_chatter(9, _verbose, "%{element}::configure max_b = %d, max_u = %d",
    this, _max_broadcast, _max_unicast);
  return 0;
}

int
UBAggregator::initialize(ErrorHandler *errh)
{
  _broadcast_buffer = (Packet **) calloc(_max_broadcast, sizeof(Packet *));
  _unicast_buffer = (Packet **) calloc(_max_unicast, sizeof(Packet *));
}

enum {H_VERBOSE, H_THRESHOLD};

String
UBAggregator::read_param(Element *e, void *thunk)
{
  UBAggregator *td = (UBAggregator *)e;
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
UBAggregator::write_param(const String &in_s, Element *e, void *vparam,
		      ErrorHandler *errh)
{
  UBAggregator *f = (UBAggregator *)e;
  String s = cp_uncomment(in_s);
  switch((int)vparam) 
  {
    case H_VERBOSE: 
    {
      unsigned m;
      if (!cp_unsigned(s, &m)) 
        return errh->error("stepup parameter must be unsigned");
      f->_verbose = m;
      break;
    }
  }

  return 0;

}
void
UBAggregator::add_handlers()
{
  add_read_handler("verbose?", read_param, (void *) H_VERBOSE);
  add_write_handler("verbose?", write_param, (void *) H_VERBOSE);
}

Packet *
UBAggregator::make_mpdu_with_pad(Packet *p_in, bool more_data)
{
  // Create the data header field
  struct data_80211 data_hdr_out;

  // Set the approrpriate flags for this packet.
  //*((unsigned short *)data_hdr_out.i_seq) = p_in->length();
  data_hdr_out.i_fc.type = FC0_TYPE_DATA;
  data_hdr_out.i_fc.subtype = FC0_SUBTYPE_DATA;
  data_hdr_out.i_fc.more_data = more_data;

  // Get the source and destination addresses
  click_ether *eh = (click_ether *)p_in->data();
  EtherAddress src = EtherAddress(eh->ether_shost);
  EtherAddress dst = EtherAddress(eh->ether_dhost);
  memcpy(data_hdr_out.i_addr1, dst.data(), ETHER_ADDR_LEN);
  memcpy(data_hdr_out.i_addr2, src.data(), ETHER_ADDR_LEN);

  //schoi +2
  uint16_t  ethtype = eh->ether_type;
  p_in->pull(sizeof(click_ether));
  
  // Copy the header to the packet
  // schoi -1 +10 to make ll header for ethtype
  //WritablePacket *p_out = p_in->push(sizeof(data_hdr_out));
  WritablePacket* p_out = p_in->push(sizeof(struct click_llc));
  struct click_llc *llc = (struct click_llc *) p_out->data();
  llc->llc_dsap = llc->llc_ssap = LLC_SNAP_LSAP;
  llc->llc_control = LLC_UI;
  llc->llc_un.type_snap.org_code[0] = 0;
  llc->llc_un.type_snap.org_code[1] = 0;
  llc->llc_un.type_snap.org_code[2] = 0;
  llc->llc_un.type_snap.ether_type = ethtype;

  p_out = p_in->push(sizeof(data_hdr_out));

  memcpy((void *)p_out->data(), &data_hdr_out, sizeof(data_hdr_out));
  ((struct data_80211 *)p_out->data())->i_seq[0] = p_out->length() & 0x00FF;
  ((struct data_80211 *)p_out->data())->i_seq[1] 
    = (p_out->length() & 0xFF00) >> 8;
  // Add CRC
  int len = p_out->length();
  unsigned int crc = 0xffffffff;
  crc = update_crc(crc, (char *) p_out->data(), len);
  p_out = p_out->put(sizeof(crc));
  memcpy(p_out->data() + len, &crc, sizeof(crc));
  len = len + sizeof(crc);

  // Pad with zeros.
  int padding_len;
  if((len%4)==0)
     padding_len=0;
  else 
     padding_len =4-(len % 4);
  
  p_out = p_out->put(padding_len);
  memset(p_out->data() + len, 0, padding_len);
  
  struct data_80211* hdr=(struct data_80211*) p_out->data();
  return p_out;
}

// Aggregate the packets in _broadcast_buffer and _unicast_buffer into
// a single buffer, allocate that buffer and return it through *agg_data.
// Return the length of the buffer through the function return value.
unsigned
UBAggregator::aggregate(unsigned char **agg_data, u_int32_t *uni_len,
                        u_int32_t *broad_len)
{
  unsigned len = 0;
  unsigned char *ptr;

  // Determine the length of the combined packets
  for (int i = 0; i < _broadcast_len; i++)
    len += _broadcast_buffer[i]->length();
  *broad_len = len;
  for (int i = 0; i < _unicast_len; i++)
    len += _unicast_buffer[i]->length();
  *uni_len = len - *broad_len;

  // Create a buffer for the combined packet, and copy the data into it
  *agg_data = (unsigned char *)malloc(len);
  ptr = *agg_data;
  for (int i = 0; i < _broadcast_len; i++)
    {
      unsigned pkt_len = _broadcast_buffer[i]->length();

      memcpy(ptr, _broadcast_buffer[i]->data(), pkt_len);
      ptr += pkt_len;
    }
  for (int i = 0; i < _unicast_len; i++)
    {
      unsigned pkt_len = _unicast_buffer[i]->length();

      memcpy(ptr, _unicast_buffer[i]->data(), pkt_len);
      ptr += pkt_len;
    }

  return len;
}

Packet *
UBAggregator::pull(int port)
{
  int i;

  // Pull packets from the broadcast queue:
  for (i = 0; i < _max_broadcast; i++)
    {
      _broadcast_buffer[i] = input(0).pull();
      if (!_broadcast_buffer[i])
        break;
    }
  _broadcast_len = i;
  
  // Pull packets from the unicase queue:
  if (_unicast_len == 0)
    {
      for (i = 0; i < _max_unicast; i++)
      {
        _unicast_buffer[i] = input(1).pull();
        if (!_unicast_buffer[i])
          break;
      }
      _unicast_len = i;
    }

  // Check to see if both buffers have zero length, if so, just return NULL
  if (_unicast_len == 0 && _broadcast_len == 0)
    return NULL;

  // Now, we need to wrap the packets into padded mpdus, taking care to set the
  // more_packets flag appropriately.  Again, we have 2 cases:
  //   - No unicast packets (so a broadcast packet has the flag unset)
  //   - Some unicast packets (so a unicast packet has the flag unset)
  if (_unicast_len == 0)
    {
      for (i = 0; i < _broadcast_len; i++)
        _broadcast_buffer[i] = make_mpdu_with_pad(_broadcast_buffer[i],
                                                  i != _broadcast_len - 1);
    }
  else
    {
      for (i = 0; i < _broadcast_len; i++)
        _broadcast_buffer[i] = make_mpdu_with_pad(_broadcast_buffer[i], true);

      for (i = 0; i < _unicast_len; i++)
        _unicast_buffer[i] = make_mpdu_with_pad(_unicast_buffer[i],
                                                i != _unicast_len - 1);
    }

  printf("[%13s][%04d] uni num:  %d, broad num: %d\n", __FILE__, __LINE__,
    _unicast_len, _broadcast_len);

  // Create the outgoing packet
  unsigned char *agg_data;
  u_int32_t uni_len, broad_len;
  unsigned agg_len = aggregate(&agg_data, &uni_len, &broad_len);
  Packet *psdu = Packet::make(HYDRA_HEADROOM, agg_data, agg_len,
                              HYDRA_TAILROOM);
  free(agg_data);

  // Create and set the annotation fields
  agg_anno_allocator(psdu, 0);
  struct click_hydra_anno *cha =
        (struct click_hydra_anno *) psdu->all_user_anno();
  cha->agg_anno->len_broadcast = broad_len;
  cha->agg_anno->len_unicast = uni_len;
  cha->agg_anno->has_unicast = (uni_len != 0);
  cha->agg_anno->num_unicast = _unicast_len; 
  cha->agg_anno->num_broadcast = _broadcast_len;
  
  // Kill all of the broadcast packets and unicast packets; they are now part
  // of the outgoing aggregated packet
  for (i = 0; i < _broadcast_len; i++)
    _broadcast_buffer[i]->kill();
  _broadcast_len = 0;

  for (i = 0; i < _unicast_len; i++)
    _unicast_buffer[i]->kill();
  _unicast_len = 0;

  // Return the outgoing packet
  return psdu;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(UBAggregator)

