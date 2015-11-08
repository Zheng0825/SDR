/*  	
 * Title:  Header decoding (HED) for 802.11n preambles
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Header for "Header_Decoding_11n.cc"
 *
 * Revision: v0.00 - 
 * 
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
 * 
 */ 


#ifndef Header_Decoding_11N_H
#define Header_Decoding_11N_H

#include "rxvector.h"
#include <itpp/itbase.h>

class Header_Decoding_11n
{
	public:
		Header_Decoding_11n(); 
				
		// ==============================================================
		// SET_SAMPLING_FREQUENCY: Set the sampling frequency after dec-
		// imation from the ADC;
		// --------------------------------------------------------------
		//  'fs' - default value is 1e6 
		// ==============================================================
		void set_sampling_frequency(double fs);
	
		// ==============================================================
		// SET_CORRELATION_THRESHOLD: Set the correlation threshold used
		// for frame and timing sychronization
		// --------------------------------------------------------------
		//  'th' - default values is 0.85
		// ==============================================================	
		void set_correlation_threshold(double th);

		// ==============================================================
		// SET_COARSE_PACKET_WINDOW: Set the window size of the frame syn-
		// chronization algorithm (uses short training field)
		// --------------------------------------------------------------
		// 'window_size' - default value is Nofdm from rxvector	
		// ==============================================================
		void set_coarse_packet_window(unsigned int window_size);
		
		// ==============================================================
		// SET_EXACT_PACKET_WINDOW: Set the window size of the for exact
		// symbol timing algorithms (uses end of short training field)
		// --------------------------------------------------------------
		// 'window_size' - default value is 6*Nofdm/10 from rxvector	
		// ==============================================================
		void set_exact_packet_window(unsigned int window_size);
		
		// ==============================================================
		// SET_TIMING_TOLERANCE: Set the number of anticipatory symbols
		// for exact symbol timing; used with OFDM sysmtes
		// --------------------------------------------------------------
		// 'timing_tolerance' - default value is 5 	
		// ==============================================================
		void set_timing_tolerance(unsigned int time_tol);

		// ==============================================================
		// SET_COARSE_CFO_WINDOW: Set the window size of the coarse fre-
		// quency offset estimation algorithm (uses the short training 
		// field)
		// --------------------------------------------------------------
		// 'window_size' - default value is 6*Nofdm/10 from rxvector	
		// ==============================================================
		void set_coarse_cfo_window(unsigned int window_size);
		
		// ==============================================================
		// SET_FINE_CFO_WINDOW: Set the window size of the coarse fre-
		// quency offset estimation algorithm (uses the long training 
		// field)
		// --------------------------------------------------------------
		// 'window_size' - default value is Nfft from rxvector	
		// ==============================================================
		void set_fine_cfo_window(unsigned int window_size);

		// ==============================================================
		// EGC_DECODE: Equal gain combining for header decoding; uses
		// equal gain combining of receive antenna streams 
		// --------------------------------------------------------------
		// 'header_symbols' - portion of complex baseband input signal that
		// contains at least all the preamble up to the (HT) signal field
		// 'column_update' - integer represents the number of columns con-
		// sumed by EGC_DECODE
		// ==============================================================
		void egc_decode(itpp::cmat header_symbols, unsigned int& column_update, unsigned int& remaining_columns, rxvector& rx_parameters);
	

		// ==============================================================
		// MRC_DECODE: Maximal ratio combining for header decoding; uses
		// the energy observed on each receive antenna to determine comb-
		// ination of the antenna streams
		// --------------------------------------------------------------
		// inputs the same as EGC_DECODE
		// ==============================================================
		void mrc_decode(itpp::cmat header_symbols, unsigned int& column_update, unsigned int& remaining_columns, rxvector& rx_parameters);

		// ==============================================================
		// GET_COARSE_CFO_WINDOW: returns 'c_cfo' private variable
		// ==============================================================
		inline double get_coarse_cfo() {return c_cfo;}

		// ==============================================================
		// GET_FINE_CFO_WINDOW: returns 'f_cfo' private variable
		// ==============================================================
		inline double get_fine_cfo() {return f_cfo;}

		// ==============================================================
		// GET_SAMPLING_FREQUENCY: returns 'sampling_frequency' private variable
		// ==============================================================
		inline double get_sampling_frequency() { return sampling_frequency; }

		// ==============================================================
		// GET_AVG_SNR: returns 'average_snr' private variable
		// ==============================================================
		inline double get_avg_snr() {return average_snr;}

		// ==============================================================
		// GET_NOISE_VARIANCE: returns 'noise_variance' private variable
		// ==============================================================
		inline double get_noise_variance() {return noise_variance;}

		// ==============================================================
		// IS_CRC_OK: is only valid after EGC_DECODE or MRC_DECODE is run;
		// returns 'crc_ok' private variable;
		// ==============================================================
		inline bool is_crc_ok() {return crc_ok;}

		inline bool is_packet_detected() {return packet_detected;}
		
	private:
		unsigned int Nfft, Ngi; // see rxvector.h for details
		itpp::bvec signal_bits; // local variable that holds the demodulated
	       		  		// (HT)SIGNAL field bits	
		
		// ==============================================================
		// HT_SIGNAL_ASSIGN: private function that takes the 'signal_bits'
		// and assigns rxvector values
		// ==============================================================
		void ht_signal_assign(itpp::bvec const& in_bits, rxvector&
rx_parameters);
		
		itpp::cmat channel;  // subcarrier channel information for OFDM symbols (one stream)
		                     // depends on combining method
		double sampling_frequency; // sampling frequency of receiver
		double N0; // noise power
		double correlation_threshold; // threshold for correlation based detection
		double fractional_energy[4]; // fractional energy per receive antenna chain
		double c_cfo, f_cfo; // channel frequency offset estimate (coarse and fine)
		int timing_tolerance; // packet synchronization tolerance (anticipatory symbols)
		double uncorrelation_ratio; // uncorrelated symbols to correlated symbols ratio
		unsigned int coarse_packet_window; // window size for packet detection
		unsigned int exact_packet_window; // window size for exact symbol timing
		unsigned int coarse_cfo_window; // window size for coarse CFO estimation
		unsigned int fine_cfo_window; // window size for fine CFO estimation
		double average_snr; // SNR average over pilot/data subcarriers
		double noise_variance;
		bool is_greenfield; // 'true' for greenfield preamble
		bool crc_ok; // 'true' if CRC passes in (HT)SIGNAL field
		bool using_custom_cpw; // 'true' indicates 'set coarse_packet_window' triggered
		bool using_custom_epw; // 'true' indicates 'set exact_packet_window' triggered
		bool packet_detected;
		bool using_custom_ccw; // 'true' indicates 'set coarse_cfo_window' triggered
		bool using_custom_fcw; // 'true' indicates 'set fine_cfo_window' triggered
		unsigned int synchronization_search_length; // depth to search for synch index
		unsigned int required_legacy_symbols; // symbols required to properly decode legacy header
		unsigned int required_additional_symbols; // additional symbols needed for ht preambles
};
#endif
