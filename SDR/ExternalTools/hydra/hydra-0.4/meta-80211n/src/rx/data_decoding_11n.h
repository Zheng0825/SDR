/*!     
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
 * 	Initial Version: \date{06/29/07}
 * 
 *  \brief Receiver Data Processing (RDP) class for 802.11n physical layers
 *
 *	\author{Robert C. Daniels (rdaniels [at] ece.utexas.edu)}
 *
 * 	- \date{06/29/07} \note{Robert Daniels: Initial Version}
 */

#ifndef Data_Decoding_11N_H
#define Data_Decoding_11N_H
#include "rxvector.h"
#include <itpp/itbase.h>
#include <itpp/itcomm.h>
#include <itpp/itstat.h>


// types define algorithm parameters of the receiver
enum estimation_mode_type {FDE_LS=0, TDE_LS=1}; // esimation of channel-related statistics
enum equalization_mode_type {FDE_ZF=0, FDE_MMSE=1, TDE_ZF=2, TDE_MMSE=3, ML=4, SPHERE=5};

//! \class Receiver Data Processing
class Data_Decoding_11n
{
	public:
		Data_Decoding_11n();
		
		// --------------- SETUP FUNCTIONS FOR RECEIVER ------------------------	

		/*! \brief set the mode of estimation used to estimate channel-related
		 * parameters (e.g. impulse-response, SNR, etc.
		 *
		 * \param  es_mode  default value is FDE_ZF (frequency-domain
		 * zero-forcing)
		 */
		inline void set_estimation_mode(estimation_mode_type es_mode) {estimation_mode=es_mode;}

		/*! \brief set the mode of equalization used to remove
		 * wireless channel intersymbol/spatial interference effects
		 *
		 * \param eq_mode default value is FDE_LS (frequency-domain least-squares)
		 */
		inline void set_equalization_mode(unsigned int eq_mode) {equalization_mode=eq_mode;}

		/*! \brief correction in the received data; should be set before
		 *  \a recover_bits is called, otherwise receiver will assume 1
		 *  MHz sampling rate with 0 Hz offset
		 * 
		 *  \param cfo channel frequency offset as estimated in preamble
		 *  \param fs sampling rate of captured waveform
		 */
		inline void set_frequency_offset(double cfo, double fs) {frequency_offset=cfo;sampling_frequency=fs;}
		
		// ---------------------------------------------------------------------		
		
		// -------- DECODE THE BITS FROM THE PAYLOAD OF THE PACKET -------------
			
		/*! \brief return bits stored in the payload of 802.11n waveform
		 *
		 *  \param rx_parameters MAC/PHY interface parameters at the receiver set 
		 *  by the Header_Decoding_11n class
		 *  \param noise_variance estimated noise variance, calculated by assuming
		 *  normalized transmit power (all other energy is associated with channel)
		 *  \param received_stream complex data matrix for the 802.11n wavform;
		 *  the receive antenna is contained in a different row of the matrix; 
		 *  the first column should precede all long training fields used for
		 *  spatial channel estimation
		 *  \param rx_output binary vector contains the recovered bits embedded 
		 *  in the received 802.11n frame waveform
		 */
		void recover_bits(rxvector& rx_parameters, itpp::cmat received_stream, double noise_variance, itpp::bvec& rx_output, itpp::bvec& meta_output);

		// ---------------------------------------------------------------------		

		// ------- RETRIEVE SIGNAL QUALITY METRICS W/O BUFFERING MODE ---------- 
		
		/*! \brief Returns the recommended MIMO MCS based on inputs
		 * 
		 *  \param snr average signal to noise ratio
		 *  \param demmel the Demmel condition number of channel matrix
		 *  \param psdu_len the number of Bytes in the packet
		 *  \retval recommend_mimo_mcs returns the modulation and coding scheme
		 *  recommended based on the channel
		 */
		unsigned int recommend_mimo_mcs(double snr, double demmel, unsigned int psdu_len); 	
		
		/*! \brief Returns the recommended SISO MCS based on inputs
		 * 
		 *  \param snr average signal to noise ratio
		 *  \param psdu_len the number of Bytes in the packet
		 *  \retval recommend_siso_mcs returns the modulation and coding scheme
		 *  recommended based on the channel
		 */
		unsigned int recommend_siso_mcs(double snr, unsigned int psdu_len); 	
		
		/*! \brief returns an array of matrices that represent the channel
		 *  matrix for each data and pilot subcarrier
		 */
		inline itpp::Array<itpp::cmat> get_channel() {return channel_matrices;}
		
		/*! \brief returns an array of matrices that represent the channel
		 *  matrix for space-time stream and extension spatial stream
		 */
		inline itpp::Array<itpp::cmat> get_extension_channel() {return extension_channel_matrices;}
		
		/*! \brief Get the average SNR for each transmit receive pair
		 *
		 *  \retval get_spatial_snr(...) returns a \f$N_{RX} \times N_{ss}\f$
		 *  matrix with the signal to noise ratio as averaged over all
		 *  subcarriers (assumes the same additive noise experienced over all
		 *  frequencies)
		 */
		inline itpp::vec get_spatial_snr() {return avg_spatial_snr;}	
		inline double get_avg_snr() {return sum(avg_spatial_snr)/(avg_spatial_snr.length());}

		/*! \brief Get the average SNR for each transmit receive pair
		 *
		 *  \retval get_extension_snr(...) returns a \f$N_{RX} \times N_{ess}\f$
		 *  matrix with the signal to noise ratio as averaged over all
		 *  subcarriers in extension channel estimate (assumes the same additive
		 *  noise experienced over all frequencies) and over eah
		 *  transmit-receive antenna pair
		 */
		inline itpp::mat get_extension_spatial_snr() {return extension_spatial_snr;}	
	
		/*! \brief Get the average SNR
		 *
		 *  \retval get_extension_snr(...) returns a \f$N_{RX} \times N_{ess}\f$
		 *  matrix with the signal to noise ratio as averaged over all
		 *  subcarriers in extension channel estimate (assumes the same additive
		 *  noise experienced over all frequencies) and over eah
		 *  transmit-receive antenna pair
		 */
		inline double get_extension_snr() {return sumsum(extension_spatial_snr)/(extension_spatial_snr.rows()*extension_spatial_snr.cols());}	
	
		/*! \brief Get the spatial correlation matrix for each received signal
		 *  
		 *  \retval get_spatial_correlation returns the spatial correlation
		 *  matrix of dimension \f$ Nss N_{RX} \times Nss N_{RX} \f$
		 *  as calculated from the current spatial channel observed	
		 */
		itpp::mat get_spatial_correlation();	
	
		/*! \brief Get the post processing SNR for the payload
 		 *
 		 *  \retval get_post_processing_snr returns the post procesing SNR
 		 *  for each spatial stream subcarrier (\f$Nsts \times Nst\f$ double
 		 *  precision matrix) in the payload
 		 */ 	
		itpp::mat get_post_processing_snr() {return post_processing_snr;}
		
		/*! \brief Get the post processing SNR for the meta payload
 		 *
 		 *  \retval get_post_processing_snr_meta returns the post procesing SNR
 		 *  for each spatial stream subcarrier (\f$Nsts \times Nst\f$ double
 		 *  precision matrix) in the meta payload
 		 */ 	
		itpp::mat get_meta_post_processing_snr() {return meta_post_processing_snr;}		

		/*! \brief Get the condition number for each subcarrier matrix
		 * 
		 *  The invertability of a numerical matrix can be quantified by the
		 *  condition number of a matrix whose range resides in \f$ [1,\infty)
		 *  \f$; Perhaps a more appropriate metric for selecting between
		 *  Multiplexing and Diversity transmission is the Demmel condition
		 *  number
		 * 
		 *  \param is_Demmel a \a true value signifies that the Demmel condition
		 *  number is to be extracted; otherwise the traditional condition
		 *  number will be calculated
		 *  \retval get_condition_number returns an vector of size \f$Nst\f$
		 *  where each value represents the condition number corresponding to
		 *	a subcarrier
		 * 
		 *  \par References: 
		 *  R. W. Heath, Jr. and A. J. Paulraj, ``Switching Between Diversity and Multiplexing in MIMO Systems,'' IEEE Trans. on Communications, vol. 53, no. 6, pp. 962-968, June 2005.
		 */
		itpp::vec get_condition_number(bool is_Demmel);
		
		/*! \brief Get the average condition number over all subcarriers
		 *
	     *  This is the same function as \a get_condition_number, but averaged
	     *  over all subcarriers 
		 */
		double get_avg_condition_number(bool is_Demmel);
		 
		/*! \brief Turn on receiver process timing
		 * 		
		 *  \param timing_on \a true sets timing of receive processes
		 */
		inline void trigger_rx_timing(bool timing_on) {is_rx_timing_on=timing_on;}

		// ---------------------------------------------------------------------

		// ------- RETRIEVE SIGNAL QUALITY METRICS WITH BUFFERING MODE ---------

		/*! \brief Trigger the analysis mode of the receiver
		 *
		 *	\param is_analysis_mode \a true sets the receiver in analysis mode
		 *	so that phase_tracking, frequency_tracking, coded_bits,
		 *	ofdm_symbols, channel_correlation, channel_condition_number,  
		 */
		void trigger_buffering_mode(bool buff_mode) {is_buffering_mode=buff_mode;}
	
		/*! \brief Get vector that contains a progression of the estimated
		 *  phase offset as measured by the pilots in each OFDM symbol
		 */
		inline itpp::mat get_phase_tracking() {return phase_tracking;}

		/*! \brief Get vector that contains a progression of the estimated
		 *  frequency offset as measured by the pilots in each OFDM symbol
		 */
		inline itpp::mat get_frequency_tracking() {return frequency_tracking;}
		
		/*! \brief Get the temporal correlation correlation matrix
		 * 	 
		 *  \retval get_temporal_correlation returns the temporal correlation
		 *  matrix of dimension \f$ Nss N_{RX} \times Nss N_{RX} \f$
		 *  as calculated from the current and past spatial channel observed;
		 *  hence, for calculation, a series of symbols must be processed
	     */	
		itpp::mat get_temporal_correlation();	

		/*! \brief Get the complex matrix containing all data and pilots from
		 *  OFDM symbols n1 through and including n2
		 *
		 *  The data and pilots of of OFDM symbols n1 through n2 (starting at
		 *  index 0); All OFDM symbols not present in last received packet will
		 *  ignored in the function call.   
		 */
		itpp::cmat get_ofdm_symbols(int n1, int n2);
		itpp::cmat get_meta_ofdm_symbols(int n1, int n2);

		/*! \brief Get the coded or uncoded bit-error rates
		 *
		 *	\param uncoded_ber \a true means that the uncoded BER will be
		 *	calculated, whereas \a false will return coded BER; the receiver
		 *	assumes that each byte of the binary input corresponds to
		 *	mod(index,256) in decimal notation where "index" is the sequential
		 *	byte index (starting at 0); the seed of the scrambler at the
		 *	transmitter is assumed be all zeros
 		 */
		int get_bit_errors(bool uncoded_bit_errors, rxvector const& rxv);

		itpp::bvec get_coded_bits();
		// ---------------------------------------------------------------------
		
	private:
		// ---------------------------------------------------------------------		
		void symbol_by_symbol_decode(rxvector& rx_parameters, itpp::cmat const& received_stream, double N0, itpp::bvec& decoded_output, unsigned int start_data_index);
		void symbol_by_symbol_decode_meta(rxvector& rx_parameters, itpp::Array<itpp::cmat> const& meta_channel, itpp::cmat const& received_stream, double N0, itpp::bvec& decoded_output, unsigned int start_data_index);
		// ---------------------------------------------------------------------	

		unsigned int Nrx,Nsd; // number of receive antennas
		bool is_buffering_mode; // boolean variable for analysis mode
		double sampling_frequency, frequency_offset; // self-explanatory
		itpp::bvec coded_bits; // data bits before decoding
		itpp::bvec recovered_bits;
		itpp::cmat ofdm_symbol_history;
		itpp::cmat meta_ofdm_symbol_history;
		unsigned int equalization_mode; // FDE_ZF - frequency domain zero-forcing
							  // FDE_MMSE - frequency domain minimum mean squared error
							  // TDE_ZF - time domain zero-forcing
							  // TDE_MMSE - time domain minimum mean squared error
							  // ML - maximum likelihood estimation
							  // SPHERE - sphere decoder ML approximation
		estimation_mode_type estimation_mode; // FDE_LS - frequency domain least squares estimation
						      // TDE_LS - time domain least squares estimation
		itpp::mat post_processing_snr;
		itpp::mat meta_post_processing_snr;
		itpp::mat phase_tracking; // holds phase offset measured in each OFDM symbol
		itpp::mat frequency_tracking; // holds frequency offset measured in each OFDM symbol
		itpp::vec single_stream_soft_bits; // holds uncoded soft bits for Viterbi
		itpp::Array<itpp::cmat> channel_matrices; // holds channel matrix estimates
		itpp::Array<itpp::cmat> extension_channel_matrices; // holds channel matrix estimates
		itpp::Array<itpp::cmat> past_channel;
		bool is_past_channel_initialized, is_rx_timing_on;
		itpp::vec avg_spatial_snr; // holds the avg snr 
		itpp::mat extension_spatial_snr; // holds the avg snr 
};
#endif

