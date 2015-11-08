/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/include/click/hydra_anno.hh,v $ 
 *  
 ***********************************************************/
#ifndef _CLICK_HYDRAANNO_H_
#define _CLICK_HYDRAANNO_H_

#include <click/vector.cc>
#include <click/packet.hh>

#define HYDRA_MAGIC    0x802b
#define HYDRA_HEADROOM	   40
#define HYDRA_TAILROOM	   40

// for flags
enum HydraAnnoFlags
{
  HYDRA_DO_RTS     = (1<<0),
};

enum HydraAnnoType 
{
  HYDRA_CCA_BUSY,
  HYDRA_CCA_IDLE,
  HYDRA_CCA_EIDLE,
  HYDRA_TX_DATA,
  HYDRA_RX_DATA,
};

enum HydraModulation
{
  BPSK,
  QPSK,
  QAM16,
  QAM64,
  QAM256,
};

enum HydraCodingRate
{
   one,
   one_second,
   one_third,
   two_third,
   one_forth,
   two_forth,
   three_forth,
};

#define HYDRA_ALL_CHANNEL 0xffff
CLICK_SIZE_PACKED_STRUCTURE(
struct click_hydra_rate {,
  uint32_t channel_number;
  uint16_t modulation;
  uint16_t coding_rate;
});

CLICK_SIZE_PACKED_STRUCTURE(
struct click_hydra_power {,
  uint32_t channel_number;
  uint16_t rx_ssi;
  uint16_t tx_power;
});

CLICK_SIZE_PACKED_STRUCTURE(
struct click_hydra_channel {,
  double real;
  double imaginary;
});

typedef Vector<struct click_hydra_rate> RateVector;
typedef Vector<struct click_hydra_power> PowerVector;
typedef Vector<struct click_hydra_channel> ChannelVector;

/*
CLICK_SIZE_PACKED_STRUCTURE(
struct click_hydra_anno {,

  u_int8_t type;
  u_int8_t magic;
  u_int16_t flags;

  u_int8_t dst[6];
  u_int8_t src[6];

  u_int32_t defer_usec;
  u_int32_t slot;

  //RateVector rate_vector;  // vector is 12 bytes
  u_int16_t rate_mbps;
  u_int16_t rrate_mbps;

  //PowerVector power_vector;
  u_int16_t power;
  u_int16_t rssi;

  //ChannelVector channel_vector;

  u_int16_t mac_type;

  u_int16_t rate_data_frame;
  u_int16_t len_data_frame;

  double avg_snr;   // to receive SNR from PHY
  //u_int8_t retries;
  //u_int16_t len;

});
*/

CLICK_SIZE_PACKED_STRUCTURE(
struct click_agg_anno {,
  u_int32_t len_broadcast;  // Number of bytes
  u_int32_t len_unicast;    // Number of bytes
  u_int32_t len_metadata;
  u_int8_t  has_unicast;   //used by aggregation
  u_int8_t  firstframe;    //used for MAC RX
  u_int8_t  lastframe;     //used for MAC RX
  u_int8_t  num_unicast;   // # of unicast
  u_int8_t  num_broadcast; // # of broadcast 
  // This one byte of array is used for a pointer to a whole
  // metadata memory area.
  // If you allocate a memory with sizeof(click_aggre_anno) -1 + len_metadata,
  // after this metadata[0], we will have (len_metadata-1) of memory space.
  // Then we can use this metadata, as a char* that points that memory space.
  char metadata[1];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct click_hydra_anno {,

  // + 4 bytes
  u_int8_t type;
  u_int8_t magic;
  u_int16_t flags;

  // 2 * 6 bytes  -> 16 bytes
  u_int8_t dst[6];
  u_int8_t src[6];

  // 2*4 bytes -> 24 bytes
  u_int32_t defer_usec;
  u_int32_t slot;

  // 2 bytes -> 26 bytes
  u_int16_t rate_mbps;

  //u_int16_t rrate_mbps;
  //u_int16_t dummy5; 

  // 4 bytes -> 30 bytes
  struct click_agg_anno* agg_anno;

  // 2 bytes -> 32 bytes
  u_int16_t mac_type;

  // 4 bytes -> 36 bytes
  u_int16_t rate_data_frame;
  u_int16_t len_data_frame;

  // 8 bytes -> 44 bytes
  double avg_snr;   // to receive SNR from PHY
  //u_int8_t retries;
  //u_int16_t len;

  //PowerVector power_vector;
  // 4 bytes -> 48 bytes
  u_int16_t power;
  u_int16_t rssi;

  //available anno length is 48 bytes

});

static void agg_anno_allocator(Packet* pkt, int len_metadata)
{
  if(len_metadata == 0) len_metadata = 1;

  //printf("IA: 1 = 0x%x\n", pkt);
  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();

  //printf("IA: 2\n");

  size_t alloc_size = sizeof(struct click_agg_anno) - 1 + len_metadata;

  //printf("IA: 3\n");
 
  cha->agg_anno = (struct click_agg_anno *) malloc(alloc_size); 

  //printf("IA: 4 = 0x%x\n", cha->agg_anno);
  
}

// This function will be called when packet destructor is called.
// To make this function to be called, please use
// pkt->set_hydra_destructor(agg_anno_destructor)
static void agg_anno_destructor(Packet* pkt)
{
  //printf("IA: 1 = 0x%x\n", pkt);
  struct click_hydra_anno* cha = (struct click_hydra_anno *) pkt->all_user_anno();

  //printf("IA: 2 = 0x%x\n", cha->agg_anno);
  if(cha->agg_anno)
    free(cha->agg_anno);

  cha->agg_anno = 0;
  //printf("IA: 3 = 0x%x\n", cha->agg_anno);
  
}


#ifndef hydra_chatter
#define hydra_chatter(x, verbose, fmt...) \
{ \
  if(x == 0) { click_chatter(fmt); } \
  else if(x < (verbose+1)) { click_chatter(fmt); }\
}
#endif

#endif /* !_CLICK_HYDRA_ANNO_H_ */
