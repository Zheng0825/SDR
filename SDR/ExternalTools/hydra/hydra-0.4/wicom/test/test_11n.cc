/*! \file test_11n.cc
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
 *  \brief Test program class that for interfacing with the IEEE
 *  802.11n physical layer 
 *
 *	\author{Robert C. Daniels (rdaniels [at] ece.utexas.edu)}
 *
 * 	- \date{06/29/07} \note{Robert Daniels: Initial Version}
 */

#include <itpp/itcomm.h>
#include <test_11n.h>
#include <transmission_interface_11n.h>
#include <data_decoding_11n.h>
#include <header_decoding_11n.h>

using namespace itpp;
using std::cout;
using std::endl;

test_11n::test_11n()
{
	// default setting for transmitted packets
	sampling_frequency=1e6;
	sampling_time=1/(double(sampling_frequency));
	coarse_window=32;
	correlation_threshold=.70;
	zero_padding_length=25;
	frequency_variance=0;
	is_greenfield=true;
	estimated_frequency_offset=0;
	frequency_offset=0;
	is_full_estimation_on=false;
	is_channel_smoothing_on=false;
	is_fixed_input_on=false;
	is_BER_on=false;
	
	// default TXVECTOR settings
		tx_param.SERVICE="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
		tx_param.TXPWR_LEVEL=1;
		tx_param.MCS=0;
		tx_param.MCS_meta=0;
		tx_param.LENGTH=0;
		tx_param.LENGTH_meta=0;
		tx_param.SMOOTHING=SMOOTHING_NOT_REC;
		tx_param.NOT_SOUNDING=CONST_NOT_SOUNDING;
		tx_param.AGGREGATION=NOT_AGGREGATED;
		tx_param.STBC=0;
		tx_param.LDPC_CODING=BCC_CODING;
		tx_param.NUM_EXTEN_SS=0;
		tx_param.ANTENNA_SET_ON=NO_ANTENNA_SET_ON;
		tx_param.ANTENNA_SET="1 0 0 0 0 0 0 0";
		tx_param.N_TX=1;
		tx_param.EXPANSION_MAT_TYPE=NO_EXPANSION_MAT;
}

vec test_11n::run(unsigned int number_of_packets, unsigned int psdu_length, unsigned int meta_length, Array<cmat> observed_channels, vec snr_values, bool verbose) 
{
	// ----------- SET UP INITIAL PARAMETERS -------------- //
	
	// *** PHY STANDARD SETTINGS AND PHYSICAL ALGORITHMS ***
	unsigned int N_RX=observed_channels(0).rows();
	tx_param.N_TX=observed_channels(0).cols();
	switch(tx_param.N_TX)
	{
		case 2: 
			tx_param.ANTENNA_SET="1 1 0 0 0 0 0 0";
			break;	
		case 3: 
			tx_param.ANTENNA_SET="1 1 1 0 0 0 0 0";
			break;	
		case 4: 
			tx_param.ANTENNA_SET="1 1 1 1 0 0 0 0";
			break;
	}
	tx_param.LENGTH=psdu_length;
	tx_param.LENGTH_meta=meta_length;
	unsigned int coded_psdu_length=0;
	if (is_channel_smoothing_on)
		tx_param.SMOOTHING=SMOOTHING_REC;
	tx_param.calculate_parameters(true); // calculate validity of TXVECTOR
	
	// if full estimation on, add extension spatial streams and recalculate
	// TXVECTOR parameters
	if (is_full_estimation_on)
	{
		tx_param.NUM_EXTEN_SS=tx_param.N_TX-1;
		tx_param.calculate_parameters(true);
	}

	// declare classes interface with transmitter and receiver
	Transmission_Interface_11n tx_front_end; // transmit interface class
	Header_Decoding_11n rx_header; // header decode class
	Data_Decoding_11n rdp; // receiver data processing class
	// set up synchronization and packet detection
	rx_header.set_sampling_frequency(sampling_frequency); // set to 20 MHz
	rx_header.set_correlation_threshold(correlation_threshold);
	rx_header.set_coarse_packet_window(coarse_window);
	if (is_fixed_input_on)
		tx_front_end.trigger_fixed_input_mode(true);
	if (is_BER_on)
		tx_front_end.trigger_data_analysis_mode(true);
	rdp.trigger_buffering_mode(true);

	// *** SIMULATION VARIABLES ***
	PER_vector=zeros(length(snr_values));
	PER_meta_vector=zeros(length(snr_values));
	BER_matrix=zeros(2,length(snr_values));
	cmat input_stream(tx_param.N_TX,tx_param.Nmetasamples+tx_param.Ndatasamples+tx_param.Npresamples);
	unsigned int input_stream_length=input_stream.cols(); 
	unsigned int number_of_taps=observed_channels.size();
	unsigned int output_stream_length=input_stream_length+2*zero_padding_length+number_of_taps-1;
	unsigned int start_index=0; 
	unsigned int tap_index=0; 
	unsigned int snr_index=0; 
	unsigned int generic_index=0;
	vec header_check_index=zeros(length(snr_values));
	bvec input_bits(8*psdu_length);
	bvec meta_bits(8*meta_length);
	bvec output_data_bits(8*psdu_length); 
	bvec output_meta_bits(8*meta_length); 
	cvec generic_vector;
	cmat output_stream(N_RX,output_stream_length);
	cmat generic_matrix=output_stream;
	std::complex<double>  iorj(0,1); // imaginary number
	AWGN_Channel awgn; // for adding additive gaussian noise 
	BERC berc; // for comparing bit error rates
	
	rdp.trigger_rx_timing(is_process_timing_on);
	
	// if desired, output simulation parameters
	if(verbose)
	{
		cout << "ADMIN: SIMULATION" << endl;
		cout << "------> Sampling Frequency = " << sampling_frequency << endl;
		cout << "------> Correlation Threshold = " << correlation_threshold << endl;
		cout << "------> Coarse Packet Detection Window = " << coarse_window << endl;
		cout << "------> Frequency Offset Added: Mean = " << frequency_offset << ", Variance = " << frequency_variance << endl;
		cout << "------> Channel Estimation Metric = " << estimation_mode <<
endl;
		cout << "------> Channel Equalization Method = " << equalization_mode <<
endl;
		cout << "------> BER Analysis Mode = " << is_BER_on << endl;
		cout << "------> Header Gain Control = " << gain_mode << endl;
		cout << "------> MCS Mode = " << tx_param.MCS << endl;
		cout << "------> Ntx = " << tx_param.N_TX << " Nrx = " << N_RX << endl;
		cout << "------> STBC Streams = " << tx_param.STBC << endl;
	}
	
	// ----------- BEGIN SIMULATION -------------- //
	// LOOP FOR EACH PACKET
	for (unsigned int packet_index=0; packet_index<number_of_packets; packet_index++)
	{	
		if (verbose)
		{
			cout << "ADMIN: SIMULATION" << endl;
			cout << "------> Iteration #" << packet_index+1 << " of " << number_of_packets << endl;
		}
		
		// randomize input bits
		RNG_randomize();
		input_bits=randb(psdu_length*8);
		meta_bits=randb(meta_length*8);
		
		// define front end transmitted streams
		tx_front_end.format_frame(tx_param, meta_bits, input_bits, input_stream);
		coded_psdu_length=length(tx_front_end.get_coded_bits())/8;
		// add multipath channel effects
		tap_index=0;
		output_stream.clear();
		for (; tap_index<number_of_taps; tap_index++)
		{
			generic_matrix=observed_channels(tap_index);
			output_stream.set_submatrix(0,N_RX-1,tap_index+zero_padding_length,zero_padding_length+input_stream_length+tap_index-1,generic_matrix*input_stream+output_stream.get_cols(tap_index+zero_padding_length,zero_padding_length+input_stream_length+tap_index-1));
		}
		
				
		// add channel frequency offset 
		generic_index=0;
		for (; generic_index<input_stream_length+number_of_taps-1; generic_index++)
		{
			output_stream.set_col(generic_index+zero_padding_length,output_stream.get_col(generic_index+zero_padding_length)*exp(iorj*2*pi*frequency_offset*sampling_time*double(generic_index)));
		}


		// iterate through SNR values
		snr_index=0;
		generic_matrix=output_stream; // hold on to noise-free output
		for (; snr_index<length(snr_values); snr_index++)
		{
			if (verbose)
			{
				cout << "ADMIN: SIMULATION" << endl;
				cout << "------> SNR Index: " << snr_values(snr_index) << endl;
			}
			
			// reinitialize output stream to noise-free output
			output_stream=generic_matrix;
			
			// initialize noise power
			awgn.set_noise(1/double(snr_values(snr_index)));

			// add noise to each receive antenna stream
			generic_index=0;
			for (; generic_index<N_RX; generic_index++)
			{
				generic_vector=generic_matrix.get_row(generic_index);
				output_stream.set_row(generic_index,awgn(generic_vector));
			}

			// perform rx_header synchronization and decoding
			start_index=0; unsigned int symbols_left=0;
			
			// ensure receiver knows the preamble format (could be reset by
			// incorrect header field decoding)
			
			rx_header.egc_decode(output_stream.get_cols(0,tx_param.Npresamples+tx_param.Ngi+zero_padding_length),start_index,symbols_left,rx_param);
			
			// perform actions based on header/synchronization feedback
			if (!rx_header.is_packet_detected()) // no packet found
			{
				if (verbose)
				{
					cout << "ADMIN: HEADER DECODE" << endl;
					cout << "------> NO PACKET DETECTED!" << endl;
				}
				PER_vector(snr_index)+=1;
				PER_meta_vector(snr_index)+=1;
			}
			else // packet found
			{
				if (verbose) 
				{
					cout << "ADMIN: HEADER DECODE" << endl;
					cout << "------> Coarse CFO: " << (rx_header.get_coarse_cfo()) << endl;
					cout << "------> Fine CFO: " << (rx_header.get_fine_cfo()) << endl;
					cout << "------> Synch Index: " << (start_index) << endl;
					cout << "------> Average SNR: " << 10*log10(rx_header.get_avg_snr()) << endl;
					cout << "------> CRC Check: ";
				}
				
				// display CRC check 
				if ((rx_header.is_crc_ok()==false)||(tx_param.LENGTH!=rx_param.LENGTH))
				{
					PER_vector(snr_index)+=1;
					PER_meta_vector(snr_index)+=1;
					if (verbose)
						cout << "FAILED!" << endl;
				}
				else
				{
					// only process data portion if header CRC check passes
					if (verbose)
						cout << "PASSED!" << endl;
					header_check_index(snr_index)+=1;
					
					// retrieve frequency offset
					estimated_frequency_offset=rx_header.get_coarse_cfo()+rx_header.get_fine_cfo();
					rdp.set_frequency_offset(estimated_frequency_offset, sampling_frequency);
					//cout << "synch: " << start_index << ", cfo: " << estimated_frequency_offset << endl;
					rdp.recover_bits(rx_param,output_stream.get(0,N_RX-1,start_index,symbols_left+start_index-1),rx_header.get_avg_snr(),output_data_bits,output_meta_bits);
					ppsnr=rdp.get_post_processing_snr();
					extchan=rdp.get_extension_channel();

					// debug
					/*
					Array<cmat> tchan=rdp.get_channel();
					{
						cout << "h11=[";
						for (unsigned int a=0; a<56; a++)
							cout << " " << tchan(a)(0,0);
						cout << "];" << endl;
					}	
					{
						cout << "h12=[";
						for (unsigned int a=0; a<56; a++)
							cout << " " << tchan(a)(0,1);
						cout << "];" << endl;
					}	
					{
						cout << "h21=[";
						for (unsigned int a=0; a<56; a++)
							cout << " " << tchan(a)(1,0);
						cout << "];" << endl;
					}	
					{
						cout << "h22=[";
						for (unsigned int a=0; a<56; a++)
							cout << " " << tchan(a)(1,1);
						cout << "];" << endl;
					}*/
					if (verbose) 
					{
						cout << "ADMIN: RECEIVER DATA PROCESSING" << endl;
						cout << "------> Spatial SNR Matrix: " << endl << 10*log10(rdp.get_spatial_snr()/rx_header.get_noise_variance()) << endl;
						cout << "------> Spatial Correlation Matrix: " << endl << rdp.get_spatial_correlation() << endl;
						cout << "------> Average Condition Number: " << rdp.get_avg_condition_number(true) << endl;
						//cout << "------> All Condition Numbers: " << endl << rdp.get_condition_number(true) << endl;
						cout << "------> Temporal Correlation Matrix: " << endl << rdp.get_temporal_correlation() << endl;
						cout << "------> Uncoded Bit Errors: " << rdp.get_bit_errors(true,rx_param) << endl;
						cout << "------> Extension SNR Matrix: " << endl << 10*log10(rdp.get_extension_snr()) << endl;
					}
										
					berc.clear();
					if (input_bits.length()!=0)
					{
						berc.count(output_data_bits,input_bits);
						if ((berc.get_errors())!=0)
							PER_vector(snr_index)+=1;
					}
					berc.clear();
					if (meta_bits.length()!=0)
					{	
						berc.count(output_meta_bits,meta_bits);
						if ((berc.get_errors())!=0)
							PER_meta_vector(snr_index)+=1;
					}
				}	
			}
		}
	}

	PER_vector=PER_vector*(1/double(number_of_packets));
	PER_meta_vector=PER_meta_vector*(1/double(number_of_packets));
	BER_matrix.set_row(0,elem_div(BER_matrix.get_row(0),double(8*psdu_length)*header_check_index));
	BER_matrix.set_row(1,elem_div(BER_matrix.get_row(1),double(8*coded_psdu_length)*header_check_index));
	//cout << "PER: " << PER_vector << endl;
	//cout << "PERmeta: " << PER_meta_vector << endl;
	return PER_vector;
}

void test_11n::set_frequency_offset_parameters(double cfo, double cfo_variance, double fs)
{
	frequency_offset=cfo;
	sampling_frequency=fs;
	sampling_time=double(1)/fs;
}
