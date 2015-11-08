/*  	
 * Title: Decoupled Equalization (DEQ) and FFT algorithm
 * Created By: Robert Daniels
 * Creation Date: 
 * 
 * Description: Use non-optimal equalization to decouple demodulation and decoding; DEQ includes demultiplexing and combining algorithms
 *
 * Revision: v0.00 - - Initial Release (only supports Greenfield operation under 20 MHz channels)
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

#include "spatial_equalization_11n.h" // decoupled equalization

using namespace itpp;
using std::cout;
using std::endl;

Spatial_Equalization_11n::Spatial_Equalization_11n(unsigned int num_space_time_streams, unsigned int num_total_subcarriers)
{
	Nsts=num_space_time_streams;
	Nrx=Nsts;
	equalization_mode=0;
	noise_variance=0.1; // initial value
	Nst=num_total_subcarriers;

	switch (Nst)
	{
		case 52:
			subcarrier_index="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
			Nfft=64;
			break;
		case 56:
			subcarrier_index="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
			Nfft=64;
			break;
		case 104:
			subcarrier_index="6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122";
			Nfft=128;
			break;
		case 114:
			subcarrier_index="2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126";
			Nfft=128;
			break;
	}
	normalization_factor=1/sqrt(Nst);
	H_tmp.set_size(Nsts,4);
	H_tmp_H.set_size(Nsts,4);
	H_I.set_size(4,4);
	x_mat.set_size(Nsts,1);
	y_mat.set_size(4,1);
	G.set_size(4,4);
	fft_vector.set_size(Nfft);
	deq_index=0; 
	N=(noise_variance)*eye_c(Nsts);
}

void Spatial_Equalization_11n::set_equalization_mode(unsigned int eq_mode)
{
	equalization_mode=eq_mode;
}

void Spatial_Equalization_11n::set_noise_variance(double noisevar)
{
	noise_variance=noisevar;
	N=(noise_variance)*eye_c(Nsts);
}

void Spatial_Equalization_11n::equalize(cmat& modulated_symbol, cmat& modulated_freq_symbol, Array<cmat> const& channels)
{ // assume input is an OFDM symbol including pilots with Nrx rows for each receive
  // antenna;  each matrix in array 'channels' holds the channel for each subcarrier;
  // as such, 'channels' holds 'total_subcarriers' elements of dimension (Nrx X Nsts); 
  // take the fft
	
	deq_index=0;
	Nrx=modulated_symbol.rows();
	modulated_freq_symbol.set_size(Nrx,Nfft);
	for (; deq_index<Nrx; deq_index++)
	{
		fft_vector=modulated_symbol.get_row(deq_index);
		fft_vector=normalization_factor*fft(fft_vector,Nfft);
		modulated_freq_symbol.set_row(deq_index,fft_vector);
	}
	// perform equalization
	switch (equalization_mode)
	{
		case 0:
		case 1:
			// equalize 'fde_symbol'
			modulated_symbol=modulated_freq_symbol;
			fde(channels,modulated_symbol); 
			// remove excess data due to excess receive streams
			if (Nrx>Nsts)
			{
				modulated_symbol.set_size(Nsts,modulated_symbol.cols(),true);
			}
			break;
		case 2:
			cout << "Error: Time domain equalization not yet supported!" << endl;
			break;
		case 3:
			break;
		case 4: 
		case 5:
		default:
			// default to fde for visualization purposes only
			modulated_symbol=modulated_freq_symbol;
			fde(channels,modulated_symbol); 
			// remove excess data due to excess receive streams
			if (Nrx>Nsts)
			{
				modulated_symbol.set_size(Nsts,modulated_symbol.cols(),true);
			}
	}
}

void Spatial_Equalization_11n::compute_post_process_snr(Array<cmat> const& channels, mat& post_process_snr)
{ 
	// assume input is an OFDM symbol including pilots with Nrx rows for each receive
  // antenna;  each matrix in array 'channels' holds the channel for each subcarrier;
  // as such, 'channels' holds 'total_subcarriers' elements of dimension (Nrx X Nsts); 
  // take the fft
	int eq_col=0;
	if (Nsts==1)
	{		
		cvec h_rx(Nrx);
		for (eq_col=0; eq_col<Nst; eq_col++)
		{
			// define the MRC solution
			H_tmp=channels(eq_col);
			h_rx=sum(H_tmp,2);
	
			// post-processing SNR is just sum-squared of one-stream rx channels
			post_process_snr(0,eq_col)=sum_sqr(abs(h_rx));
		}
	}
	else
	{
		switch (equalization_mode)
		{
			case 0: // zero forcing
				for (eq_col=0; eq_col<Nst; eq_col++)
				{
					// current channel matrix
					H_tmp=channels(eq_col);
					H_tmp_H=hermitian_transpose(H_tmp);
					
					// post-processing SNR (see MIMO text by Heath et al)
					post_process_snr.set_col(eq_col,real(diag(inv(H_tmp_H*H_tmp))));
				}
				// invert each pp-SNR element
				post_process_snr=elem_div(ones(Nsts,Nst),post_process_snr);
				break;
			case 1:	// minimum mean square error
				// assume channel model y=Hx+n where y is Nrx X 1, x is Nsts X 1
				for (eq_col=0; eq_col<Nst; eq_col++)
				{
					// define the MMSE equalizer matrix
					H_tmp=channels(eq_col);
					H_tmp_H=hermitian_transpose(H_tmp);
					H_I=H_tmp_H*H_tmp+N;
					H_I=inv(H_I);

					// post-processing SNR (see MIMO text by Heath et al)
					post_process_snr.set_col(eq_col,real(diag(H_I)));
				}	
				// invert each pp-SNR element
				post_process_snr=elem_div(ones(Nsts,Nst),post_process_snr);
				break;		
			case 2:
				cout << "Error: Time domain equalization not yet supported!" << endl;
				break;
			case 3:
				break;
			case 4: 
			case 5:
			default:  // use zero-forcing by default	
				for (eq_col=0; eq_col<Nst; eq_col++)
				{
					// current channel matrix 
					H_tmp=channels(eq_col);
					H_tmp_H=hermitian_transpose(H_tmp);

					// post-processing SNR (see MIMO text by Heath et al)
					post_process_snr.set_col(eq_col,real(diag(inv(H_tmp_H*H_tmp))));
				}
				// invert each pp-SNR element
				post_process_snr=elem_div(ones(Nsts,Nst),post_process_snr);		
		}
	}
}

void Spatial_Equalization_11n::fde(Array<cmat> const& H, cmat& eq_data)
{	
	unsigned int eq_col=0;
	cmat comb(1,Nrx);
	if (Nsts==1)
	{	
		cvec h_rx(Nrx);
		for (; eq_col<Nst; eq_col++)
		{
			// output vector
			y_mat=eq_data.get(0,Nrx-1,subcarrier_index(eq_col),subcarrier_index(eq_col));
			
			// define the MRC solution
			H_tmp=H(eq_col);
			h_rx=sum(H_tmp,2);
			comb.set_row(0,conj(h_rx));
			x_mat=comb*y_mat/sum_sqr(abs(h_rx));

			// set eq_data column to equalized vector
			eq_data.set_submatrix(0,subcarrier_index(eq_col),x_mat);
		}
	}
	else
	{
		switch (equalization_mode)
		{
			case 0: // zero forcing
				for (; eq_col<Nst; eq_col++)
				{
					// current channel matrix
					H_tmp=H(eq_col);
					
					// output vector
					y_mat=eq_data.get(0,Nrx-1,subcarrier_index(eq_col),subcarrier_index(eq_col));
					
					// define the ZF solution
					x_mat=ls_solve_od(H_tmp,y_mat);
					
					// set eq_data column to equalized vector
					eq_data.set_submatrix(0,subcarrier_index(eq_col),x_mat);
				}
				break;
			case 1:	// minimum mean square error
				// assume channel model y=Hx+n where y is Nrx X 1, x is Nsts X 1
				for (; eq_col<Nst; eq_col++)
				{
					// define the MMSE equalizer matrix
					H_tmp=H(eq_col);
					H_tmp_H=hermitian_transpose(H_tmp);
					H_I=H_tmp_H*H_tmp+N;
					H_I=inv(H_I);
					G=H_I*H_tmp_H;
				
					// define output vector
					y_mat=eq_data.get(0,Nrx-1,subcarrier_index(eq_col),subcarrier_index(eq_col));
					
					// define the MMSE solution
					x_mat=G*y_mat; // data estimate
				
					// set eq_data column to equalized vector
					eq_data.set_submatrix(0,subcarrier_index(eq_col),x_mat);
				}	
				break;	
			case 4:
			case 5:	
			default:  // use zero-forcing by default	
				for (; eq_col<Nst; eq_col++)
				{
					// current channel matrix
					H_tmp=H(eq_col);
					
					// output vector
					y_mat=eq_data.get(0,Nrx-1,subcarrier_index(eq_col),subcarrier_index(eq_col));
					
					// define the ZF solution
					x_mat=ls_solve_od(H(eq_col),y_mat);
					
					// set eq_data column to equalized vector
					eq_data.set_submatrix(0,subcarrier_index(eq_col),x_mat);
				}
		}
	}
}
