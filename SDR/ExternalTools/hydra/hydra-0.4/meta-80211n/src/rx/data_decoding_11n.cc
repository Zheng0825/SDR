/*  	
 * Title:  Receiever Data Processsing (RDP)
 * Created By: Robert Daniels
 * Creation Date: 	
 * Description:  
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

/*
 * Compensate for residual frequency offset using linear interpolation
 * technique from Wonsoo Kim.
 */
#define RESIDUAL_OFFSET_CORRECTION    0

#include "data_decoding_11n.h"
#include "spatial_deparsing_11n.h"
#include "soft_demapping_11n.h"
#include "spatial_equalization_11n.h"
#include "deinterleaving_11n.h"
#include "descrambling_11n.h"
#include "pilot_estimation_11n.h"
#if RESIDUAL_OFFSET_CORRECTION
#include "rfo_correction_11n.h"
#endif
#include "pilot_removal_11n.h"
#include "channel_estimation_11n.h"
#include "signal_field_decoding_11n.h"
#include "space_time_decoding_11n.h"
#include "soft_bits_decoding_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Data_Decoding_11n::Data_Decoding_11n()
{
	// default to frequency domain channel estimation
	estimation_mode=FDE_LS;
	// default to least-squares estimation metric
	equalization_mode=FDE_ZF;
	// default cfo parameters
	sampling_frequency=1e6;frequency_offset=0;
	// default to 52 data symbols per symbol
	Nsd=52;
	// subcarrier energy
	avg_spatial_snr.set_size(4,4); // maximum dimensions
	// past channel initialization
	is_past_channel_initialized=false;
}

void Data_Decoding_11n::recover_bits(rxvector& rx_parameters, cmat received_stream, double noise_variance, bvec& rx_output, bvec& meta_output)
{

// rx_parameters contains PHY parameters sent to MAC from PHY (set in Header_Decoding_11n::header_decode)
// received_stream = matrix with Nrx rows and Nrdata cols holding all received data
// channel_matrices = (Nst) complex channel matrices with dimension (Nrx X Nsts)
	
	// set up phy independent local variables
	Nrx=received_stream.rows();
	unsigned int input_length=received_stream.cols();
	std::complex<double> iorj(0,1); // the imaginary number 'i' or 'j'

	// *** PROCESS LTF SYMBOLS ***
	
	// PROCESS 0: Frequency Offset Compensation
	//cout << "LTF Process 0: " << endl;	
	unsigned int start_ltf_index=0; // variables define sample points of training
	// only remove cfo when large enough to remove
	if (std::abs(sampling_frequency/frequency_offset)<40000)
	{
		double sampling_time=1/sampling_frequency;
		std::complex<double> frequency_constant=-1*iorj*2*pi*frequency_offset*sampling_time;
		// remove frequency offset
		for (unsigned int a=0; a<Nrx; a++)
		{
			for (int b=0; b<input_length; b++)
			{
				received_stream(a,b)*=exp(frequency_constant*b);
			}
		}
	}
	unsigned int end_data_ltf_index=start_ltf_index+rx_parameters.Ndltf*rx_parameters.Nofdm-1; 
	unsigned int end_ext_ltf_index=end_data_ltf_index+rx_parameters.Neltf*rx_parameters.Nofdm; 
	// for greenfield preambles, the signal field is embedded in LTFs;
	// copy averaged LTFs into SIGNAL field location;
	{
		cmat noise_variance_mat(Nrx,rx_parameters.Nfft);
		for (unsigned int a=0; a<Nrx; a++)
		{
			for (unsigned int b=start_ltf_index; b<(start_ltf_index+rx_parameters.Nfft); b++)
			{
				// equally weight double length LTF temporally
				received_stream(a,b+4*rx_parameters.Nofdm+rx_parameters.Ngi)=0.5*(received_stream(a,b+2*rx_parameters.Ngi)+received_stream(a,b+rx_parameters.Nfft+2*rx_parameters.Ngi));
				noise_variance_mat(a,b-start_ltf_index)=received_stream(a,b+2*rx_parameters.Ngi)-received_stream(a,b+rx_parameters.Nfft+2*rx_parameters.Ngi);
			}
		}
		start_ltf_index+=4*rx_parameters.Nofdm;
		end_data_ltf_index+=4*rx_parameters.Nofdm;
		end_ext_ltf_index+=4*rx_parameters.Nofdm;
		noise_variance=0;
		for (unsigned int b=0; b<Nrx; b++)
		{
			noise_variance+=variance(noise_variance_mat.get_row(b));
		}
		noise_variance/=(2*Nrx);
	}
	// PROCESS  1: Spatial Channel Estimation
	// cout << "LTF Process 1: " << endl;	
	Channel_Estimation_11n sce(rx_parameters); // define spatial channel estimation class
	
	// in buffer mode, store past channel information
	if (is_buffering_mode&&is_past_channel_initialized)
		past_channel=channel_matrices;

	// Estimate channel matrices, considering the # of extension spatial streams
	channel_matrices.set_size(rx_parameters.Nst); // initialize channel matrix Array
	{
		sce.estimate(received_stream.get_cols(start_ltf_index,end_data_ltf_index),estimation_mode,channel_matrices);
		// if channel smoothing on, smooth channel estimate
		// check to see if extension spatial dimensions are excited 
		if (rx_parameters.NUM_EXTEN_SS!=0)
		{
			extension_channel_matrices.set_size(rx_parameters.Nst);
			sce.extension_estimate(received_stream.get_cols(end_data_ltf_index+1,end_ext_ltf_index),estimation_mode,extension_channel_matrices);
			extension_spatial_snr=sce.get_subcarrier_energy()/noise_variance;
      if(0) {
        std::cerr << "[data_decoding_11n] extension subcarrier energy = " << 10*log10(get_extension_snr()*noise_variance) << " dB\n";
        std::cerr << "                    noise_variance              = " << 10*log10(noise_variance) << " dB\n";
      }
		}
		else
			extension_channel_matrices=channel_matrices;			
	}
	if (is_buffering_mode&&(!is_past_channel_initialized))
	{
		past_channel=channel_matrices;
		is_past_channel_initialized=true;
	}
	double average_snr=0;
	{
		for (int subidx=0; subidx<rx_parameters.Nst; subidx++)
			average_snr+=sumsum(sqr(abs(channel_matrices(subidx))))/(rx_parameters.Nsts*rx_parameters.Nst);
	}

	// remainder of processing is done on an OFDM symbol by symbol basis
	// *** PROCESS DATA SYMBOLS***
	if (rx_parameters.LENGTH_meta>0)
	{
		Array<cmat> meta_channel_matrices;
		sce.meta_channel_conversion(channel_matrices,meta_channel_matrices);
		symbol_by_symbol_decode_meta(rx_parameters, meta_channel_matrices, received_stream, noise_variance, meta_output, end_ext_ltf_index+1);
	}
	if (rx_parameters.LENGTH>0)
		symbol_by_symbol_decode(rx_parameters, received_stream, noise_variance, rx_output, end_ext_ltf_index+1+rx_parameters.Nmetasamples);
	
	// *** POST-PROCESSING ***
	//remove zero padding at the end
	rx_output.set_size(8*rx_parameters.LENGTH,true);
	meta_output.set_size(8*rx_parameters.LENGTH_meta,true);
	
	// store decoded bits in buffer mode for BER comparison
	if (is_buffering_mode)
 		recovered_bits=concat(meta_output,rx_output);
}	

void Data_Decoding_11n::symbol_by_symbol_decode(rxvector& rx_parameters, cmat const& received_stream, double N0, itpp::bvec& decoded_output, unsigned int start_data_index)
{

	Real_Timer rxtimer; double last_time;
	vec rxprocesses=zeros(11); // temporary for debug: to be deleted
	if (is_rx_timing_on)
	{
		rxtimer.tic(); // temporary for debug: to be deleted
	}	
		
	// define local OFDM symbol to process
	Spatial_Equalization_11n deq(rx_parameters.Nsts,rx_parameters.Nst);
	Pilot_Removal_11n rmp(rx_parameters.Nst);
	Pilot_Estimation_11n phc(rx_parameters.Nsts,rx_parameters.Nst);
#if RESIDUAL_OFFSET_CORRECTION
	RFO_Correction_11n rfc(rx_parameters.Nsts,rx_parameters.Nst);
#endif
	Soft_Demapping_11n dem(rx_parameters.Nbpsc,rx_parameters.Ncbps,(equalization_mode==4),channel_matrices);
	Deinterleaving_11n dlv(4*rx_parameters.Nbpsc,13,rx_parameters.Nbpsc);
	Space_Time_Decoding_11n std(rx_parameters.Nsts,rx_parameters.Nss,rx_parameters.Nsd);
	
	// set the equalization parameters for the receiver
	deq.set_equalization_mode(equalization_mode); 
	deq.set_noise_variance(N0);
	
	// remainder of processing is done on an OFDM symbol by symbol basis	
	if (is_buffering_mode)
	{
		frequency_tracking.set_size(rx_parameters.Nsts,rx_parameters.Nsym);
		phase_tracking.set_size(rx_parameters.Nsts,rx_parameters.Nsym);
	}

	mat phase_matrix_1(rx_parameters.Nsts,2);
	mat phase_matrix_2(rx_parameters.Nsts,2);
	unsigned int Nbss=rx_parameters.Ncbps/rx_parameters.Nss;
	
	post_processing_snr.set_size(rx_parameters.Nsts,rx_parameters.Nst);
	cmat ofdm_symbol_1(Nrx,rx_parameters.Nofdm);
	cmat ofdm_symbol_2(Nrx,rx_parameters.Nofdm);
	cmat ofdm_holder_1(Nrx,rx_parameters.Nofdm);
	cmat ofdm_holder_2(Nrx,rx_parameters.Nofdm);
  mat soft_matrix(rx_parameters.Nss,rx_parameters.Nsym*rx_parameters.Ncbps/rx_parameters.Nss);
	mat soft_matrix_temp_1(rx_parameters.Nss,Nbss);
	mat soft_matrix_temp_2(rx_parameters.Nss,Nbss);
	ofdm_symbol_history.set_size(rx_parameters.Nss,rx_parameters.Nsym*rx_parameters.Nsd);

	if (is_rx_timing_on)
	{	
		last_time=rxtimer.toc();
		cout << "ADMIN: RECEIVER" << endl;
		cout << "------> RX Loop Setup Time: " << last_time << endl;
	}
	unsigned int loop_counts=0;
	if (rx_parameters.STBC>0)
		loop_counts=rx_parameters.Nsym/2;
	else
		loop_counts=rx_parameters.Nsym;

	for (unsigned int sym_ind=0; sym_ind<loop_counts; sym_ind++)
	{
		// cout << "Index: " << sym_ind << "/" << loop_counts << endl;
		// PROCESS   0: Format Each OFDM Symbol
		// cout << "Payload Process 0" << endl;
		{
			if (rx_parameters.STBC!=0)
			{
				ofdm_symbol_1=received_stream.get_cols(sym_ind*2*(rx_parameters.Nofdm)+rx_parameters.Ngi+start_data_index,(2*sym_ind+1)*(rx_parameters.Nofdm)+start_data_index-1);
				ofdm_symbol_2=received_stream.get_cols((sym_ind*2+1)*(rx_parameters.Nofdm)+rx_parameters.Ngi+start_data_index,(2*sym_ind+2)*(rx_parameters.Nofdm)+start_data_index-1);
			}
			else
				ofdm_symbol_1=received_stream.get_cols(sym_ind*(rx_parameters.Nofdm)+rx_parameters.Ngi+start_data_index,(sym_ind+1)*(rx_parameters.Nofdm)+start_data_index-1);
			if (is_rx_timing_on)
			{
				rxprocesses(0)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}

		// PROCESS   1: Linear Combining (when beamforming/precoding used)
		// cout << "Payload Process 1" << endl;
		{
			if (is_rx_timing_on)
			{
				rxprocesses(1)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}	

		// PROCESS   2: Equalization/Demultiplexing and take FFT
		// cout << "Payload Process 2" << endl;
		{
			if (rx_parameters.STBC==0)
			{	
				deq.equalize(ofdm_symbol_1,ofdm_holder_1,channel_matrices);
			}
			else
			{
				deq.equalize(ofdm_symbol_1,ofdm_holder_1,channel_matrices);
				deq.equalize(ofdm_symbol_2,ofdm_holder_2,channel_matrices);
			}
			if (sym_ind==0) // only compute PP-SNR on first OFDM symbol
			{
				deq.compute_post_process_snr(channel_matrices,post_processing_snr);
			}
			if (is_rx_timing_on)
			{
				rxprocesses(2)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}
		
		// PROCESS   3: Phase offset correction (from Pilots)
		// cout << "Payload Process 3" << endl;
		{
			if (rx_parameters.STBC==0)
			{
        phc.one_shot_estimate(ofdm_symbol_1, sym_ind,phase_matrix_1);
        phc.digital_phase_smoothing(sym_ind, phase_matrix_1);
				phc.correct(ofdm_symbol_1,phase_matrix_1);
				if ((equalization_mode==4)&&(rx_parameters.Nss>1)) // for ML use holder
				{	
					phc.correct(ofdm_holder_1,phase_matrix_1);
				}

#if RESIDUAL_OFFSET_CORRECTION
        rfc.one_shot_estimate(ofdm_symbol_1, sym_ind,phase_matrix_1);
        rfc.correct(ofdm_symbol_1,phase_matrix_1);
#endif
				// in buffer mode, store tracking information
				if (is_buffering_mode)
				{
					phase_tracking.set_col(sym_ind,(phase_matrix_1.get_col(0)));
					frequency_tracking.set_col(sym_ind,(phase_matrix_1.get_col(1)));
				}
			}
			else
			{
				phc.one_shot_estimate(ofdm_symbol_1, 2*sym_ind,phase_matrix_1);
		    phc.digital_phase_smoothing(2*sym_ind, phase_matrix_1);
				phc.correct(ofdm_symbol_1,phase_matrix_1);
				phc.correct(ofdm_holder_1,phase_matrix_1);

#if RESIDUAL_OFFSET_CORRECTION
        rfc.one_shot_estimate(ofdm_symbol_1, 2*sym_ind,phase_matrix_1);
				rfc.correct(ofdm_symbol_1,phase_matrix_1);
				rfc.correct(ofdm_holder_1,phase_matrix_1);
#endif

				phc.one_shot_estimate(ofdm_symbol_2, 2*sym_ind+1,phase_matrix_2);
		    phc.digital_phase_smoothing(2*sym_ind+1, phase_matrix_2);
				phc.correct(ofdm_symbol_2,phase_matrix_2);
				phc.correct(ofdm_holder_2,phase_matrix_2);
        
#if RESIDUAL_OFFSET_CORRECTION
        rfc.one_shot_estimate(ofdm_symbol_2, 2*sym_ind+1,phase_matrix_2);
				rfc.correct(ofdm_symbol_2,phase_matrix_2);
				rfc.correct(ofdm_holder_2,phase_matrix_2);
#endif
				// in buffer mode, store tracking information
				if (is_buffering_mode)
				{
					phase_tracking.set_col(2*sym_ind,(phase_matrix_1.get_col(0)));
					phase_tracking.set_col(2*sym_ind+1,(phase_matrix_2.get_col(0)));
					frequency_tracking.set_col(2*sym_ind,(phase_matrix_1.get_col(1)));
					frequency_tracking.set_col(2*sym_ind+1,(phase_matrix_2.get_col(1)));
				}
			}
			if (is_rx_timing_on)
			{
				rxprocesses(3)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}	
		
		// PROCESS   4: Remove Pilots and Null Tones
		// cout << "Payload Process 4" << endl;
		{
			if (rx_parameters.STBC==0)
			{
				rmp.remove(ofdm_symbol_1);
				if ((equalization_mode==4)&&(rx_parameters.Nss>1)) // for ML
				{
					rmp.remove(ofdm_holder_1);
				}
			}
			else
			{
				rmp.remove(ofdm_symbol_1);
				rmp.remove(ofdm_symbol_2);
				rmp.remove(ofdm_holder_1);
				rmp.remove(ofdm_holder_2);
			}
			if (is_rx_timing_on)
			{
				rxprocesses(4)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}	
			if (sym_ind==0) // only handle PP-SNR on first symbol
			{
				rmp.remove(post_processing_snr);
				post_processing_snr=post_processing_snr/N0;
				avg_spatial_snr=sum(post_processing_snr,2)/post_processing_snr.cols();
			}
		}
		// PROCESS   5: Space-time Block Decoding
		// cout << "Payload Process 5" << endl;
		{
			if (rx_parameters.STBC!=0)
			{
				std.decode(ofdm_holder_1,ofdm_holder_2,ofdm_symbol_1,ofdm_symbol_2,channel_matrices);
				if (is_buffering_mode) // maintain symbol history in buffering mode
				{
					ofdm_symbol_history.set_submatrix(0,2*sym_ind*rx_parameters.Nsd,ofdm_symbol_1);		
					ofdm_symbol_history.set_submatrix(0,(2*sym_ind+1)*rx_parameters.Nsd,ofdm_symbol_2);		
				}
			}
			else
			{
				if (is_buffering_mode) // maintain symbol history in buffering mode
					ofdm_symbol_history.set_submatrix(0,sym_ind*rx_parameters.Nsd,ofdm_symbol_1);
			}
			if (is_rx_timing_on)
			{
				rxprocesses(5)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}
		
		// PROCESS   6: Demodulate into Soft Bits
		//cout << "Payload Process 6" << endl;
		{
			soft_matrix_temp_1=dem.demodulate_soft_bits(rx_parameters.Nbpsc,ofdm_symbol_1,ofdm_holder_1,post_processing_snr,N0);
			if (rx_parameters.STBC!=0)
				soft_matrix_temp_2=dem.demodulate_soft_bits(rx_parameters.Nbpsc,ofdm_symbol_2,ofdm_holder_2,post_processing_snr,N0);
			if (is_rx_timing_on)
			{
				rxprocesses(6)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}	
				
		// PROCESSS  7: Deinterleave				
		//cout << "Payload Process 7" << endl;
		{
			dlv.deinterleave(soft_matrix_temp_1);
			if (rx_parameters.STBC!=0)
			{
				dlv.deinterleave(soft_matrix_temp_2);
				soft_matrix.set_submatrix(0,sym_ind*2*Nbss,soft_matrix_temp_1);
				soft_matrix.set_submatrix(0,(sym_ind*2+1)*Nbss,soft_matrix_temp_2);
			}
			else
				soft_matrix.set_submatrix(0,sym_ind*Nbss,soft_matrix_temp_1);
			if (is_rx_timing_on)
			{
				rxprocesses(7)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}	
	} // end of per OFDM symbol processing
	
	// PROCESS  8: De-parser
	// cout << "Payload Process 8" << endl;
	{
		Spatial_Deparsing_11n css(rx_parameters.Nsd);
		css.combine(soft_matrix,single_stream_soft_bits,rx_parameters.mod_index);
		if (is_rx_timing_on)
		{
			rxprocesses(8)+=rxtimer.toc()-last_time;
			last_time=rxtimer.toc();
		}
	}
	
	// PROCESS  9: Viterbi Decoder
	// cout << "Payload Process 9" << endl;
	{
		Soft_Bits_Decoding_11n vit(rx_parameters.code_rate);
	 	decoded_output=vit.decode(0,single_stream_soft_bits); // currently hard coded to BCC	
		decoded_output.set_size(rx_parameters.LENGTH*8+16,true);
		if (is_rx_timing_on)
		{
			rxprocesses(9)+=rxtimer.toc()-last_time;
			last_time=rxtimer.toc();
		}
	}
		
	// PROCESS 10: De-scrambler
	//cout << "Payload Process 10" << endl;
	{
		Descrambling_11n dsc;
		dsc.descramble(decoded_output);
		if (is_rx_timing_on)
		{
			rxprocesses(10)+=rxtimer.toc()-last_time;
			last_time=rxtimer.toc();
		}
	}

	// if the timing is being profiled, print times
	if (is_rx_timing_on)
	{
		cout << "------> Symbol Formatting time: " << rxprocesses(0) << endl;
		cout << "------> Combining time: " << rxprocesses(1) << endl;
		cout << "------> Equalization time: " << rxprocesses(2) << endl;
		cout << "------> Phase Correction time: " << rxprocesses(3) << endl;
		cout << "------> Remove Pilots/Null tones time: " << rxprocesses(4) << endl;
		cout << "------> STBC Decode time: " << rxprocesses(5) << endl;
		cout << "------> Demodulation time: " << rxprocesses(6) << endl;
		cout << "------> Deinterleave time: " << rxprocesses(7) << endl;
		cout << "------> Deparse time: " << rxprocesses(8) << endl;
		cout << "------> Decode time: " << rxprocesses(9) << endl;
		cout << "------> Descramble time: " << rxprocesses(10) << endl;
	}
}

void Data_Decoding_11n::symbol_by_symbol_decode_meta(rxvector& rx_parameters, Array<cmat> const& meta_channel, cmat const& received_stream, double N0, itpp::bvec& decoded_output, unsigned int start_data_index)
{
	Real_Timer rxtimer; double last_time;
	vec rxprocesses=zeros(11); // temporary for debug: to be deleted
	if (is_rx_timing_on)
	{
		rxtimer.tic(); // temporary for debug: to be deleted
	}	
		
	// define local OFDM symbol to process
	Spatial_Equalization_11n deq(rx_parameters.Nsts_meta,rx_parameters.Nst);
	Pilot_Removal_11n rmp(rx_parameters.Nst);
	Pilot_Estimation_11n phc(rx_parameters.Nsts_meta,rx_parameters.Nst);
#if RESIDUAL_OFFSET_CORRECTION
	RFO_Correction_11n rfc(rx_parameters.Nsts_meta,rx_parameters.Nst);
#endif
  Soft_Demapping_11n dem(rx_parameters.Nbpsc_meta,rx_parameters.Ncbps_meta,(equalization_mode==4),meta_channel);
	Deinterleaving_11n dlv(4*rx_parameters.Nbpsc_meta,13,rx_parameters.Nbpsc_meta);
	Space_Time_Decoding_11n std(rx_parameters.Nsts_meta,rx_parameters.Nss_meta,rx_parameters.Nsd);
	
	// set the equalization parameters for the receiver
	deq.set_equalization_mode(equalization_mode); 
	deq.set_noise_variance(N0);
	
	// remainder of processing is done on an OFDM symbol by symbol basis	
	if (is_buffering_mode)
	{
		frequency_tracking.set_size(rx_parameters.Nsts_meta,rx_parameters.Nsym_meta);
		phase_tracking.set_size(rx_parameters.Nsts_meta,rx_parameters.Nsym_meta);
	}

	mat phase_matrix_1(rx_parameters.Nsts_meta,2);
	mat phase_matrix_2(rx_parameters.Nsts_meta,2);
	unsigned int Nbss=rx_parameters.Ncbps_meta/rx_parameters.Nss_meta;
	meta_post_processing_snr.set_size(rx_parameters.Nsts_meta,rx_parameters.Nst);
	cmat ofdm_symbol_1(Nrx,rx_parameters.Nofdm);
	cmat ofdm_symbol_2(Nrx,rx_parameters.Nofdm);
	cmat ofdm_holder_1(Nrx,rx_parameters.Nofdm);
	cmat ofdm_holder_2(Nrx,rx_parameters.Nofdm);
  mat soft_matrix(rx_parameters.Nss_meta,rx_parameters.Nsym_meta*rx_parameters.Ncbps_meta);
	mat soft_matrix_temp_1(rx_parameters.Nss_meta,Nbss);
	mat soft_matrix_temp_2(rx_parameters.Nss_meta,Nbss);
	meta_ofdm_symbol_history.set_size(rx_parameters.Nss_meta,rx_parameters.Nsym_meta*rx_parameters.Nsd);

	if (is_rx_timing_on)
	{	
		last_time=rxtimer.toc();
		cout << "ADMIN: RECEIVER" << endl;
		cout << "------> RX Loop Setup Time: " << last_time << endl;
	}
	unsigned int loop_counts=0;
	if (rx_parameters.STBC>0)
		loop_counts=rx_parameters.Nsym_meta/2;
	else
		loop_counts=rx_parameters.Nsym_meta;
	for (unsigned int sym_ind=0; sym_ind<loop_counts; sym_ind++)
	{
		// cout << "Index: " << sym_ind << "/" << loop_counts << endl;
		// PROCESS   0: Format Each OFDM Symbol
		// cout << "Payload Process 0" << endl;
		{
			if (rx_parameters.STBC!=0)
			{
				ofdm_symbol_1=received_stream.get_cols(sym_ind*2*(rx_parameters.Nofdm)+rx_parameters.Ngi+start_data_index,(2*sym_ind+1)*(rx_parameters.Nofdm)+start_data_index-1);
				ofdm_symbol_2=received_stream.get_cols((sym_ind*2+1)*(rx_parameters.Nofdm)+rx_parameters.Ngi+start_data_index,(2*sym_ind+2)*(rx_parameters.Nofdm)+start_data_index-1);
			}
			else
				ofdm_symbol_1=received_stream.get_cols(sym_ind*(rx_parameters.Nofdm)+rx_parameters.Ngi+start_data_index,(sym_ind+1)*(rx_parameters.Nofdm)+start_data_index-1);
			if (is_rx_timing_on)
			{
				rxprocesses(0)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}
		// PROCESS   1: Linear Combining (when beamforming/precoding used)
		// cout << "Payload Process 1" << endl;
		{
			if (is_rx_timing_on)
			{
				rxprocesses(1)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}	

		// PROCESS   2: Equalization/Demultiplexing and take FFT
		// cout << "Payload Process 2" << endl;
		{
			if (rx_parameters.STBC==0)
			{	
				deq.equalize(ofdm_symbol_1,ofdm_holder_1,meta_channel);
			}
			else
			{
				deq.equalize(ofdm_symbol_1,ofdm_holder_1,meta_channel);
				deq.equalize(ofdm_symbol_2,ofdm_holder_2,meta_channel);
			}
			if (sym_ind==0) // only compute PP-SNR on first OFDM symbol
			{
				deq.compute_post_process_snr(meta_channel,meta_post_processing_snr);
			}
			if (is_rx_timing_on)
			{
				rxprocesses(2)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}

		// PROCESS   3: Phase offset correction (from Pilots)
		// cout << "Payload Process 3" << endl;
		{
			if (rx_parameters.STBC==0)
			{
        phc.one_shot_estimate(ofdm_symbol_1, sym_ind,phase_matrix_1);
        phc.digital_phase_smoothing(sym_ind, phase_matrix_1);
        phc.correct(ofdm_symbol_1,phase_matrix_1);
				if ((equalization_mode==4)&&(rx_parameters.Nss_meta>1)) // for ML use holder
				{	
					phc.correct(ofdm_holder_1,phase_matrix_1);
				}

#if RESIDUAL_OFFSET_CORRECTION
        rfc.one_shot_estimate(ofdm_symbol_1, sym_ind,phase_matrix_1);
        rfc.correct(ofdm_symbol_1,phase_matrix_1);
#endif
				// in buffer mode, store tracking information
				if (is_buffering_mode)
				{
					phase_tracking.set_col(sym_ind,(phase_matrix_1.get_col(0)));
					frequency_tracking.set_col(sym_ind,(phase_matrix_1.get_col(1)));
				}
			}
			else
			{
				phc.one_shot_estimate(ofdm_symbol_1, 2*sym_ind,phase_matrix_1);
		    phc.digital_phase_smoothing(2*sym_ind, phase_matrix_1);
				phc.correct(ofdm_symbol_1,phase_matrix_1);
				phc.correct(ofdm_holder_1,phase_matrix_1);

#if RESIDUAL_OFFSET_CORRECTION
        rfc.one_shot_estimate(ofdm_symbol_1, 2*sym_ind,phase_matrix_1);
        rfc.correct(ofdm_symbol_1,phase_matrix_1);
        rfc.correct(ofdm_holder_1,phase_matrix_1);
#endif

        phc.one_shot_estimate(ofdm_symbol_2, 2*sym_ind+1,phase_matrix_2);
        phc.digital_phase_smoothing(2*sym_ind+1, phase_matrix_2);
				phc.correct(ofdm_symbol_2,phase_matrix_2);
				phc.correct(ofdm_holder_2,phase_matrix_2);

#if RESIDUAL_OFFSET_CORRECTION
        rfc.one_shot_estimate(ofdm_symbol_2, 2*sym_ind+1,phase_matrix_2);
        rfc.correct(ofdm_symbol_2,phase_matrix_2);
        rfc.correct(ofdm_holder_2,phase_matrix_2);
#endif
				// in buffer mode, store tracking information
				if (is_buffering_mode)
				{
					phase_tracking.set_col(2*sym_ind,(phase_matrix_1.get_col(0)));
					phase_tracking.set_col(2*sym_ind+1,(phase_matrix_2.get_col(0)));
					frequency_tracking.set_col(2*sym_ind,(phase_matrix_1.get_col(1)));
					frequency_tracking.set_col(2*sym_ind+1,(phase_matrix_2.get_col(1)));
				}
			}
			if (is_rx_timing_on)
			{
				rxprocesses(3)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}	
		
		// PROCESS   4: Remove Pilots and Null Tones
		// cout << "Payload Process 4" << endl;
		{
			if (rx_parameters.STBC==0)
			{
				rmp.remove(ofdm_symbol_1);
				if ((equalization_mode==4)&&(rx_parameters.Nss_meta>1)) // for ML
				{
					rmp.remove(ofdm_holder_1);
				}
			}
			else
			{
				rmp.remove(ofdm_symbol_1);
				rmp.remove(ofdm_symbol_2);
				rmp.remove(ofdm_holder_1);
				rmp.remove(ofdm_holder_2);
			}
			if (is_rx_timing_on)
			{
				rxprocesses(4)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}	
			if (sym_ind==0) // only handle PP-SNR on first symbol
			{
				rmp.remove(meta_post_processing_snr);
				meta_post_processing_snr=meta_post_processing_snr/N0;
			}
		}
		// PROCESS   5: Space-time Block Decoding
		// cout << "Payload Process 5" << endl;
		{
			if (rx_parameters.STBC!=0)
			{
				std.decode(ofdm_holder_1,ofdm_holder_2,ofdm_symbol_1,ofdm_symbol_2,meta_channel);
				if (is_buffering_mode) // maintain symbol history in buffering mode
				{
					meta_ofdm_symbol_history.set_submatrix(0,2*sym_ind*rx_parameters.Nsd,ofdm_symbol_1);		
					meta_ofdm_symbol_history.set_submatrix(0,(2*sym_ind+1)*rx_parameters.Nsd,ofdm_symbol_2);		
				}
			}
			else
			{
				if (is_buffering_mode) // maintain symbol history in buffering mode
					meta_ofdm_symbol_history.set_submatrix(0,sym_ind*rx_parameters.Nsd,ofdm_symbol_1);
			}
			if (is_rx_timing_on)
			{
				rxprocesses(5)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}
		
		// PROCESS   6: Demodulate into Soft Bits
		//cout << "Payload Process 6" << endl;
		{
			soft_matrix_temp_1=dem.demodulate_soft_bits(rx_parameters.Nbpsc_meta,ofdm_symbol_1,ofdm_holder_1,meta_post_processing_snr,N0);
			if (rx_parameters.STBC!=0)
				soft_matrix_temp_2=dem.demodulate_soft_bits(rx_parameters.Nbpsc_meta,ofdm_symbol_2,ofdm_holder_2,meta_post_processing_snr,N0);
			if (is_rx_timing_on)
			{
				rxprocesses(6)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}	
				
		// PROCESSS  7: Deinterleave				
		//cout << "Payload Process 7" << endl;
		{
			dlv.deinterleave(soft_matrix_temp_1);
			if (rx_parameters.STBC!=0)
			{
				dlv.deinterleave(soft_matrix_temp_2);
				soft_matrix.set_submatrix(0,sym_ind*2*Nbss,soft_matrix_temp_1);
				soft_matrix.set_submatrix(0,(sym_ind*2+1)*Nbss,soft_matrix_temp_2);
			}
			else
				soft_matrix.set_submatrix(0,sym_ind*Nbss,soft_matrix_temp_1);
			if (is_rx_timing_on)
			{
				rxprocesses(7)+=rxtimer.toc()-last_time;
				last_time=rxtimer.toc();
			}
		}	
	} // end of per OFDM symbol processing
	
	// PROCESS  8: De-parser
	// cout << "Payload Process 8" << endl;
	{
		Spatial_Deparsing_11n css(rx_parameters.Nsd);
		css.combine(soft_matrix,single_stream_soft_bits,rx_parameters.mod_index_meta);
		if (is_rx_timing_on)
		{
			rxprocesses(8)+=rxtimer.toc()-last_time;
			last_time=rxtimer.toc();
		}
	}
	
	
	// PROCESS  9: Viterbi Decoder
	// cout << "Payload Process 9" << endl;
	{
		Soft_Bits_Decoding_11n vit(rx_parameters.code_rate_meta);
	 	decoded_output=vit.decode(0,single_stream_soft_bits); // currently hard coded to BCC	
		decoded_output.set_size(rx_parameters.LENGTH_meta*8+16,true);
		if (is_rx_timing_on)
		{
			rxprocesses(9)+=rxtimer.toc()-last_time;
			last_time=rxtimer.toc();
		}
	}
		
	// PROCESS 10: De-scrambler
	//cout << "Payload Process 10" << endl;
	{
		Descrambling_11n dsc;
		dsc.descramble(decoded_output);
		if (is_rx_timing_on)
		{
			rxprocesses(10)+=rxtimer.toc()-last_time;
			last_time=rxtimer.toc();
		}
	}

	// if the timing is being profiled, print times
	if (is_rx_timing_on)
	{
		cout << "------> Symbol Formatting time: " << rxprocesses(0) << endl;
		cout << "------> Combining time: " << rxprocesses(1) << endl;
		cout << "------> Equalization time: " << rxprocesses(2) << endl;
		cout << "------> Phase Correction time: " << rxprocesses(3) << endl;
		cout << "------> Remove Pilots/Null tones time: " << rxprocesses(4) << endl;
		cout << "------> STBC Decode time: " << rxprocesses(5) << endl;
		cout << "------> Demodulation time: " << rxprocesses(6) << endl;
		cout << "------> Deinterleave time: " << rxprocesses(7) << endl;
		cout << "------> Deparse time: " << rxprocesses(8) << endl;
		cout << "------> Decode time: " << rxprocesses(9) << endl;
		cout << "------> Descramble time: " << rxprocesses(10) << endl;
	}
}

unsigned int Data_Decoding_11n::recommend_mimo_mcs(double snr, double demmel, unsigned int psdu_len)
{
	
	unsigned int recommended_mcs=0;
	// determine if condition number is suitable for MIMO transmission
	if (demmel<20) // valid region of boundary equation
	{
		unsigned int rec_1_stream_mcs=recommend_siso_mcs(snr,psdu_len);
		unsigned int rec_2_stream_mcs=0;
		//unsigned int rec_3_stream_mcs=16;
		//unsigned int rec_4_stream_mcs=24;
		
		double stream_2_switching_value;
		if (demmel<1.8)
			stream_2_switching_value=6.0;
		else
		{
			// compute 2 stream boundary equation for switching
			stream_2_switching_value=14.1*exp(0.041*demmel)-18.1*exp(-0.38*demmel);
		}

		// set up constants based on psdu length
		unsigned int q_len=ceil_i(double(psdu_len)/100); // quantized length (100 Byte granularity)
		
		// determine multi-stream optimal MCSs
		switch(q_len)
		{
			case 1: // 100 Byte PSDU
			case 2: // 200 Byte PSDU
			case 3: // 300 Byte PSDU
			case 4: // 400 Byte PSDU
			case 5: // 500 Byte PSDU
			case 6: // 600 Byte PSDU
			case 7: // 700 Byte PSDU
			case 8: // 800 Byte PSDU
			case 9: // 900 Byte PSDU
			case 10: // 1000 Byte PSDU
				if (snr>(stream_2_switching_value+16.3)) {
					rec_2_stream_mcs=15;
				}
				else if (snr>(stream_2_switching_value+15.6)) {
					rec_2_stream_mcs=14;
				}
				else if (snr>(stream_2_switching_value+13.6)) {
					rec_2_stream_mcs=13;
				}
				else if (snr>(stream_2_switching_value+9.4)) {
					rec_2_stream_mcs=12;
				}
				else if (snr>(stream_2_switching_value+6.4)) {
					rec_2_stream_mcs=11;
				}
				else if (snr>(stream_2_switching_value+4.1)) {
					rec_2_stream_mcs=10;
				}
				else if (snr>(stream_2_switching_value+1.9)) {
					rec_2_stream_mcs=9;
				}
				else if (snr>(stream_2_switching_value)) {
					rec_2_stream_mcs=8;
				}
				else
					rec_2_stream_mcs=0; // 2 stream not recommended
				break;
			case 11: // 1100 Byte PSDU
			case 12: // 1200 Byte PSDU
			case 13: // 1300 Byte PSDU
			case 14: // 1400 Byte PSDU
			case 15: // 1500 Byte PSDU
			case 16: // 1600 Byte PSDU
			default: // largest packet size
				break;	
		}
		if (rec_2_stream_mcs==0)
			recommended_mcs=rec_1_stream_mcs;
		else
			recommended_mcs=rec_2_stream_mcs;
	}
	else
		recommended_mcs=recommend_siso_mcs(snr,psdu_len);
	
	return recommended_mcs;
}

unsigned int Data_Decoding_11n::recommend_siso_mcs(double snr, unsigned int psdu_len)
{	
	unsigned int recommended_mcs=0;
	unsigned int q_len=ceil_i(double(psdu_len)/100); // quantized length (100 Byte granularity)

	// currently using 10% PER values
	switch(q_len)
	{
		case 1: // 10% PER, BER=1.317e-4; 1% PER, BER=1.256e-5
			if (snr<8.0) {
				recommended_mcs=0;
			}
			else if (snr<9.7) {
				recommended_mcs=1;
			}
			else if (snr<11.6) {
				recommended_mcs=2;
			}
			else if (snr<14.9) {
				recommended_mcs=3;
			}
			else if (snr<18.8) {
				recommended_mcs=4;
			}
			else if (snr<20.3) {
				recommended_mcs=5;
			}
			else if (snr<21.7) {
				recommended_mcs=6;
			}
			else {
				recommended_mcs=7;
			}
 			break;
		case 2: // 10% PER, BER=6.584e-5; 1% PER, BER=6.281e-6
		case 3: // 10% PER, BER=4.390e-5; 1% PER, BER=4.188e-6
		case 4: // 10% PER, BER=3.292e-5; 1% PER, BER=3.141e-6
		case 5: // 10% PER, BER=2.634e-5; 1% PER, BER=2.513e-6
			if (snr<8.5) {
				recommended_mcs=0;
			}
			else if (snr<10.2) {
				recommended_mcs=1;
			}
			else if (snr<12.2) {
				recommended_mcs=2;
			}
			else if (snr<15.3) {
				recommended_mcs=3;
			}
			else if (snr<19.1) {
				recommended_mcs=4;
			}
			else if (snr<20.7) {
				recommended_mcs=5;
			}
			else if (snr<22.2) {
				recommended_mcs=6;
			}
			else {
				recommended_mcs=7;
			}
 			break;
		case 6: // 10% PER, BER=2.195e-5; 1% PER, BER=2.094e-6
		case 7: // 10% PER, BER=1.881e-5; 1% PER, BER=1.795e-6
		case 8: // 10% PER, BER=1.646e-5; 1% PER, BER=1.570e-6
		case 9: // 10% PER, BER=1.463e-5; 1% PER, BER=1.396e-6
		case 10: // 10% PER, BER=1.317e-5; 1% PER, BER=1.256e-6
			if (snr<8.0) {
				recommended_mcs=0;
			}
			else if (snr<10.5) {
				recommended_mcs=1;
			}
			else if (snr<12.8) {
				recommended_mcs=2;
			}
			else if (snr<15.6) {
				recommended_mcs=3;
			}
			else if (snr<19.0) {
				recommended_mcs=4;
			}
			else if (snr<21.0) {
				recommended_mcs=5;
			}
			else if (snr<22.5) {
				recommended_mcs=6;
			}
			else {
				recommended_mcs=7;
			}
 			break;
		case 11: // 10% PER, BER=1.197e-5; 1% PER, BER=1.142e-6
		case 12: // 10% PER, BER=1.097e-5; 1% PER, BER=1.047e-6
		case 13: // 10% PER, BER=1.013e-5; 1% PER, BER=9.663e-7
		case 14: // 10% PER, BER=9.407e-6; 1% PER, BER=8.973e-7
		case 15: // 10% PER, BER=8.780e-6; 1% PER, BER=8.375e-7
			if (snr<9.1) {
				recommended_mcs=0;
			}
			else if (snr<11.0) {
				recommended_mcs=1;
			}
			else if (snr<12.9) {
				recommended_mcs=2;
			}
			else if (snr<15.7) {
				recommended_mcs=3;
			}
			else if (snr<19.9) {
				recommended_mcs=4;
			}
			else if (snr<21.3) {
				recommended_mcs=5;
			}
			else if (snr<22.8) {
				recommended_mcs=6;
			}
			else {
				recommended_mcs=7;
			}
 			break;
		case 16: // 10% PER, BER=8.213e-6; 1% PER, BER=7.852e-7
		case 17: // 10% PER, BER=7.747e-6; 1% PER, BER=7.390e-7
		case 18: // 10% PER, BER=7.317e-6; 1% PER, BER=6.979e-7
		case 19: // 10% PER, BER=6.932e-6; 1% PER, BER=6.612e-7
		case 20: // 10% PER, BER=6.585e-6; 1% PER, BER=6.281e-7
			if (snr<9.3) {
				recommended_mcs=0;
			}
			else if (snr<10.8) {
				recommended_mcs=1;
			}
			else if (snr<13.0) {
				recommended_mcs=2;
			}
			else if (snr<15.8) {
				recommended_mcs=3;
			}
			else if (snr<20.0) {
				recommended_mcs=4;
			}
			else if (snr<21.5) {
				recommended_mcs=5;
			}
			else if (snr<22.9) {
				recommended_mcs=6;
			}
			else {
				recommended_mcs=7;
			}
 			break;
		default: // 1kB packet default
			if (snr<8.8) {
				recommended_mcs=0;
			}
			else if (snr<10.4) {
				recommended_mcs=1;
			}
			else if (snr<12.5) {
				recommended_mcs=2;
			}
			else if (snr<15.4) {
				recommended_mcs=3;
			}
			else if (snr<19.6) {
				recommended_mcs=4;
			}
			else if (snr<21.0) {
				recommended_mcs=5;
			}
			else if (snr<22.5) {
				recommended_mcs=6;
			}
			else {
				recommended_mcs=7;
			}
			break;	
	}
	return recommended_mcs;
}

mat Data_Decoding_11n::get_spatial_correlation()
{
	unsigned int Nrx=channel_matrices(0).rows();
	unsigned int Nss=channel_matrices(0).cols();
	unsigned int Nst=channel_matrices.size();
	cmat return_matrix(Nrx*Nss,Nrx*Nss);
	cmat return_matrix_temp(Nrx*Nss,Nrx*Nss);

	return_matrix.clear();
	cmat left_correlation_vector(Nrx*Nss,1);	
	cmat right_correlation_vector(1,Nrx*Nss);	

	for (unsigned int a=0; a<Nst; a++)
	{
		left_correlation_vector=reshape(channel_matrices(a),Nrx*Nss,1);
		right_correlation_vector=hermitian_transpose(left_correlation_vector);
		return_matrix_temp=left_correlation_vector*right_correlation_vector;
		for (unsigned int b=0; b<Nrx*Nss; b++)
		{
			for (unsigned int c=0; c<Nrx*Nss; c++)
			{
				return_matrix_temp(b,c)=return_matrix_temp(b,c)*(1/(abs(left_correlation_vector(b,0)*conj(right_correlation_vector(0,c)))));
			}
		}
		return_matrix+=return_matrix_temp;
	} 
	double const return_matrix_scale=1/double(Nst);
	return abs(return_matrix)*return_matrix_scale; 
}

mat Data_Decoding_11n::get_temporal_correlation()
{
	unsigned int Nrx=channel_matrices(0).rows();
	unsigned int Nss=channel_matrices(0).cols();
	unsigned int Nst=channel_matrices.size();
	cmat return_matrix(Nrx*Nss,Nrx*Nss);
	cmat return_matrix_temp(Nrx*Nss,Nrx*Nss);
	return_matrix.clear();
	cmat left_correlation_vector(Nrx*Nss,1);	
	cmat right_correlation_vector(1,Nrx*Nss);	
	{
		if (((past_channel(0).cols()!=Nss)||(past_channel(0).rows()!=Nrx))||(past_channel.size()!=Nst))
		{
			return zeros(Nrx*Nss,Nrx*Nss); // if dimensions change, no correlation
		}
		else
		{
			for (unsigned int a=0; a<Nst; a++)
			{		
				left_correlation_vector=reshape(channel_matrices(a),Nrx*Nss,1);
				right_correlation_vector=hermitian_transpose(reshape(past_channel(a),Nrx*Nss,1));
				return_matrix_temp=left_correlation_vector*right_correlation_vector;
				for (unsigned int b=0; b<Nrx*Nss; b++)
				{
					for (unsigned int c=0; c<Nrx*Nss; c++)
					{
						return_matrix_temp(b,c)=return_matrix_temp(b,c)*(1/(abs(left_correlation_vector(b,0)*conj(right_correlation_vector(0,c)))));
					}
				}
				return_matrix+=return_matrix_temp;
			}
			double const return_matrix_scale=1/double(Nst);
			return abs(return_matrix)*return_matrix_scale; 
		} 
	}
}

bvec Data_Decoding_11n::get_coded_bits()
{
	coded_bits.set_size(single_stream_soft_bits.length(),false);
	bvec onebit=ones_b(1); bvec zerobit=zeros_b(1);
	for (unsigned int coded_bits_index=0; coded_bits_index<single_stream_soft_bits.length(); coded_bits_index++)
	{
		if (single_stream_soft_bits(coded_bits_index)<0)
			coded_bits(coded_bits_index)=onebit(0);
		else
			coded_bits(coded_bits_index)=zerobit(0);
	}
	return coded_bits;
}

int Data_Decoding_11n::get_bit_errors(bool uncoded_bit_errors, rxvector const& rxv)
{
	{
		bvec fixed_input(rxv.LENGTH*8);
		BERC berc;
		
		// generate input bit using known algorithm
		// each byte corresponds to its index in binary
		int data_element;
		for (int index=0; index<rxv.LENGTH; index++)
		{
			data_element=mod(index,256);
			fixed_input.replace_mid(8*index,dec2bin(8,data_element));
		}
		// analyze bit errors of coded or uncoded bits
		if (uncoded_bit_errors)
		{	
			// assign coded bits from output
			get_coded_bits();
			// find the coded bits
			{	
				unsigned int extra_bits=rxv.Nsym*rxv.Ndbps-8*rxv.LENGTH-16;
				Descrambling_11n dsc;
				Soft_Bits_Decoding_11n vit(rxv.code_rate);
				// format binary input
				fixed_input=concat(zeros_b(16),fixed_input,zeros_b(extra_bits));
				// scramble bit sequence
				dsc.scramble(fixed_input,zeros_b(7));
				fixed_input.replace_mid(length(fixed_input)-6,zeros_b(6)); // return tail to zero state
				// encode the bit sequence
				vit.encode(0,fixed_input);//,length(coded_bits)); // code the bistream using MCS
			}
			berc.clear();
			berc.count(fixed_input,coded_bits);
			return int(berc.get_errors());
		}
		else // coded BER
		{
			// just compare recovered output to fixed input
			berc.clear();
			berc.count(fixed_input,recovered_bits);
			return int(berc.get_errors());
		}
	}

}

cmat Data_Decoding_11n::get_ofdm_symbols(int n1, int n2)
{
	if (n1<0)
		n1=0;
	if (n2>=ofdm_symbol_history.cols()/Nsd)
		n2=ofdm_symbol_history.cols()/Nsd-1;
	if (n1>=ofdm_symbol_history.cols()/Nsd)
		return zeros_c(ofdm_symbol_history.rows(),1);
	else
		return ofdm_symbol_history.get_cols(n1*Nsd,(n2+1)*Nsd-1);
}

cmat Data_Decoding_11n::get_meta_ofdm_symbols(int n1, int n2)
{
	if (n1<0)
		n1=0;
	if (n2>=meta_ofdm_symbol_history.cols()/Nsd)
		n2=meta_ofdm_symbol_history.cols()/Nsd-1;
	if (n1>=meta_ofdm_symbol_history.cols()/Nsd)
		return zeros_c(meta_ofdm_symbol_history.rows(),1);
	else
		return meta_ofdm_symbol_history.get_cols(n1*Nsd,(n2+1)*Nsd-1);
}


vec Data_Decoding_11n::get_condition_number(bool is_Demmel)
{
	unsigned int Nst=extension_channel_matrices.size();
	vec singular_values;
	vec return_vector(Nst);
  	if (Nst<1) { return_vector.set_size(1); return_vector[0] = 1.0; return return_vector; }
	unsigned int mat_rank=std::min(extension_channel_matrices(0).rows(),extension_channel_matrices(0).cols());
	
	if (is_Demmel) // Demmel condition number
	{
		double temp=0;
		for (unsigned int a=0; a<Nst; a++)
		{
			temp=0;
			singular_values=svd(extension_channel_matrices(a));
			if ((singular_values(0)/singular_values(mat_rank-1))>1000)
				return_vector(a)=1000;
			else
			{
				for (unsigned int b=0; b<mat_rank; b++)
					temp+=(1/pow(singular_values(b),2));
				
				return_vector(a)=singular_values(0)*std::sqrt(temp);
			}	
		}
	}
	else // standard condition number
	{
		for (unsigned int a=0; a<Nst; a++)
		{
			singular_values=svd(extension_channel_matrices(a));
			return_vector(a)=singular_values(0)/singular_values(mat_rank-1);
			if (return_vector(a)>1000)
				return_vector(a)=1000;
		}
	}
	return return_vector;
}

double Data_Decoding_11n::get_avg_condition_number(bool is_Demmel)
{	
	vec cond_vec=get_condition_number(is_Demmel);
	return sum(cond_vec)/length(cond_vec);
}
	
