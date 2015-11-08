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
#define number_of_tx_antennas 2 // 1 through 4
#define number_of_rx_antennas 2 // 1 through 4
#define number_of_channel_taps 3 // any positive integer
#define preamble_mode_of_operation "greenfield" // "legacy", "mixed_mode", or "greenfield"
#define estimate_all_channel_dimensions true // true adds extension spatial streams
#define frequency_of_operation 2.5e9 // 2.5e9 or 5e9
#define sampling_rate_of_signal 1e6 // 
#define channel_width_occupied 20 // either 20 or 40 MHz
#define frequency_offset_mean 10e3 // < 1 MHz
#define frequency_offset_variance 0 // assuming gaussian noise
#define number_of_channels_to_run 10000 // packets per SNR simulated
#define channel_scaling_bounds "5 40" // in dB
#define packet_byte_length 1024 // self-explanatory
#define meta_byte_length 64 // self-explanatory
#define meta_modulation_and_coding_scheme 1 //
#define show_sim_messages false // set to true shows simulation messages
#define using_LDPC_codes false // not yet supported
#define using_short_guard_interval false // true gives half the size guard interval
#define space_time_code_index 0 // 0-3 (see XX of standard)
#define packet_detection_method_index 0 // 0 = ...
#define using_MRC_header_decode false // maximum ratio combining for header decoding
#define channel_estimation_method_index 0 // 0 = FDE-LS, 1 = TDE-LS
#define equalization_method_index 0 // 0=FDE-LS, 1=TDE-LS, 2=FDE-MMSE, 3=TDE-MMSE,
#define phase_smoothing_on true // true turns on digital PLL
#define packet_detection_threshold 0.75
#define packet_synchronization_window 32
#define header_decode_mode "EGC" // "EGC" or "MRC"
#define profile_process_timing false // reports the timing of each PHY element

#include "self_adaptation_mimo_ofdm.h"
#include <itpp/itbase.h>
#include "test_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

int main()
{
	// self adaptation objects
	int data_size=2000;
	int error_size=1000;
	Self_Adaptation_MIMO_OFDM sad(2,0,0,0,5,packet_byte_length,56,64,2);
	sad.initialize_database(data_size,error_size);
	sad.load_database("1000x1000_4dim_1024B_so_mcs_0_15.sad");
	sad.trigger_diversity_preservation_mode(true,(10/pow(0.35,4)));
	// intialize variables
	Array<cmat> H;H.set_size(number_of_channel_taps); 
	std::complex<double> iorj(0,1);
	vec channel_bounds=channel_scaling_bounds;
	double channel_scalar;
	vec trate="6.5 13 19.5 26 39 52 58.5 65 13 26 39 52 78 104 119 130";
	vec pervec;
	vec tputvec; 
	// declare 802.11n physical layer testing class
	test_11n simulation;
	simulation.trigger_process_timing(profile_process_timing);

	// set frequency offset
	simulation.set_frequency_offset_parameters(frequency_offset_mean,frequency_offset_variance,sampling_rate_of_signal);	
	
	// setup synchronization/preamble detection
	simulation.set_correlation_threshold(packet_detection_threshold);
	simulation.set_packet_detection_window(packet_synchronization_window);
	if (header_decode_mode=="EGC")
		simulation.set_gain_control(0);
	else if (header_decode_mode=="MRC")
		simulation.set_gain_control(1);
	
	// setup equalization
	simulation.set_estimation_metric(channel_estimation_method_index);
	simulation.set_equalization_method(equalization_method_index);
		
	// set physical layer parameters
	simulation.set_stbc_streams(space_time_code_index);
	simulation.trigger_extension_streams(estimate_all_channel_dimensions);
	simulation.trigger_process_timing(profile_process_timing);
	simulation.set_meta_modulation_and_coding(meta_modulation_and_coding_scheme);
	
	// *** RUN SIMULATION / DISPLAY RESULTS ***
	int mcs=0;
	int Ns=std::min(number_of_tx_antennas,number_of_rx_antennas);
	Array<cmat> extchan(Ns);
	mat ppsnr;
	vec error_vector; 
	int chan_counter=0, err_counter=0;
	double tput_counter=0;

	// iterate to create each channel tap
	for (unsigned int index=0; index<number_of_channels_to_run; index++)
	{

		// generate channel realization
		RNG_randomize();
		channel_scalar=sqrt(pow(10,((channel_bounds(1)-channel_bounds(0))*randu()+channel_bounds(0))/10));
		double chan_power=0;
		int tapidx;
		for (tapidx=0; tapidx<number_of_channel_taps; tapidx++)
		{	
			RNG_randomize();
			H(tapidx)=randn_c(number_of_rx_antennas,number_of_tx_antennas);
			chan_power+=sqr(norm(H(tapidx),"fro"));
		}
		for (tapidx=0; tapidx<number_of_channel_taps; tapidx++)
			H(tapidx)=H(tapidx)*(std::complex<double>(channel_scalar/sqrt(chan_power)));
		
		// find recommended MCS
		simulation.set_modulation_and_coding(0);
		if (!(simulation.run(1,1,0,H,to_vec(1),false))(0))
		{
			//simulation.run(1,packet_byte_length,meta_byte_length,H,to_vec(1),show_sim_messages);
			extchan=simulation.get_extension_channel();
			mcs=sad.recommend_parameters(extchan,1);
			if (mcs==-1)
			{
				cout << "---> Failure Probability High >>> ";
				simulation.set_modulation_and_coding(0);
				error_vector=simulation.run(1,packet_byte_length,meta_byte_length,H,to_vec(1),show_sim_messages);
			//		cout << "---> Avg SNR: " << 10*log10(sumsum(simulation.get_post_processing_snr())/(52)) << " >>> ";
				sad.update_database(simulation.get_extension_channel(),1,0,(error_vector(0)!=0));
				if (error_vector(0)==1)
				{
					cout << " Fail" << endl;
				}
				else
				{
					cout << " Pass" << endl;
				}
			}	
			else
			{
				chan_counter+=1;
				// simulation call
				cout << "---> MCS-" << index << " Recommended/Tested: " << mcs ;
				//mcs=randi(0,15);
				cout << "/" << mcs << " >>> ";
				simulation.set_modulation_and_coding(mcs);
				error_vector=simulation.run(1,packet_byte_length,meta_byte_length,H,to_vec(1),show_sim_messages);
				cout << "---> Min/Avg/Max SNR: " << 10*log10(min(min(simulation.get_post_processing_snr()))) << "/" << 10*log10(sumsum(simulation.get_post_processing_snr())/(((mcs/8)+1)*52)) << "/" << 10*log10(max(max((simulation.get_post_processing_snr())))) << " >>> ";
				if (error_vector(0)==1)
				{
					cout << "***FAIL***" << endl;
					err_counter+=1;
				}
				else
				{
					tput_counter+=trate(mcs);
					cout << "   Pass" << endl;
				}
				// update database with result
				sad.update_database(simulation.get_extension_channel(),1,mcs,(error_vector(0)!=0));
			}
			if (chan_counter==100)
			{
				pervec=concat(pervec,double(err_counter)/100);
				tputvec=concat(tputvec,(tput_counter/100));
				cout << "Rate Avg = " << (tput_counter/100) << endl;
				cout << "Packet Error Rate = " << double(err_counter)/100 << endl;
				chan_counter=0;
				err_counter=0;
		 		tput_counter=0;
			}
		}
		else
		{
			cout << "!!!!synch failed!!!! ---> scalar = " << channel_scalar << endl;
		}
	}
	cout << "tputvec=" << tputvec << endl;
	cout << "pervec=" << pervec << endl;
	sad.print_parameters(std::cout);
	sad.print_data(std::cout,false);
	//sad.save_database("1000x1000_4dim_1024B_so_mcs_0_15.sad");
	return 0;	
}
