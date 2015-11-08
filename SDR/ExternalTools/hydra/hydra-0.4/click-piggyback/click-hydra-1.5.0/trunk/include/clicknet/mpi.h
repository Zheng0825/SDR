/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 * 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 * 
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/include/clicknet/mpi.h,v $ 
 *  
 ***********************************************************/
#ifndef _CLICKNET_MPI_H_
#define _CLICKNET_MPI_H_

/*
enum MPITypeTable {
#ifdef NEW_MPI
	MPITYPE_TX_DATA,
  	MPITYPE_TX_NODATA,
  	MPITYPE_TX_REQ,
  	MPITYPE_RX_DATA,
  	MPITYPE_CCA_IDLE,
  	MPITYPE_CCA_BUSY,
	MPITYPE_TX_ACK,
	MPITYPE_RX_ACK
#else
	MPITYPE_TX_DATA,
	MPITYPE_TX_ACK,
  	MPITYPE_RX_DATA,
	MPITYPE_RX_ACK,
  	MPITYPE_CCA_IDLE,
  	MPITYPE_CCA_BUSY,
  	MPITYPE_TX_NODATA,
  	MPITYPE_TX_REQ
#endif

};
*/

// For compatibility
enum MPITypeTable {
        // For MPI with gnu radio
        MPIMSG_TX_DATA = 0,
        MPIMSG_TX_DONE = 1,
        MPIMSG_RX_DATA = 2,
        MPIMSG_CS_BUSY = 3,
        MPIMSG_CS_IDLE = 4,

        // For two kinds of MPI with Labview
	MPITYPE_TX_DATA = 0,
	MPITYPE_TX_ACK = 1,
  	MPITYPE_TX_NODATA = 1,
  	MPITYPE_RX_DATA = 2,
	MPITYPE_RX_ACK = 3,
  	MPITYPE_TX_REQ = 3,
  	MPITYPE_CCA_IDLE = 4,
  	MPITYPE_CCA_BUSY = 5,
	MPITYPE_TX_DONE = 100,
	MPITYPE_RX_START = 101
};

// For 2X2 MIMO, more 8 MCSs are added.
enum MPIRateTable {
	MPIRATE_6M = 0, 
	MPIRATE_9M,
	MPIRATE_12M,
	MPIRATE_18M,
	MPIRATE_24M,
	MPIRATE_36M,
	MPIRATE_48M,
	MPIRATE_54M,
        MPIRATE_13M,
        MPIRATE_26M,
        MPIRATE_39M,
        MPIRATE_52M,
        MPIRATE_78M,
        MPIRATE_104M,
        MPIRATE_117M,
        MPIRATE_130M,
        MPIRATE_COUNT
};

enum MPICodingRateTable {
	MPI_CODINGRATE_1_2 = 0, 
	MPI_CODINGRATE_2_3, 
	MPI_CODINGRATE_3_4, 
	MPI_CODINGRATE_COUNT 
};

enum MPIConstellationTable {
	MPI_CONSTELLATION_BPSK = 0,
	MPI_CONSTELLATION_QPSK,
	MPI_CONSTELLATION_QAM16,
	MPI_CONSTELLATION_QAM64,
	MPI_CONSTELLATION_COUNT
};

CLICK_SIZE_PACKED_STRUCTURE(
struct mpitype {,
  	u_int32_t	type;
});

// for regacy
CLICK_SIZE_PACKED_STRUCTURE(
struct txvector {,
  	u_int8_t	type;
	u_int16_t	length 	:12;
        u_int16_t	rate 	:4;

	u_int16_t	service;
	u_int8_t	txpwr;
});


CLICK_SIZE_PACKED_STRUCTURE(
struct gr_txvector {,
  	u_int8_t	type;
	u_int16_t	length;
        u_int16_t	rate;
	float		txpwr;
});


// new txvector for data aggregation
CLICK_SIZE_PACKED_STRUCTURE(
struct gr_txvector_new {,
  	u_int32_t	type;
	u_int32_t       len_metadata;
        u_int32_t       len_broadcast;
        u_int32_t       len_unicast;
        u_int32_t	rate;
	float		txpwr;
        char            metadata[1];
});

CLICK_SIZE_PACKED_STRUCTURE(
struct gr_txvector_nometa {,
  	u_int32_t	type;
	u_int32_t       len_metadata;
        u_int32_t       len_broadcast;
        u_int32_t       len_unicast;
        u_int32_t	rate;
	float		txpwr;
});

/*
#define MAX_CHANNEL_INFO 128
CLICK_SIZE_PACKED_STRUCTURE(
struct channel_info {,
        double		real;
        double		imaginary;
});
*/

// for regacy
CLICK_SIZE_PACKED_STRUCTURE(
struct rxvector {,
  	u_int8_t	type;
	u_int16_t	length 	:12;
        u_int16_t	rate 	:4;

	u_int16_t	service;
	u_int8_t	rssi;
 	u_int16_t	avg_snr;   // for snr reception
	//u_int8_t	info_size;

        //struct channel_info ch_info[MAX_CHANNEL_INFO];
});


CLICK_SIZE_PACKED_STRUCTURE(
struct gr_rxvector {,
  	u_int8_t	type;
	u_int16_t	length;
        u_int16_t	rate;
	float		snr;
});


CLICK_SIZE_PACKED_STRUCTURE(
struct gr_rxvector_new {,
  	u_int32_t	type;
	u_int32_t       len_metadata;
        u_int32_t       len_broadcast;
        u_int32_t       len_unicast;
        u_int32_t	rate;
	float		snr;
        char            metadata[1];
});
#endif /* !_CLICKNET_MPI_H_ */
