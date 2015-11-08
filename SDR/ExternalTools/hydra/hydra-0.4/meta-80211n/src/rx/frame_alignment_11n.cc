/*  	
 * Title:  Exact Packet Detection (EPD)
 * Created By: Robert Daniels
 * Creation Date: 
 * 
 * Description: Initial symbol timing for 802.11n preambles
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

#include "frame_alignment_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

#define ALIGN_WITH_MAX    1

Frame_Alignment_11n::Frame_Alignment_11n(unsigned int win_size)
{
	D=16;	// periodicity of short training (20 MHz = 16, 40 MHz = 32)
	L=win_size; // number of samples to average over
	decision_metric=0;
}

unsigned int Frame_Alignment_11n::schmidl_cox_egc(cmat in_data, unsigned int
anticipation_symbols)
{
// received stream symbol timing
// based on "ROBUST TIMING AND FREQUENCY ESTIMATION for OFDM" by Schmidl & Cox
// assume in_data starts in stfs and contains at least 3 stfs
	
	unsigned int data_length=in_data.cols();
	unsigned int Nrx=in_data.rows();
	cvec ratio_num(Nrx);ratio_num.clear();
	vec ratio_den(Nrx);ratio_den.clear();
	unsigned int row_index=0;
	double correlation_floor=0;
#if ALIGN_WITH_MAX
  unsigned int max_antenna=0;
  vec corr_mat; double max_corr=0;
	
	for (; row_index<Nrx; row_index++)
	{
		ratio_num(row_index)=sum(sum(elem_mult(in_data.get(row_index,row_index,0,L-1),conj(in_data.get(row_index,row_index,D,D+L-1)))));
		ratio_den(row_index)=pow(norm(in_data.get(row_index,row_index,D,D+L-1),"fro"),2);
	}
  corr_mat=elem_div(abs(ratio_num),ratio_den);
  for (unsigned int k=0; k<corr_mat.size(); k++)
    if (max_corr<corr_mat(k)) {
      max_corr = corr_mat(k);
      max_antenna = k;
    }
	decision_metric=max_corr;     // eq (8) in Schmidl & Cox
#else
	for (; row_index<Nrx; row_index++)
	{
		ratio_num(row_index)=sum(sum(elem_mult(in_data.get(row_index,row_index,0,2*D-1),conj(in_data.get(row_index,row_index,D,3*D-1)))));
		ratio_den(row_index)=pow(norm(in_data.get(row_index,row_index,D,3*D-1),"fro"),2);
	}
	decision_metric=sum(elem_div(abs(ratio_num),ratio_den))/Nrx; // eq (8) in Schmidl & Cox
#endif

	// calculate correlation floor
#if ALIGN_WITH_MAX
	row_index=max_antenna;
  correlation_floor=abs(sum(sum(elem_mult(in_data.get(row_index,row_index,data_length-D-L,data_length-D-1),conj(in_data.get(row_index,row_index,data_length-L,data_length-1)))))/pow(norm(in_data.get(row_index,row_index,data_length-L,data_length-1),"fro"),2));
#else
	row_index=0;
	for (; row_index<Nrx; row_index++)
	{
		correlation_floor+=abs(sum(sum(elem_mult(in_data.get(row_index,row_index,data_length-5*(D/2),data_length-(D/2)-1),conj(in_data.get(row_index,row_index,data_length-2*D,data_length-1)))))/pow(norm(in_data.get(row_index,row_index,data_length-5*(D/2),data_length-(D/2)-1),"fro"),2));
	} 
	correlation_floor*=(1/double(Nrx));
#endif
	
	Th=decision_metric-((L-L/2)/double(L))*(decision_metric-correlation_floor);
	// detect D symbols of decorrelation
  if (0) {
    fprintf(stderr, "[frame_alignment_11n] correlation ceil  = %.3f\n", decision_metric);
    fprintf(stderr, "[frame_alignment_11n] correlation floor = %.3f\n", correlation_floor);
    fprintf(stderr, "[frame_alignment_11n] correlation Th    = %.3f\n", Th);
#if ALIGN_WITH_MAX
    fprintf(stderr, "[frame_alignment_11n] max antenna       = %d\n", max_antenna);
#endif
  }

	row_index=0;
	for (; row_index<Nrx; row_index++)
	{
		ratio_num(row_index)=sum(sum(elem_mult(in_data.get(row_index,row_index,0,L-1),conj(in_data.get(row_index,row_index,D,L+D-1)))));
		ratio_den(row_index)=pow(norm(in_data.get(row_index,row_index,D,L+D-1),"fro"),2);
	}
#if ALIGN_WITH_MAX
  corr_mat=elem_div(abs(ratio_num),ratio_den);
	decision_metric=corr_mat(max_antenna);
#else
	decision_metric=sum(elem_div(abs(ratio_num),ratio_den))/Nrx;
#endif
	unsigned int a=1; // need index outside of loop
	
	for (;((a+L+D<data_length)&&(decision_metric>Th));a++)
	{
		row_index=0;
		for (; row_index<Nrx; row_index++)
		{
			ratio_num(row_index)+=in_data(row_index,L+a-1)*conj(in_data(row_index,L+D+a-1));
			ratio_num(row_index)-=in_data(row_index,a-1)*conj(in_data(row_index,D+a-1));
			ratio_den(row_index)+=std::real(in_data(row_index,L+D+a-1)*conj(in_data(row_index,L+D+a-1)));
			ratio_den(row_index)-=std::real(in_data(row_index,D+a-1)*conj(in_data(row_index,D+a-1)));
		}
#if ALIGN_WITH_MAX
    corr_mat=elem_div(abs(ratio_num),ratio_den);
    decision_metric=corr_mat(max_antenna);
#else
		decision_metric=sum(elem_div(abs(ratio_num),ratio_den))/Nrx;
#endif
	}
	
	if ((decision_metric>Th))
	{
		return 0;
	}
	else
	{
		return a+L+D-(L-L/2)-anticipation_symbols-3; 
		// return estimated symbol (based on D symbol threshold)
		// '-3' is a fudge factor simulated in high SNR
	}
}

unsigned int Frame_Alignment_11n::schmidl_cox_mrc(cmat in_data, cvec rx_ratios, unsigned int anticipation_symbols)
{
	return 0;
}

unsigned int Frame_Alignment_11n::zhou_saito_egc(cmat in_data)
{
// based on "A NEW SYMBOL TIMING SYNCHRONIZATION FOR OFDM BASED 
// WLANS UNDER MULTIPATH FADING CHANNELS" by Zhou & Saito

// equal gain combining packet detection
// returns symbol index for packet start
// in_data = input data symbols

	
}

unsigned int Frame_Alignment_11n::zhou_saito_mrc(cmat in_data, cvec rx_ratios)
{
// based on "A NEW SYMBOL TIMING SYNCHRONIZATION FOR OFDM BASED 
// WLANS UNDER MULTIPATH FADING CHANNELS" by Zhou & Saito

// maximal gain combining packet detection
// returns symbol index for packet start
// in_data = input data symbols

	
}

