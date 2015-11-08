/*! \file test_80211n.h
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
 *
 * 	Initial Version: \date{06/29/07}
 * 
 *  \brief Header for test program class that for interfacing with the IEEE
 *  802.11n physical layer 
 *
 *	\author{Robert C. Daniels (rdaniels [at] ece.utexas.edu)}
 *
 * 	- \date{06/29/07} \note{Robert Daniels: Initial Version}
 */

#ifndef _INCLUDED_TEST_80211N_H_
#define _INCLUDED_TEST_80211N_H_

#include <itpp/itbase.h>
#include <itpp/itstat.h>
#include "../src/tx/txvector.h"
#include "../src/rx/rxvector.h"

//! \brief Test class for the IEEE 802.11n physical layer
class test_11n
{
	public:
		test_11n();
		
		// *** SETTING PHY STANDARD PARAMETERS ***		

		/*! \brief Set the modulation and coding scheme of the 802.11n physical
 		 *  layer for the simulation
		 *
		 *  \param mcs_mode integer of value 0-76 that indicates the modulation
		 *  scheme and coding rate used in the 802.11n physical layer; see
		 *  tables n81-96 of the 802.11n physical layer for details
		 */
		inline void set_modulation_and_coding(unsigned int mcs_mode){tx_param.MCS=mcs_mode;}
		inline void set_meta_modulation_and_coding(unsigned int mcs_mode){tx_param.MCS_meta=mcs_mode;}
		
		/*! \brief Set the spatial mapping matrices in the 802.11n transmitter
		 *  
		 *  \param Q this array of complex matrices is of the same size as the
		 *  number of pilot and data subcarriers;  Each matrix is of dimensions
		 *  \f$ N_{TX} \times N_{STS} \f$;  See section 20.3.10.10.1 of the
		 *  802.11n standard for more details
		 */
		inline void set_spatial_mapping_matrices(itpp::Array<itpp::cmat> const& Q) {tx_param.EXPANSION_MAT=Q;}
	
		/*! \brief Set the space-time coding scheme
		 *  
		 *  \param stbc_strms parameter sets the number difference between the
		 *  number of spatial streams and the number of transmit antennas used
		 *  (e.g. \f$Ntx-Nss\f$ where \f$Nss\f$ represents the number of
		 *  multiplexed streams)
		 */
		inline void set_stbc_streams(unsigned int stbc_strms)
{tx_param.STBC=stbc_strms;}

		/*! \brief Set the txvector given to the transmitter, which fully
 		 *  configures the parameters passed from the MAC to PHY
		 *
		 *  \param txv the txvector class variable will be assigned in whole,
		 *  instead of using default values;  for configuring txvector see table
		 *  n56 of the 802.11n standard as well as "txvector.h"
		 */
		inline void set_txvector(txvector txv) {tx_param=txv;}
		
		// *** SETTING PHY ALGORITHM PARAMETERS ***		
		
		/*! \brief Set the correlation threshold of the synchronization
 		 * algorithms int the 802.11n physical layer simulation
		 *
		 *  \param th_value should be between 0.5 and 1; by default his value is
		 *  set to 0.75
		 */
		inline void set_correlation_threshold(double th_value)
{correlation_threshold=th_value;}
		
		/*! \brief Set the window size for packet detection algorithms in the
		 * 802.11n physical layer simulation
		 *
		 *  \param window should be \f$>Nfft/2\f$ where \f$Nfft\f$ is the size
		 *  of the FFT used for the OFDM physical layer; default value is 32
	 	 */
		inline void set_packet_detection_window(unsigned int
window) {coarse_window=window;}
		
		/*! \brief Set the estimation metric used to determine the channel state
		 *  information
		 *
		 *  \param est_mode 0 -> frequency domain least-squares estimation, 1->
		 *  time domain least-squares estimation
		 */
		inline void set_estimation_metric(unsigned int est_mode)
{estimation_mode=est_mode;}

		/*! \brief Set the equalization method used to remove the channel
 		 *  impairments acting on the received signal
		 *  
		 *  \param eq_mode 0 -> frequency domain equalization using
		 *  zero-forcing, 1 -> frequency domain equalization using minimum mean
		 *  square error, 2 -> time domain equalization using zero-forcing, 3 ->
		 *  time domain equalization using minimum mean-sqare error, 4 ->
		 *  maximum likelihood signal detection
		 */
		inline void set_equalization_method(unsigned int eq_mode)
{equalization_mode=eq_mode;}

		// *** SETTING PHY SIMULATION PARAMETERS ***
		
		/*! \brief Turn on BER analysis
		 *
		 *  \param ber_on \a true turns on BER data storage (may incur longer
		 *  simulation times, off by default)
		 */
		inline void trigger_BER_analysis(bool ber_on) {is_BER_on=ber_on;}
		
		/*! \brief Turn on fixed inputs
		 *
		 *  \param fixed_on \a true uses a standard Byte sequence and seed in the transmitter
		 */
		inline void trigger_fixed_input(bool fixed_on) {is_fixed_input_on=fixed_on;}

		/*! \brief Turn on timing analysis of physical layer processes
		 *
		 *  \param fixed_on \a true uses a standard Byte sequence and seed in the transmitter
		 */
		inline void trigger_process_timing(bool timing_on) {is_process_timing_on=timing_on;}

		
		/*! \brief Turn on full channel estimation, regardless of spatial
 		 *  streams 
		 *
		 *  \param ess_on \a true means that \f$Ness\f$ will be set such that
		 *  \f$Ntx=Nsts+Ness\f$
		 */
		inline void trigger_extension_streams(bool ess_on) {is_full_estimation_on=ess_on;}
	
		/*! \brief Turn on smooting of the channel estimation data
		 *
		 *  \param ess_on \a true means smoothing turned on
		 */
		inline void trigger_channel_smoothing(bool smooth_on) {is_channel_smoothing_on=smooth_on;}
		
		/*! \brief Get BER for all SNRs simulated
		 * 
		 *  \retval get_BER_vector vector of bit error rates corresponding to
		 *  the SNRs given at the beginning of simulation; 
		 *
		 *	\note BER calculated for those packets where header CRC passes; 
		 *  may not reflect true BER in low SNR where packets fail due to
		 *  mis-synchronization and header failure
		 */
		inline itpp::mat get_BER_matrix() {return BER_matrix;}

		/*! \brief Set the frequency offset parameters for the simulation
		 *  
		 *  \param cfo the mean of the channel frequency offset to add to the
		 *  received signals (simulates channel frequency offset); default value
		 *  is 0
		 *  
		 *  \param cfo_variance the variance of the frequency offset added to
		 *  the received signal over time (simulates frequency drift); default
		 *  value is 0
		 *
		 *  \param fs the sampling frequency at the receiver; default value is
		 *  1e6;  in reality this is a relative parameter; it complements the \a
		 *  cfo parameter;  802.11n systems may use 20 MHz or 40 MHz
		 */
		void set_frequency_offset_parameters(double cfo, double cfo_variance, double fs);

		// *** RUNNING SIMULATIONS ***	
	
		/*! \brief Run a simulation of the 802.11n physical layer with a fixed
 		 *  channel, a list of tested SNR values
		 *  
		 *  \param number_of_packets integer parameter that tells the simulator
		 *  how many iterations should be performed for evaluating the packet
		 *  error rate for each SNR value given a fixed channel realization
		 *  \param psdu_length integer length indicating the number of bytes in
		 *  the packet given to the physical layer by the MAC layer
		 *  \param observed_channels an array of complex matrices; the size of
		 *  the array equals the number of multipath taps in the channel, with
		 *  the first element being the first tap;  the dimension of each
		 *  complex matrix should equal \f$N_{RX} \times N_{TX}\f$
	     *  \param snr_values is a vector of double precision floating point
	     *  numbers each of which represents an SNR value to be simulated
	     *  (linear scale NOT dB)
	 	 *  \param verbose a boolean variable in which a \a true value indicates
	 	 *  that more detail of the simulation will be printed on the screen
		 *  \retval run(...) this function returns a binary matrix;  a "1" in a
		 *  entry of the matrix indicates a failed packet;  the rows of the
		 *  matrix contain the results for a distinct snr while the columns
		 *  represent each iteration as indicated by \a number_of_packets
		 */
		itpp::vec run(unsigned int number_of_packets, unsigned int psdu_length, unsigned int meta_length, itpp::Array<itpp::cmat> observed_channels, itpp::vec snr_values, bool verbose);

	private:
		double sampling_frequency;
		double sampling_time;
		double correlation_threshold;
		double estimated_frequency_offset;
		double frequency_offset;
		double frequency_variance;
		unsigned int coarse_window;
		unsigned int zero_padding_length;
		unsigned int gain_mode;
		unsigned int estimation_mode;
		unsigned int equalization_mode;
		unsigned int stbc_streams;
		txvector tx_param;
		rxvector rx_param;
		bool is_greenfield;
		itpp::mat BER_matrix;
		itpp::vec PER_vector;
		itpp::vec PER_meta_vector;
		bool is_BER_on, is_full_estimation_on, is_channel_smoothing_on, is_fixed_input_on, is_process_timing_on;
};

#endif /* _INCLUDED_TEST_80211N_H_ */

