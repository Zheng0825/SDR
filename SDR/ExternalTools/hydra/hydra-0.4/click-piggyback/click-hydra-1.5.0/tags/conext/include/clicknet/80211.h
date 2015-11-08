/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/include/clicknet/80211.h,v $ 
 *  
 *  Purpose	:
 *	    
 *  Copyright	:
 *
 *  Revisions	:
 *	    $Log: 80211.h,v $
 *	    Revision 1.5  2005/08/15 21:14:17  choiiss
 *	
 *	    Added CTS frame which carries RSSI information of receiver
 *	
 *	    Revision 1.4  2005/07/09 16:48:40  choiiss
 *	
 *	    Added CVS header
 *	
 *	
 ***********************************************************/

#ifndef _CLICKNET_80211_H_
#define _CLICKNET_80211_H_

// x is expected to be (unsigned char pointer)
#define IS_BROADCAST(x)  \
    ( x[0] == 0xff && x[1] == 0xff && x[2] == 0xff && \
      x[3] == 0xff && x[4] == 0xff && x[5] == 0xff ) 

#define ETHER_ADDR_LEN 	6
#define IEEE80211_MTU   2346


/*
 * generic definitions for IEEE 802.11 frames
 */

//FIXME: check endian (2 bytes)
CLICK_SIZE_PACKED_STRUCTURE(
struct fc_80211 {,
	u_int8_t	subtype			:4;
	u_int8_t	type			:2;
	u_int8_t	protocol_version	:2;

	u_int8_t	order			:1;
	u_int8_t	wep			:1;
	u_int8_t	more_data		:1;
	u_int8_t	pwr_mgt			:1;
	u_int8_t	retry			:1;
	u_int8_t	more_flag		:1;
	u_int8_t	to_from_ds		:2;
});

#define IEEE80211_LONG_TIMER 1
#ifndef IEEE80211_LONG_TIMER

CLICK_SIZE_PACKED_STRUCTURE(
struct common_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[2];
  	u_int8_t	i_ra[6];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct data_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[2];
	u_int8_t	i_addr1[6];
	u_int8_t	i_addr2[6];
	u_int8_t	i_addr3[6];
	u_int8_t	i_seq[2];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct rts_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[2];
	u_int8_t	i_ra[6];
	u_int8_t	i_ta[6];
}); 

CLICK_SIZE_PACKED_STRUCTURE(
struct cts_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[2];
	u_int8_t	i_ra[6];  // transmitter mac address
});

CLICK_SIZE_PACKED_STRUCTURE(
struct ack_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[2];
	u_int8_t	i_ra[6];  // transmitter mac address
});
#else
CLICK_SIZE_PACKED_STRUCTURE(
struct common_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[4];
  	u_int8_t	i_ra[6];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct data_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[4];
	u_int8_t	i_addr1[6];
	u_int8_t	i_addr2[6];
	u_int8_t	i_addr3[6];
	u_int8_t	i_seq[2];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct rts_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[4];
	u_int8_t	i_ra[6];
	u_int8_t	i_ta[6];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct cts_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[4];
	u_int8_t	i_ra[6];  // transmitter mac address
});

CLICK_SIZE_PACKED_STRUCTURE(
struct ack_80211 {,
	struct fc_80211 i_fc;
	u_int8_t	i_dur[4];
	u_int8_t	i_ra[6];  // transmitter mac address
});
#endif

CLICK_SIZE_PACKED_STRUCTURE(
struct common_rbar {,
	struct fc_80211 i_fc;
	u_int16_t	i_rate :4;
	u_int16_t	i_len  :12;
	u_int8_t	i_ra[6];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct data_rbar {,
	struct fc_80211 i_fc;
	u_int16_t	i_rate :4;
	u_int16_t	i_len  :12;
	u_int8_t	i_addr1[6];
	u_int8_t	i_addr2[6];
	u_int8_t	i_addr3[6];
	u_int8_t	i_seq[2];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct rts_rbar {,
	struct fc_80211 i_fc;
	u_int16_t	i_rate :4;
	u_int16_t	i_len  :12;
	u_int8_t	i_ra[6];
	u_int8_t	i_ta[6];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct cts_rbar {,
	struct fc_80211 i_fc;
	u_int16_t	i_rate :4;
	u_int16_t	i_len  :12;
	u_int8_t	i_ra[6];  // transmitter mac address
});

CLICK_SIZE_PACKED_STRUCTURE(
struct ack_rbar {,
	struct fc_80211 i_fc;
	u_int16_t	i_rate :4;
	u_int16_t	i_len  :12;
	u_int8_t	i_ra[6];  // transmitter mac address
});

#define CTS_RSSI_SIZE sizeof(unsigned short)

CLICK_SIZE_PACKED_STRUCTURE(
struct cts_80211_rssi {,
	struct cts_80211 cts;
	u_int8_t	i_rssi[CTS_RSSI_SIZE];
});

#define	FC0_VERSION_MASK		(0x03 >> 0)
#define	FC0_VERSION_0			(0x00 >> 0)

#define	FC0_TYPE_MASK			(0x0c >> 2)
#define	FC0_TYPE_MGT			(0x00 >> 2)
#define	FC0_TYPE_CTL			(0x04 >> 2)
#define	FC0_TYPE_DATA			(0x08 >> 2)

#define	FC0_SUBTYPE_MASK		(0xf0 >> 4)
/* for TYPE_MGT */
#define	FC0_SUBTYPE_ASSOC_REQ		(0x00 >> 4)
#define	FC0_SUBTYPE_ASSOC_RESP		(0x10 >> 4)
#define	FC0_SUBTYPE_REASSOC_REQ		(0x20 >> 4)
#define	FC0_SUBTYPE_REASSOC_RESP	(0x30 >> 4)
#define	FC0_SUBTYPE_PROBE_REQ		(0x40 >> 4)
#define	FC0_SUBTYPE_PROBE_RESP		(0x50 >> 4)
#define	FC0_SUBTYPE_BEACON		(0x80 >> 4)
#define	FC0_SUBTYPE_ATIM		(0x90 >> 4)
#define	FC0_SUBTYPE_DISASSOC		(0xa0 >> 4)
#define	FC0_SUBTYPE_AUTH		(0xb0 >> 4)
#define	FC0_SUBTYPE_DEAUTH		(0xc0 >> 4)
/* for TYPE_CTL */
#define	FC0_SUBTYPE_PS_POLL		(0xa0 >> 4)
#define	FC0_SUBTYPE_RTS			(0xb0 >> 4)
#define	FC0_SUBTYPE_CTS			(0xc0 >> 4)
#define	FC0_SUBTYPE_ACK			(0xd0 >> 4)
#define	FC0_SUBTYPE_CF_END		(0xe0 >> 4)
#define	FC0_SUBTYPE_CF_END_ACK		(0xf0 >> 4)
/* for TYPE_DATA (bit combination) */
#define FC0_SUBTYPE_DATA		(0x00 >> 4)
#define	FC0_SUBTYPE_CF_ACK		(0x10 >> 4)
#define	FC0_SUBTYPE_CF_POLL		(0x20 >> 4)
#define	FC0_SUBTYPE_CF_ACPL		(0x30 >> 4)
#define	FC0_SUBTYPE_NODATA		(0x40 >> 4)
#define	FC0_SUBTYPE_CFACK		(0x50 >> 4)
#define	FC0_SUBTYPE_CFPOLL		(0x60 >> 4)
#define	FC0_SUBTYPE_CF_ACK_CF_ACK	(0x70 >> 4)

#define	FC1_DIR_MASK			(0x03 >> 0) 
#define	FC1_DIR_NODS			(0x00 >> 0)	/* STA->STA */
#define	FC1_DIR_TODS			(0x01 >> 0)	/* STA->AP  */
#define	FC1_DIR_FROMDS			(0x02 >> 0)	/* AP ->STA */
#define	FC1_DIR_DSTODS			(0x03 >> 0)	/* AP ->AP  */

#define	FC1_MORE_FRAG			(0x04 >> 2)
#define	FC1_RETRY			(0x08 >> 3)
#define	FC1_PWR_MGT			(0x10 >> 4)
#define	FC1_MORE_DATA			(0x20 >> 5)
#define	FC1_WEP				(0x40 >> 6)
#define	FC1_ORDER			(0x80 >> 7)

#define	NWID_LEN			32

//FIXME: consider preamble and plcp length
static inline uint16_t txtime_usec(int length, int rate)
{
  //assert(length);
  //assert(rate);

  if (!rate || !length ) {
    return 0;
  }
  

  return  (length * 8)/ rate;

}

//FIXME: consider preamble and plcp length
static inline uint32_t txtime_usec_long(int length, int rate)
{
  //assert(length);
  //assert(rate);

  if (!rate || !length ) {
    return 0;
  }

  return  (length * 8)/ rate;
}

static inline bool is_rts(struct fc_80211& fc)
{
  return ((fc.type == FC0_TYPE_CTL) && 
          (fc.subtype == FC0_SUBTYPE_RTS));
}
 
static inline bool is_cts(struct fc_80211& fc)
{
  return ((fc.type == FC0_TYPE_CTL) && 
          (fc.subtype == FC0_SUBTYPE_CTS));
}
 
static inline bool is_ack(struct fc_80211& fc)
{
  return ((fc.type == FC0_TYPE_CTL) && 
          (fc.subtype == FC0_SUBTYPE_ACK));
} 

static inline bool is_data(struct fc_80211& fc)
{
  return ((fc.type == FC0_TYPE_DATA) && 
          (fc.subtype == FC0_SUBTYPE_DATA));
}
 
static inline bool is_unicast(struct common_80211& cmm)
{
  return (is_data(cmm.i_fc) && !IS_BROADCAST(cmm.i_ra) );
}

static inline bool is_broadcast(struct common_80211& cmm)
{
  return (is_data(cmm.i_fc) && IS_BROADCAST(cmm.i_ra) );
}

#endif /* !_CLICKNET_80211_H_ */
