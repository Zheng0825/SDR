/*  	
 * Title:  Spatial Channel Estimation (SCE)
 * Created By: Robert Daniels
 * Creation Date: 
 * 
 * Description: Spatial channel and SNR estimation of 802.11n preambles
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

#include "channel_estimation_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Channel_Estimation_11n::Channel_Estimation_11n(rxvector const& rx_param)
{
	PHTLTF="1 0 1 1;1 1 0 1;1 1 1 0;0 1 1 1";
	PHTLTF-="0 1 0 0;0 0 1 0;0 0 0 1;1 0 0 0";
	Nsts=rx_param.Nsts;
	Ness=rx_param.Ness;
	Nsts_meta=rx_param.Nsts_meta;
	train="1 0 0 1 1 0 1 0 1 0 0 0 0 0 1 1 0 0 1 0 1 0 1 1 1 1 0 0 1 1 1 1 0 0 1 1 0 1 0 1 1 1 1 1 1 0 0 1 1 0 1 0 1 1 1 1";
	train-="0 1 1 0 0 1 0 1 0 1 1 1 1 1 0 0 1 1 0 1 0 1 0 0 0 0 1 1 0 0 0 0 1 1 0 0 1 0 1 0 0 0 0 0 0 1 1 0 0 1 0 1 0 0 0 0";
	Nst=56;
	Nfft=64;
	Ngi=16;
	subcarrier_indices="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";	
	Nofdm=Ngi+Nfft;
	average_snr=0;
	noise_variance=0;
}

cmat Channel_Estimation_11n::egc_ls_fde(cmat channel_block)
{
// equal gain combining and least squares frequency domain estimation


	unsigned int Nrx=channel_block.rows();
	cvec channel_sample(channel_block.cols());

	// estimate the noise variance
	noise_variance=0;
	for (unsigned int a=0; a<Nrx; a++)
	{
		channel_sample=channel_block.get_row(a);
		noise_variance+=variance(channel_sample(0,Nfft-1)-channel_sample(Nfft,2*Nfft-1)); // find noise variance of received streams
	}
	noise_variance/=(2*Nrx); // avg No per rx chain
	
	// estimate temporally averaged channel elements	
	channel_block.set_submatrix(0,0,0.5*(channel_block.get_cols(0,Nfft-1)+channel_block.get_cols(Nfft,2*Nfft-1)));
	channel_block.set_size(Nrx,Nfft,true);
	for (unsigned int a=0; a<Nrx; a++)
	{
		channel_sample=fft(channel_block.get_row(a),Nfft);
		for (unsigned int b=0; b<Nst; b++)
			channel_block(a,subcarrier_indices(b))=channel_sample(subcarrier_indices(b))*train(b);
	}
	channel_block*=1/sqrt(Nst);
	subcarrier_energy=(sqr(abs(channel_block)))/(noise_variance); // find snr over each subcarrier from channel estimate
	average_snr=sum(sum(subcarrier_energy))/(Nrx*train.length());
	return channel_block;	
}

void Channel_Estimation_11n::estimate(cmat received_stream, unsigned int est_mode, Array<cmat>& returned_array)
{
// performs estimation of spatial channel element from training symbols;
// 'received_stream' holds the input containing all channel data
// for greenfield, first LTF may be double length
	
	// determine number of space-time streams for decoding
	unsigned int Nltf=received_stream.cols()/(Nofdm);
	unsigned int Nrx=received_stream.rows();
	

	switch (est_mode)
	{
		case 0: // LS-FDE
		{	
			// take FFTs
			cvec fft_vector;
			for (unsigned int a=0; a<Nrx; a++)
			{
				fft_vector=received_stream.get_row(a);
				for (unsigned int b=0; b<Nltf; b++)
				{
					fft_vector.replace_mid(b*Nfft,(1/sqrt(Nst))*fft(fft_vector(b*Nofdm+Ngi,(b+1)*Nofdm-1),Nfft));
				}
				received_stream.set_row(a,fft_vector);
			}
			// remove unwanted zero tones
			for (int c=0; c<Nltf; c++)
			{
				for (unsigned int d=0; d<Nst; d++)
				{
					received_stream.copy_col(c*Nst+d,c*Nfft+subcarrier_indices(d));
				}
			}	
			
			cmat fde_matrix(Nrx,Nltf);
			cmat combining_matrix=(1/double(Nltf))*transpose(PHTLTF.get(0,Nsts-1,0,Nltf-1));
			// perform estimation by demultiplexing subcarrier training
			// perform least-square estimation and demultiplex for each transmit-receive pair
			subcarrier_energy=zeros(Nrx,Nsts);
			for (unsigned int e=0; e<Nst;e++)
			{
				for (unsigned int f=0; f<Nltf; f++)
					fde_matrix.set_col(f,received_stream.get_col(f*Nst+e)*train(e));
				returned_array(e)=(fde_matrix*combining_matrix);
				subcarrier_energy+=real(elem_mult(returned_array(e),conj(returned_array(e))));
			}
			subcarrier_energy=subcarrier_energy*(1/double(Nfft));
		}	
			break;
		case 1: // LS-TDE
			break;
	}
}

void Channel_Estimation_11n::extension_estimate(cmat received_stream, unsigned int est_mode, Array<cmat>& returned_array)
{
// performs estimation of spatial channel element from training symbols;
// 'received_stream' holds the input containing all channel data
// for greenfield, first LTF may be double length
	
	// determine number of space-time streams for decoding
	unsigned int Neltf=received_stream.cols()/(Nofdm);
	unsigned int Nrx=received_stream.rows();
		
	switch (est_mode)
	{
		case 0: // LS-FDE
		{	
			// take FFTs

			cvec fft_vector;
			for (unsigned int a=0; a<Nrx; a++)
			{
				fft_vector=received_stream.get_row(a);
				for (unsigned int b=0; b<Neltf; b++)
				{
					fft_vector.replace_mid(b*Nfft,(1/sqrt(Nst))*fft(fft_vector(b*Nofdm+Ngi,(b+1)*Nofdm-1),Nfft));
				}
				received_stream.set_row(a,fft_vector);
			}
	
			// remove unwanted zero tones
			for (int c=0; c<Neltf; c++)
			{
				for (unsigned int d=0; d<Nst; d++)
				{
					received_stream.copy_col(c*Nst+d,c*Nfft+subcarrier_indices(d));
				}
			}	
			
			cmat fde_matrix(Nrx,Neltf);
			subcarrier_energy=zeros(Nrx,Neltf);
			// perform estimation by demultiplexing subcarrier training
			// perform least-square estimation and demultiplex for each transmit-receive pair
			vec signal_centroid(Nrx*Ness);
			mat signal_variance(Nrx*Ness,Nst);
			for (unsigned int e=0; e<Nst; e++)
			{
				for (unsigned int f=0; f<Neltf; f++)
					fde_matrix.set_col(f,received_stream.get_col(f*Nst+e)*train(e));
				returned_array(e)=(fde_matrix);
				subcarrier_energy+=real(elem_mult(returned_array(e),conj(returned_array(e))));
			}
			subcarrier_energy=subcarrier_energy*(1/double(Nfft));
		}	
		break;
	case 1: // LS-TDE
		break;
		
		
	}
}

void Channel_Estimation_11n::meta_channel_conversion(Array<cmat> const& channel_estimate, Array<cmat>& meta_estimate)
{
	unsigned int Nrx=channel_estimate(0).rows();
	meta_estimate.set_size(Nst);
	cmat cmat_template=zeros_c(Nrx,Nsts_meta);
	unsigned int index=0;
	unsigned int index2=0;
	// meta mapping depends on data mapping for proper channel estimation
	switch(10*Nsts+Nsts_meta)
	{
		case 11:
		case 22:
		case 33:
		case 44:
			meta_estimate=channel_estimate;
			break;
		case 21:
			for (;index<Nst;index++)
			{
				index2=0;
				for (; index2<Nrx; index2++)
				{
					cmat_template(index2,0)=sqrt(2)*channel_estimate(index)(index2,0)+channel_estimate(index)(index2,1);
				}
				meta_estimate(index)=cmat_template;
			}
			break;
		case 31:
			for (;index<Nst;index++)
			{
				index2=0;
				for (; index2<Nrx; index2++)
				{
					cmat_template(index2,0)=sqrt(3)*channel_estimate(index)(index2,0)+channel_estimate(index)(index2,1)+channel_estimate(index)(index2,2);
				}
				meta_estimate(index)=cmat_template;
			}
			break;
		case 32:
			for (;index<Nst;index++)
			{
				cmat_template.set_col(0,channel_estimate(index).get_col(0));
				index2=1;
				for (; index2<Nrx; index2++)
				{
					cmat_template(index2,1)=sqrt(2)*channel_estimate(index)(index2,1)+channel_estimate(index)(index2,2);
				}
				meta_estimate(index)=cmat_template;
			}
			break;
		case 41:
			for (;index<Nst;index++)
			{
				index2=0;
				for (; index2<Nrx; index2++)
				{
					cmat_template(index2,0)=2*channel_estimate(index)(index2,0)+channel_estimate(index)(index2,1)+channel_estimate(index)(index2,2)+channel_estimate(index)(index2,3);
				}
				meta_estimate(index)=cmat_template;
			}
			break;
		case 42:
			for (;index<Nst;index++)
			{
				index2=0;
				for (; index2<Nrx; index2++)
				{
					cmat_template(index2,0)=sqrt(2)*channel_estimate(index)(index2,0)+channel_estimate(index)(index2,1);
					cmat_template(index2,1)=sqrt(2)*channel_estimate(index)(index2,2)+channel_estimate(index)(index2,3);
				}
				meta_estimate(index)=cmat_template;
			}
			break;
		case 43:
			for (;index<Nst;index++)
			{
				cmat_template.set_col(0,channel_estimate(index).get_col(0));
				cmat_template.set_col(1,channel_estimate(index).get_col(1));
				index2=1;
				for (; index2<Nrx; index2++)
				{
					cmat_template(index2,2)=sqrt(2)*channel_estimate(index)(index2,2)+channel_estimate(index)(index2,3);
				}
				meta_estimate(index)=cmat_template;
			}
			break;
	}
}

void Channel_Estimation_11n::smooth_channel_estimate(unsigned int smoothing_index, Array<cmat>& channel_estimate)
{
	unsigned int Nst=channel_estimate.size();
	// need a channel copy for reference
	Array<cmat> temporary_channel=channel_estimate;	
	std::complex<double> iorj(0,1);
	std::complex<double> unit(1,0);
	// smoothing can only be performed on polar coordinates
	Array<mat> channel_phase(Nst); 
	Array<mat> channel_magnitude(Nst);
	for (unsigned int index=0; index<Nst; index++)
	{
		channel_phase(index)=arg(channel_estimate(index));
		channel_magnitude(index)=abs(channel_estimate(index));
	}
	unwrap_channel_phase(channel_phase);
	// need a phase copy for reference
	Array<mat> temporary_phase=channel_phase;
	
	switch (smoothing_index)
	{
		case 0: // 1st neighbors subcarrier smoothing
			{
				// positive subcarriers
				//channel_phase(0)+=temporary_phase(1);
				//channel_phase(0)*=0.5;
				channel_estimate(0)=elem_mult(unit*0.5*(channel_magnitude(0)+channel_magnitude(1)),exp(iorj*channel_phase(0)));
				for (unsigned int a=1; a<(Nst/2-1); a++)
				{
					channel_phase(a)+=temporary_phase(a-1)+temporary_phase(a+1);
					channel_phase(a)*=0.333333;
					channel_estimate(a)=elem_mult(unit*0.333333*(channel_magnitude(a-1)+channel_magnitude(a)+channel_magnitude(a+1)),exp(iorj*channel_phase(a)));
				}

				//channel_phase(Nst/2-1)+=temporary_phase(Nst/2-2);
				//channel_phase(Nst/2-1)*=0.5;
				channel_estimate(Nst/2-1)=elem_mult(unit*0.5*(channel_magnitude(Nst/2-2)+channel_magnitude(Nst/2-1)),exp(iorj*channel_phase(Nst/2-1)));
				
				// negative subcarriers
				//channel_phase(Nst/2)+=temporary_phase(Nst/2+1);
				//channel_phase(Nst/2)*=0.5;
				channel_estimate(Nst/2)=elem_mult(unit*0.5*(channel_magnitude(Nst/2)+channel_magnitude(Nst/2+1)),exp(iorj*channel_phase(Nst/2)));

				for (unsigned int b=(Nst/2+1); b<(Nst-1); b++)
				{
					channel_phase(b)+=temporary_phase(b-1)+temporary_phase(b+1);
					channel_phase(b)*=0.333333;
					channel_estimate(b)=elem_mult(unit*0.333333*(channel_magnitude(b-1)+channel_magnitude(b)+channel_magnitude(b+1)),exp(iorj*channel_phase(b)));
				}

				//channel_phase(Nst-1)+=temporary_phase(Nst-2);
				//channel_phase(Nst-1)*=0.5;
				channel_estimate(Nst-1)=elem_mult(unit*0.5*(channel_magnitude(Nst-2)+channel_magnitude(Nst-1)),exp(iorj*channel_phase(Nst-1)));

			}
			break;
	}
}

void Channel_Estimation_11n::unwrap_channel_phase(Array<mat>& channel_phase)
{
	unsigned int Nst=channel_phase.size();
	double cumulative_sum=0;
	double delta=0;
	unsigned int Rows=channel_phase(0).rows();
	unsigned int Cols=channel_phase(0).cols();
	
	for (unsigned int rw=0; rw<Rows; rw++)
	{
		for (unsigned int cl=0; cl<Cols; cl++)
		{	
			cumulative_sum=0;
			delta=0;
			for (unsigned int idx=1; idx<Nst/2; idx++)
			{
				delta=channel_phase(idx)(rw,cl)+cumulative_sum-channel_phase(idx-1)(rw,cl);
				if (std::abs(delta) > pi) // phase needs to be wrapped
				{
					if (delta>0)
						cumulative_sum-=2*pi;
					else
						cumulative_sum+=2*pi;
				}
				channel_phase(idx)(rw,cl)+=cumulative_sum;
			}
			cumulative_sum=0;
			delta=0;
			for (unsigned int idx=(Nst/2+1); idx<Nst; idx++)
			{
				delta=channel_phase(idx)(rw,cl)+cumulative_sum-channel_phase(idx-1)(rw,cl);
				if (std::abs(delta) > pi) // phase needs to be wrapped
				{
					if (delta>0)
						cumulative_sum-=2*pi;
					else
						cumulative_sum+=2*pi;
				}
				channel_phase(idx)(rw,cl)+=cumulative_sum;
			}

		}
	}
	

}
