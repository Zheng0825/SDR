/* -*- c++ -*- */
/* 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_11N_PHY_txvector_H
#define INCLUDED_11N_PHY_txvector_H

#include <itpp/itbase.h>

#ifndef INCLUDED_11N_PHY_rxvector_H 
// don't redefine types shared by txvector and rxvector
enum non_ht_modulation_type {ERP_DSSS=0, ERP_CCK=1, ERP_OFDM=2, ERP_PBCC=3, DSSS_OFDM=4, OFDM_=5};
enum format_type {NON_HT=0,HT_MF=1,HT_GF=2};
enum preamble_type_type {SHORTPREAMBLE=0,LONGPREAMBLE=1};
enum ch_bandwidth_type {HT_CBW20=0,HT_CBW40=1,NON_HT_CBW20=2,NON_HT_CBW40=3};
enum smoothing_type {SMOOTHING_REC=0,SMOOTHING_NOT_REC=1};
enum sounding_type {SOUNDING=0,CONST_NOT_SOUNDING=1};
enum aggregation_type {AGGREGATED=0,NOT_AGGREGATED=1};
enum ldpc_coding_type {BCC_CODING=0,CONST_LDPC_CODING=1};
enum short_gi_type {LONG_GI=0,CONST_SHORT_GI=1};
enum modulation_type {QAM_2=0,QAM_4=1,QAM_16=2,QAM_64=3};
enum coding_rate_type {R_1_2=0,R_2_3=1,R_3_4=2,R_5_6=3};
#endif

// types unique to txvector
enum ch_offset_type {CH_OFF_20=0,CH_OFF_40=1,CH_OFF_20U=2,CH_OFF_20L=3};
enum antenna_set_on_type {CONST_ANTENNA_SET_ON=0,NO_ANTENNA_SET_ON=1};
enum expansion_mat_type_type {NO_EXPANSION_MAT=0,COMPRESSED_SV=1,NON_COMPRESSED_SV=2,CSI_MATRICES=3};


/*!
 * \brief MIMO TxVector object.
 *
 * Data is stored internally as a gr_message object. This general formulation of
 * TxVector allows a great deal of flexibility in writing and reading TxVector
 * objects.
 */

class txvector
{

	public:
		txvector();
		
		// ============================================================
		// ---------- Functions to control standard types -------------
		// ============================================================
		// 'calculate parameters': set the values of the protected variables
		// 'check_txvector_integrity': 'true' = ensure the TXVECTOR private
		// variables make sense together
		void calculate_parameters(bool check_txvector_integrity); 
		

    		/*!
    		* \brief Set/Get methods for TXPWR_LEVEL
     		*
     		* non-unified definition of received signal strength
     		*/
    		inline void set_TXPWR_LEVEL (int x) { TXPWR_LEVEL = (unsigned int) x;}
    		inline int get_TXPWR_LEVEL () const { return (int) TXPWR_LEVEL; } 

    		/*!
     		* \brief Set/Get methods for MCS
     		*
     		* values 0-76 represent different modulation and coding schemes\n
     		* see 21.6 in IEEE 802.11n standard
     		*/
    		inline void set_MCS (int x) { MCS = (unsigned int) x;}
    		inline int get_MCS () const { return (int) MCS; } 
				inline void set_MCS_meta (int x) { MCS_meta = (unsigned int) x;}
    		inline int get_MCS_meta () const { return (int) MCS_meta; }

    		/*!
    	 	* \brief Set/Get methods for LENGTH
     		*
     		* number of bytes in HT PSDU (0-65535)
     		*/
    		inline void set_LENGTH (int x) { LENGTH = (unsigned int) x;}
    		inline int get_LENGTH () const { return (int) LENGTH; } 
				inline void set_LENGTH_meta (int x) { LENGTH_meta = (unsigned int) x;}
    		inline int get_LENGTH_meta () const { return (int) LENGTH_meta; } 

    		/*!
     		* \brief Set/Get methods for SMOOTHING
     		*
     		* SMOOTHING_REC = turn on channel estimation smoothing\n
     		* SMOOTHING_NOT_REC = turn off channel estimation smoothing
     		*/
    		inline void set_SMOOTHING (int x) { SMOOTHING = (smoothing_type) x;}
    		inline int get_SMOOTHING () const { return (int) SMOOTHING; } 
	
    		/*!
     		* \brief Set/Get methods for NOT_SOUNDING
     		*
     		* SOUNDING = the packet contains sounding data only\n
     		* CONST_NOT_SOUNDING = indicates normal packet operation
     		*/
    		inline void set_NOT_SOUNDING (int x) { NOT_SOUNDING = (sounding_type) x;}
    		inline int get_NOT_SOUNDING () const { return (int) NOT_SOUNDING; } 
	
    		/*!
     		* \brief Set/Get methods for AGGREGATION
     		*
     		* AGGREGATED = A-MPDU aggregation in this packet\n
     		* NOT_AGGREGATED = no A-MPDU aggregation
     		*/
    		inline void set_AGGREGATION (int x) { AGGREGATION = (aggregation_type) x;}
    		inline int get_AGGREGATION () const { return (int) AGGREGATION; } 

    		/*!
     		* \brief Set/Get methods for STBC
     		*
     		* 0 = no space-time block coding (Nsts=Nss)\n
     		* 1 = Nsts=1+Nss\n
     		* 2 = Nsts=2+Nss\n
     		* 3 = RESERVED
     		*/
    		inline void set_STBC (int x) { STBC = (unsigned int) x;}
    		inline int get_STBC () const { return (int) STBC; } 
				inline void set_STBC_meta (int x) { STBC_meta = (unsigned int) x;}
    		inline int get_STBC_meta () const { return (int) STBC_meta; } 

    		/*!
     		* \brief Set/Get methods for LDPC_CODING
     		*
     		* BCC_CODING = binary convolutional codes\n
     		* CONST_LDPC_CODING = low-density parity check codes
     		*/
    		inline void set_LDPC_CODING (int x) { LDPC_CODING = (ldpc_coding_type) x;}
    		inline int get_LDPC_CODING () const { return (int) LDPC_CODING; } 

    		/*!
    	 	* \brief Set/Get methods for NUM_EXTEN_SS
     		*
     		* 0-3 integer indicates number of extension spatial streams
     		*/
   	 	inline void set_NUM_EXTEN_SS (int x) { NUM_EXTEN_SS = (unsigned int) x;}
    		inline int get_NUM_EXTEN_SS () const { return (int) NUM_EXTEN_SS; } 

    		/*!
     		* \brief Set/Get methods for N_TX
     		*
     		* 1-4 integer equals transmit antennas available
     		*/
    		inline void set_N_TX (int x) { N_TX = (unsigned int) x;}
    		inline int get_N_TX () const { return (int) N_TX; } 

    		/*!
     		* \brief Set/Get methods for ANTENNA_SET_ON
     		*
     		* CONST_ANTENNA_SET_ON = indicates that ANTENNA_SET is valid field
     		* NO_ANTENNA_SET_ON = indicates that ANTENNA_SET is not valid
     		*/
    		inline void set_ANTENNA_SET_ON (int x) { ANTENNA_SET_ON = (antenna_set_on_type) x;}
    		inline int get_ANTENNA_SET_ON () const { return (int) ANTENNA_SET_ON; } 

    		/*!
     		* \brief Set/Get methods for ANTENNA_SET
     		*
     		* 8 bit binary vector ("1" bit equals active antenna)
     		*/
    		inline void set_ANTENNA_SET (itpp::bvec x) { ANTENNA_SET = x;}
    		inline itpp::bvec get_ANTENNA_SET () const { return ANTENNA_SET; } 

    		/*!
    	 	* \brief Set/Get methods for EXPANSION_MAT_TYPE
     		*
     		* NO_EXPANSION_MAT = EXPANSION_MAT not used
     		* COMPRESSED_SV = EXPANSION_MAT contains compressed BF matrices
     		* NON_COMPRESSED_SV = EXPANSION_MAT contains BF matrices
     		* CSI_MATRICES = EXPANSION_MAT contains channel information
     		*/
    		inline void set_EXPANSION_MAT_TYPE (int x) { EXPANSION_MAT_TYPE = (expansion_mat_type_type) x;}
    		inline int get_EXPANSION_MAT_TYPE () const { return (int) EXPANSION_MAT_TYPE; } 

    		/*!
     		* \brief Set/Get methods for 
     		*/
    		inline void set_EXPANSION_MAT (itpp::Array<itpp::cmat> x) { EXPANSION_MAT = x;}
    		inline itpp::Array<itpp::cmat> get_EXPANSION_MAT () const { return EXPANSION_MAT; } 
  
	private:
		// ============================================================
		// -------------------  TXVECTOR Standard ---------------------
		// -> see table n56 of 802.11n standard for more detail
		// ============================================================
		itpp::bvec SERVICE;
		unsigned int TXPWR_LEVEL;
		unsigned int MCS;
		unsigned int MCS_meta;
		unsigned int LENGTH;
		unsigned int LENGTH_meta;
		smoothing_type SMOOTHING;
		sounding_type NOT_SOUNDING;
		aggregation_type AGGREGATION;
		unsigned int STBC;
		unsigned int STBC_meta;
		ldpc_coding_type LDPC_CODING;
		unsigned int NUM_EXTEN_SS;
		unsigned int N_TX;
		antenna_set_on_type ANTENNA_SET_ON;
		itpp::bvec ANTENNA_SET;
		expansion_mat_type_type EXPANSION_MAT_TYPE;
		itpp::Array<itpp::cmat> EXPANSION_MAT; 

 	public:
		// ============================================================
		// Methods for accessing control variables
 		// ============================================================
		
  		/*!
  		* \brief Get/Set methods for accessing unsigned int control members
   		*/
 	  inline unsigned int get_Nsts () const { return Nsts;}
 		inline unsigned int get_Nsts_meta () const { return Nsts_meta;}
   	inline unsigned int get_Nss () const { return Nss;}
   	inline unsigned int get_Nss_meta () const { return Nss_meta;}
   	inline unsigned int get_mod_index () const { return (unsigned int) mod_index;}
   	inline unsigned int get_mod_index_meta () const { return (unsigned int) mod_index_meta;}
   	inline unsigned int get_code_rate () const { return (unsigned int) code_rate;}
   	inline unsigned int get_code_rate_meta () const { return (unsigned int) code_rate_meta;}
   	inline unsigned int get_Nbpsc () const { return Nbpsc;}
   	inline unsigned int get_Nbpsc_meta () const { return Nbpsc_meta;}
   	inline unsigned int get_Nsd () const { return Nsd;}
   	inline unsigned int get_Nsp () const { return Nsp;}
   	inline unsigned int get_Nst () const { return Nst;}
   	inline unsigned int get_Nnull () const { return Nnull;}
    inline unsigned int get_Nfft () const { return Nfft;}
   	inline unsigned int get_Nofdm () const { return Nofdm;}
   	inline unsigned int get_Ngi  () const { return Ngi;}
   	inline unsigned int get_Ncbps () const { return Ncbps;}
   	inline unsigned int get_Ncbps_meta () const { return Ncbps_meta;}
   	inline unsigned int get_Ndbps () const { return Ndbps;}
   	inline unsigned int get_Ndbps_meta () const { return Ndbps_meta;}
   	inline unsigned int get_Nes  () const { return Nes;}
   	inline unsigned int get_Nes_meta  () const { return Nes_meta;}
   	inline unsigned int get_Nsym () const { return Nsym;}
   	inline unsigned int get_Nsym_meta () const { return Nsym_meta;}
   	inline unsigned int get_Neltf () const { return Neltf;}
   	inline unsigned int get_Ndltf () const { return Ndltf;}
   	inline unsigned int get_Ness () const { return Ness;}
   	inline unsigned int get_Npresamples () const { return Npresamples;}
   	inline unsigned int get_Ndatasamples () const { return Ndatasamples;}
   	inline unsigned int get_Nmetasamples () const { return Nmetasamples;}
   	inline unsigned int get_Ndatacoded () const { return Ndatacoded;}
   	inline unsigned int get_Nmetacoded () const { return Nmetacoded;}
    		
		inline void set_Nsts (unsigned int x) { Nsts = x; }
		inline void set_Nsts_meta (unsigned int x) { Nsts_meta = x; }
    inline void set_Nss (unsigned int x) { Nss = x; }
    inline void set_Nss_meta (unsigned int x) { Nss_meta = x; }
   	inline void set_mod_index (unsigned int x) { mod_index = (modulation_type) x; }
   	inline void set_mod_index_meta (unsigned int x) { mod_index_meta = (modulation_type) x; }
   	inline void set_code_rate (unsigned int x) { code_rate = (coding_rate_type) x; }
   	inline void set_code_rate_meta (unsigned int x) { code_rate_meta = (coding_rate_type) x; }
   	inline void set_Nbpsc (unsigned int x) { Nbpsc = x; }
   	inline void set_Nbpsc_meta (unsigned int x) { Nbpsc_meta = x; }
   	inline void set_Nsd (unsigned int x) { Nsd = x; }
   	inline void set_Nsp (unsigned int x) { Nsp = x; }
   	inline void set_Nst (unsigned int x) { Nst = x; }
   	inline void set_Nnull (unsigned int x) { Nnull = x; }
   	inline void set_Nfft (unsigned int x) { Nfft = x; }
   	inline void set_Nofdm (unsigned int x) { Nofdm = x; }
   	inline void set_Ngi  (unsigned int x) { Ngi  = x; }
   	inline void set_Ncbps (unsigned int x) { Ncbps = x; }
   	inline void set_Ncbps_meta (unsigned int x) { Ncbps_meta = x; }
   	inline void set_Ndbps (unsigned int x) { Ndbps = x; }
   	inline void set_Ndbps_meta (unsigned int x) { Ndbps_meta = x; }
   	inline void set_Nes  (unsigned int x) { Nes  = x; }
   	inline void set_Nes_meta  (unsigned int x) { Nes_meta  = x; }
 		inline void set_Nsym (unsigned int x) { Nsym = x; }
 		inline void set_Nsym_meta (unsigned int x) { Nsym_meta = x; }
 		inline void set_Neltf (unsigned int x) { Neltf = x; }
 		inline void set_Ndltf (unsigned int x) { Ndltf = x; }
 		inline void set_Ness (unsigned int x) { Ness = x; }
  	inline void set_Npresamples (unsigned int x) { Npresamples  = x; }
  	inline void set_Ndatasamples (unsigned int x) { Ndatasamples = x; }
 		inline void set_Nmetasamples (unsigned int x) { Nmetasamples = x; }
  	inline void set_Ndatacoded (unsigned int x) { Ndatacoded = x; }
  	inline void set_Nmetacoded (unsigned int x) { Nmetacoded = x; }

	private:
		// ============================================================
		// --------- Private variables for controlling PHY ----------
		// ============================================================
		bool is_valid; // 'true' indicates TXVECTOR standard values are valid
		unsigned int Ntx;
    unsigned int Nsts; // number of space-time-streams in PHY (STBC)
		unsigned int Nsts_meta; // number of meta space-time-streams in PHY (STBC)
		unsigned int Nss; // number of spatial-streams in PHY (Multiplexing)
		unsigned int Nss_meta; // number of meta spatial-streams in PHY (Multiplexing)
		modulation_type mod_index; // 'QAM_2' - BPSK Modulated Subcarriers
					   // 'QAM_4' - QPSK Modulated Subcarriers
					   // 'QAM_16' - 16-QAM Modulated Subcarriers
					   // 'QAM_64' - 64-QAM Modulated Subcarriers
		modulation_type mod_index_meta; 
		coding_rate_type code_rate; // 'R_1_2' - rate 1/2 error control coding
					    // 'R_2_3' - rate 2/3 error control coding
					    // 'R_3_4' - rate 3/4 error control coding
					    // 'R_5_6' - rate 5/6 error control coding
		coding_rate_type code_rate_meta; // 'R_1_2' - rate 1/2 error control coding
		unsigned int Nbpsc; // number of bits per subcarrier (e.g. 2 for QAM_4)
		unsigned int Nbpsc_meta; // number of bits per subcarrier (e.g. 2 for QAM_4)
		unsigned int Nsd; // number of data subcarriers
		unsigned int Nsp; // number of pilot subcarriers
		unsigned int Nst; // number of total subcarriers (Nsd+Nsp)
		unsigned int Nnull; // number of null subcarriers
		unsigned int Nfft; // FFT/IFFT order (Nst+Nnull)
		unsigned int Nofdm; // number of samples per OFDM symbol (Ngi+Nfft)
		unsigned int Ngi; // samples per guard interval of payload
		unsigned int Ncbps; // number of coded bits per OFDM symbol
		unsigned int Ncbps_meta;
		unsigned int Ndbps; // number of uncoded bits per OFDM symbol
		unsigned int Ndbps_meta;
		unsigned int Nes; // number of encoding streams (for parallel encoding)
		unsigned int Nes_meta;
		unsigned int Nsym; // number of OFDM symbols in payload
		unsigned int Nsym_meta;
		unsigned int Neltf; // number of extension spatial training fields
		unsigned int Ndltf; // number of data spatial training fields
		unsigned int Ness; // number of extension spatial streams (Nst+Ness<=Ntx)
		unsigned int Npresamples; // total number of complex samples in preamble
		unsigned int Ndatasamples; // total number of complex samples in payload
		unsigned int Nmetasamples; // total number of complex samples in payload
		unsigned int Ndatacoded; // number of coded bits (LENGTH * coding rate)
		unsigned int Nmetacoded; // number of coded bits (LENGTH * coding rate)


  friend class Transmission_Interface_11n;
  friend class test_11n;
  friend class Preamble_Formatting_11n;
  friend class Payload_Formatting_11n;
  friend class STF_Formatting_11n;
  friend class LTF_Formatting_11n;
  friend class Signal_Field_Formatting_11n;
  friend class Precoding_Matrices_11n;
};

#endif /* INCLUDED_11N_PHY_TXVECTOR_H */

