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

// preprocessor directives define simulation
#define number_of_tx_antennas 4 // 1 through 4
#define number_of_rx_antennas 4 // 1 through 4
#define number_of_channel_taps 3 // any positive integer
#define power_delay_profile_mean "1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;1e-3 1e-4 1e-6;" // matrix with columns as vector of channel for each tap (vec (Hk) where k is tap) 
#define power_delay_profile_std "1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;1e-4 1e-5 1e-7;" // same size as channel taps
#define modulation_and_coding_scheme 30 // see XX of 802.11n standard
#define meta_modulation_and_coding_scheme 30 // see XX of 802.11n standard
#define packet_byte_length 1024 // self-explanatory
#define meta_byte_length 500 
#define estimate_all_channel_dimensions false // true adds extension spatial streams
#define frequency_of_operation 2.5e9 // 2.5e9 or 5e9
#define sampling_rate_of_signal 1e6 // 
#define frequency_offset_mean 10e3 // < 1 MHz
#define frequency_offset_variance 0 // assuming gaussian noise
#define number_of_packets_to_run 10 // packets per SNR simulated
#define SNR_value_array "30 33 36 40" // in dB
#define show_sim_messages false // set to true shows simulation messages
#define using_LDPC_codes false // not yet supported
#define using_short_guard_interval false // true gives half the size guard interval
#define space_time_code_index 0 // 0-3 (see XX of standard)
#define packet_detection_method_index 0 // 0 = ...
#define channel_estimation_method_index 0 // 0 = FDE-LS, 1 = TDE-LS
#define equalization_method_index 0 // 0=FDE-LS, 1=TDE-LS, 2=FDE-MMSE, 3=TDE-MMSE,
#define phase_smoothing_on true // true turns on digital PLL
#define packet_detection_threshold 0.75
#define packet_synchronization_window 32
#define BER_analysis_mode true // true of false
#define fixed_input_mode false // true for getting uncoded bits
#define use_channel_smoothing false // true smooths the channel estimates
#define profile_process_timing false // reports the timing of each PHY element

#include <itpp/itbase.h>
#include "test_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

int main()
{
	// *** CREATE WIRELESS CHANNEL ***
	
	// intialize variables
	Array<cmat> H;H.set_size(number_of_channel_taps); 
	cmat PDPm=power_delay_profile_mean;
	cmat PDPs=power_delay_profile_std;
	std::complex<double> iorj(0,1);
	cmat temp_matrix;
	// iterate to create each channel tap
	for (unsigned int index=0; index<number_of_channel_taps; index++)
	{
		RNG_randomize();
		temp_matrix=elem_mult(reshape(transpose(PDPm.get(0,-1,index,index)),number_of_rx_antennas,number_of_tx_antennas),exp(iorj*2*pi*randu(number_of_rx_antennas,number_of_tx_antennas)))+elem_mult(reshape(transpose(PDPs.get(0,-1,index,index)),number_of_rx_antennas,number_of_tx_antennas),randn_c(number_of_rx_antennas,number_of_tx_antennas));
		H(index)=temp_matrix;
	}

	// *** SET UP TEST_80211n *** 
	
	// declare 802.11n physical layer testing class
	test_11n simulation;
	
	// set frequency offset
	simulation.set_frequency_offset_parameters(frequency_offset_mean,frequency_offset_variance,sampling_rate_of_signal);	
	// setup synchronization/preamble detection
	simulation.set_correlation_threshold(packet_detection_threshold);
	simulation.set_packet_detection_window(packet_synchronization_window);
	
	// setup equalization
	simulation.set_estimation_metric(channel_estimation_method_index);
	simulation.set_equalization_method(equalization_method_index);
	
	// set physical layer parameters
	simulation.set_modulation_and_coding(modulation_and_coding_scheme);
	simulation.set_meta_modulation_and_coding(meta_modulation_and_coding_scheme);
	simulation.set_stbc_streams(space_time_code_index);
	simulation.trigger_extension_streams(estimate_all_channel_dimensions);
	simulation.trigger_channel_smoothing(use_channel_smoothing);

	// setup simulation parameters
	vec SNR=SNR_value_array; SNR=pow(10,SNR/10);
	simulation.trigger_BER_analysis(BER_analysis_mode);
	simulation.trigger_fixed_input(fixed_input_mode);
	simulation.trigger_process_timing(profile_process_timing);

	// *** RUN SIMULATION / DISPLAY RESULTS ***
 
	// simulation call
	vec error_vector=simulation.run(number_of_packets_to_run,packet_byte_length,meta_byte_length,H,SNR,show_sim_messages);
	mat error_matrix;
	if (BER_analysis_mode)
		error_matrix=simulation.get_BER_matrix();

	// display results
	cout << "------> Error Percentages: " << endl;
	for (unsigned int errindex=0; errindex<length(SNR); errindex++)  
	{
		cout << "        SNR = " << 10*log10(SNR(errindex));
		cout << ", PER = " << error_vector(errindex);
	 	cout << ", C-BER = ";
		if (BER_analysis_mode)
			cout << error_matrix(0,errindex);
		else
			cout << "N/A";
		cout << ", U-BER = ";
	 	if (BER_analysis_mode&&fixed_input_mode)
			cout << error_matrix(1,errindex) << endl;
		else
			cout << "N/A" << endl;
	}	
	return 0;
}
