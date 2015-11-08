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
 *  \brief Test program class that for interfacing with the IEEE
 *  802.11n physical layer 
 *
 *	\author{Robert C. Daniels (rdaniels [at] ece.utexas.edu)}
 *
 * 	- \date{06/29/07} \note{Robert Daniels: Initial Version}
 */

#include "test_11n.h"
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
	zero_padding_length=25;
	frequency_variance=0;
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
	unsigned int number_of_taps=observed_channels.size();
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
	unsigned int input_stream_length=input_stream.cols(); 
	unsigned int output_stream_length=input_stream_length+2*zero_padding_length+number_of_taps-1;
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
		cout << "------> Frequency Offset Added: Mean = " << frequency_offset << ", Variance = " << frequency_variance << endl;
		cout << "------> Channel Estimation Metric = " << estimation_mode <<
endl;
		cout << "------> Channel Equalization Method = " << equalization_mode <<
endl;
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
		//cout << "input=[" << input_stream << "];" << endl;
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
		//for (; generic_index<input_stream_length+number_of_taps-1; generic_index++)
		//{
		//	output_stream.set_col(generic_index+zero_padding_length,output_stream.get_col(generic_index+zero_padding_length)*exp(iorj*2*pi*frequency_offset*sampling_time*double(generic_index)));
		//}

		// iterate through SNR values
		snr_index=0;
		generic_matrix=output_stream; // hold on to noise-free output
		double channel_power=0;
		for (; snr_index<length(snr_values); snr_index++)
		{
			if (verbose)
			{
				cout << "ADMIN: SIMULATION" << endl;
				cout << "------> SNR Index: " << snr_values(snr_index) << endl;
			}
			
			// initialize noise power (assumes unit power channel)
			channel_power=0;
			for (unsigned int aa=0; aa<number_of_taps; aa++)
				channel_power+=sumsum(sqr(abs(observed_channels(aa))));
			awgn.set_noise(1/double(snr_values(snr_index)));
			// add noise to each receive antenna stream
			for (generic_index=0; generic_index<N_RX; generic_index++)
			{
				generic_vector=generic_matrix.get_row(generic_index);
				output_stream.set_row(generic_index,awgn(generic_vector));
			}

			// ensure receiver knows the preamble format (could be reset by
			// incorrect header field decoding)
			rx_param.MCS=tx_param.MCS;
			rx_param.MCS_meta=tx_param.MCS_meta;
			rx_param.LENGTH=tx_param.LENGTH;
			rx_param.LENGTH_meta=tx_param.LENGTH_meta;
			rx_param.SMOOTHING=tx_param.SMOOTHING;
			rx_param.NOT_SOUNDING=tx_param.NOT_SOUNDING;
			rx_param.AGGREGATION=tx_param.AGGREGATION;
			rx_param.STBC=tx_param.STBC;
			rx_param.LDPC_CODING=tx_param.LDPC_CODING;
			rx_param.NUM_EXTEN_SS=tx_param.NUM_EXTEN_SS;
			rx_param.calculate_parameters(true); 

			// perform rx_header synchronization and decoding
			start_index=zero_padding_length+2*rx_param.Nofdm; 
			unsigned int symbols_left=rx_param.Npresamples+rx_param.Nmetasamples+rx_param.Ndatasamples-2*rx_param.Nofdm; 
			// packet found
			header_check_index(snr_index)+=1;
					
			// retrieve frequency offset
			estimated_frequency_offset=0;
			rdp.set_frequency_offset(estimated_frequency_offset, sampling_frequency);
			rdp.set_equalization_mode(equalization_mode);
			output_data_bits=zeros_b(input_bits.length());
			rdp.recover_bits(rx_param,output_stream.get(0,N_RX-1,start_index,symbols_left+start_index-1),1/snr_values(snr_index),output_data_bits,output_meta_bits);
			ppsnr=rdp.get_post_processing_snr();
			extchan=rdp.get_extension_channel();
			//cout << "ofdm=" << rdp.get_ofdm_symbols(0,1) << ";" << endl;
			//cout << "meta=" << rdp.get_meta_ofdm_symbols(0,1) << ";" << endl;
			/*for (int cc=0; cc<2; cc++)
			{
				for (int bb=0; bb<2; bb++)
				{
					cout << "h" << bb << cc << "=["; 
					for (int aa=0; aa<rx_param.Nst; aa++)
						cout << extchan(aa)(bb,cc) << " ";
					cout << "];" << endl;
				}
			}*/
			if (verbose) 
			{
						cout << "ADMIN: RECEIVER DATA PROCESSING" << endl;
						cout << "------> Average Spatial SNR: " << 10*log10(rdp.get_spatial_snr()) << endl;
						cout << "------> Average Condition Number: " << rdp.get_avg_condition_number(true) << endl;
			}
			//cout << "input=" << input_bits << ";" << endl;						
			//cout << "output=" << output_data_bits << ";" << endl;						
			//cout << "input=" << meta_bits << ";" << endl;						
			//cout << "output=" << output_meta_bits << ";" << endl;						
			// Are there any errors in the packet?
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

	PER_vector=PER_vector*(1/double(number_of_packets));
	PER_meta_vector=PER_meta_vector*(1/double(number_of_packets));
	BER_matrix.set_row(0,elem_div(BER_matrix.get_row(0),double(8*psdu_length)*header_check_index));
	BER_matrix.set_row(1,elem_div(BER_matrix.get_row(1),double(8*coded_psdu_length)*header_check_index));
	chan_cond=rdp.get_avg_condition_number(false);
	avg_snr=rdp.get_avg_snr();
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
