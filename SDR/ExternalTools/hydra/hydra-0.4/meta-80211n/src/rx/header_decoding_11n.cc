/*  	
 * Title:  Header decoding for 802.11n preambles
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Class definition for header decoding of 802.11n preambles
 *
 * Revision: v0.00 - 1/02/2007 - Initial Release
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

#include "header_decoding_11n.h"
#include "frame_detection_11n.h"
#include "frame_alignment_11n.h"
#include "cfo_estimation_11n.h"
#include "channel_estimation_11n.h"
#include "signal_field_decoding_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

// need to set RSSI, REC_MCS, RCPI, non_ht parameters

Header_Decoding_11n::Header_Decoding_11n()
{
	channel="0";
	synchronization_search_length=200;
	required_legacy_symbols=400;
	required_additional_symbols=80;
	fractional_energy[0]=0.25;
	fractional_energy[1]=0.25;
	fractional_energy[2]=0.25;
	fractional_energy[3]=0.25;
	crc_ok=false; // a successful header decode triggers 'true' value	
	correlation_threshold=0.70;
	timing_tolerance=3; // allow for 0 symbol offset of synchronization
	uncorrelation_ratio=0.1; // based on simulations
	sampling_frequency=1e6;
	using_custom_cpw=false;
	using_custom_epw=false;
	using_custom_ccw=false;
	using_custom_fcw=false;
	average_snr=0;
	packet_detected=false;
}

void Header_Decoding_11n::mrc_decode(itpp::cmat header_symbols, unsigned int& column_update, unsigned int& remaining_columns, rxvector& rx_parameters)
{
}

void Header_Decoding_11n::egc_decode(itpp::cmat header_symbols, unsigned int& column_update, unsigned int& remaining_columns, rxvector& rx_parameters)
{

	// determine preamble format based on rxvector
	rx_parameters.Nfft=64;
	rx_parameters.Ngi=16;
	rx_parameters.Nofdm=80;
	if (!using_custom_cpw)
	{
		coarse_packet_window=rx_parameters.Nfft/2;
	}
	if (!using_custom_epw)
	{
		exact_packet_window=3*rx_parameters.Nofdm/10;
	}
	if (!using_custom_ccw)
	{
		coarse_cfo_window=6*rx_parameters.Nofdm/10;
	}
	if (!using_custom_fcw)
	{
		fine_cfo_window=rx_parameters.Nfft;
	}
	required_legacy_symbols=rx_parameters.Nofdm*5;

	// proceed with packet detection and header decode functions	
	Frame_Detection_11n cpd(coarse_packet_window,correlation_threshold); // coarse packet-detection class
	
	// determine packet start with coarse packet detection
	crc_ok=false;	
	int synch_index=cpd.egc_frame_detect(header_symbols.get_cols(0,synchronization_search_length-1));
	if ((synch_index!=-1)) // if packet detected, continue	
	{
		packet_detected=true;
		if ((required_legacy_symbols+synch_index-1)<header_symbols.cols()) // fail since this implies improper synchronization
		{
			Frame_Alignment_11n epd(exact_packet_window); // exact packet-detection class
			CFO_Estimation_11n ffe(fine_cfo_window,sampling_frequency); // fine frequency offset estimation
			Channel_Estimation_11n cce(rx_parameters); // coarse channel estimation
			Signal_Field_Decoding_11n sig; // signal decoding
	
			// determine coarse channel frequency offset
			c_cfo=ffe.schmidl_cox_egc(header_symbols.get(0,-1,synch_index+rx_parameters.Ngi,synch_index+2*rx_parameters.Ngi+2*coarse_cfo_window-1),true); // sends half an STF symbol
		
			// correct with coarse estimate to decode SIG/HTSIG
			ffe.cfo_correct(c_cfo,header_symbols);
		
			// determine exact symbol index of LTF start
			column_update=synch_index+epd.schmidl_cox_egc(header_symbols.get_cols(synch_index+rx_parameters.Nfft,synch_index+4*rx_parameters.Nfft-1),timing_tolerance)+rx_parameters.Nfft;
      if (0) {
        fprintf(stderr, "[header_decoding_11n] synch_index   = %d\n", synch_index);
        fprintf(stderr, "[header_decoding_11n] column_update = %d\n", column_update);
      }

			// refine channel frequency offset estimate	
			f_cfo=ffe.schmidl_cox_egc(header_symbols.get(0,-1,column_update+rx_parameters.Ngi,column_update+rx_parameters.Ngi+2*rx_parameters.Nfft-1),false); // sends 2 OFDM symbols in 1st LTF field
			// correct with refined estimate
			ffe.cfo_correct(f_cfo,header_symbols);
		
			// find single-stream combined subcarrier tones
			channel=cce.egc_ls_fde(header_symbols.get(0,-1,column_update+2*rx_parameters.Ngi,column_update+2*rx_parameters.Ngi+2*rx_parameters.Nfft-1)); // sends 1st LTF field minus prefix	
			
			average_snr=cce.get_average_snr();
			noise_variance=cce.get_noise_variance();
		
			if ((column_update+5*rx_parameters.Nofdm)<=header_symbols.cols())
			{
				signal_bits=sig.htsig_egc(header_symbols.get_cols(column_update+2*rx_parameters.Nofdm,column_update+5*rx_parameters.Nofdm-1),channel,1/average_snr);
				if (signal_bits=="0")
					crc_ok=false;
				else
				{
					crc_ok=true;
					ht_signal_assign(signal_bits,rx_parameters);
					if (!rx_parameters.get_is_valid())
						crc_ok=false;
				}
				remaining_columns=rx_parameters.Npresamples+rx_parameters.Ndatasamples+rx_parameters.Nmetasamples-2*rx_parameters.Nofdm; // eliminate HT-STF 
			}
		}	 
	}
	else
		packet_detected=false;
}

void Header_Decoding_11n::ht_signal_assign(bvec const& in_bits, rxvector& rx_parameters)
{
	// HT-SIG1 - meta info
	rx_parameters.MCS_meta=bin2dec(in_bits(0,6),false);
	rx_parameters.LENGTH_meta=bin2dec(in_bits(7,18),false);
	// HT-SIG2 - regular payload info
	rx_parameters.MCS=bin2dec(in_bits(19,25),false);
	rx_parameters.LENGTH=bin2dec(in_bits(26,41),false);
	// HT-SIG3 - transmission information
	rx_parameters.SMOOTHING=SMOOTHING_NOT_REC;
	if (in_bits(42,42)==ones_b(1))
		rx_parameters.SMOOTHING=SMOOTHING_REC;
	rx_parameters.NOT_SOUNDING=CONST_NOT_SOUNDING;
	if (in_bits(43,43)==zeros_b(1))
		rx_parameters.NOT_SOUNDING=SOUNDING;
	rx_parameters.AGGREGATION=NOT_AGGREGATED;
	if (in_bits(44,44)==ones_b(1))
		rx_parameters.AGGREGATION=AGGREGATED;
	rx_parameters.STBC=bin2dec(in_bits(45,46),false);
	rx_parameters.LDPC_CODING=BCC_CODING;
	if (in_bits(47,47)==ones_b(1))
		rx_parameters.LDPC_CODING=CONST_LDPC_CODING;
	rx_parameters.NUM_EXTEN_SS=bin2dec(in_bits(48,49),false);
	rx_parameters.calculate_parameters(true);
}

void Header_Decoding_11n::set_sampling_frequency(double fs)
{
	sampling_frequency=fs;
}

void Header_Decoding_11n::set_correlation_threshold(double th)
{
	correlation_threshold=th;
}	

void Header_Decoding_11n::set_coarse_packet_window(unsigned int window_size)
{
	coarse_packet_window=window_size;
	using_custom_cpw=true;
}

void Header_Decoding_11n::set_exact_packet_window(unsigned int window_size)
{
	exact_packet_window=window_size;
	using_custom_epw=true;
}

void Header_Decoding_11n::set_coarse_cfo_window(unsigned int window_size)
{
	coarse_cfo_window=window_size;
	using_custom_ccw=true;
}

void Header_Decoding_11n::set_fine_cfo_window(unsigned int window_size)
{
	fine_cfo_window=window_size;
	using_custom_fcw=true;
}

void Header_Decoding_11n::set_timing_tolerance(unsigned int time_tol)
{
	timing_tolerance=time_tol;
}

