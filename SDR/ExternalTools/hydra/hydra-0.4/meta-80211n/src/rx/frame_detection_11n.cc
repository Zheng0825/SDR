/*  	
 * Title:  Coarse Packet Detection (CPD)
 * Created By: Robert Daniels
 * Creation Date: 
 * 
 * Description: Packet Detection for 802.11n preambles
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

#include "frame_detection_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

#define CHECK_DETECTION_FLOOR   0
#define USE_MAX_ENERGY_ANTENNA  1
#define FRAME_DETECTION_FLOOR   0.40

Frame_Detection_11n::Frame_Detection_11n(unsigned int win_size, double threshold)
{
	L=win_size; // size of window to capture (less than 7*D) 
	D=16;	// periodicity of short training (20 MHz = 16, 40 MHz = 32)
	Th=threshold; // for estimating packet start
	correlation_ratio=0; // initialize decision statistic
}

int Frame_Detection_11n::egc_frame_detect(cmat in_data)
{
// equal gain combining packet detection
// returns symbol index for packet start
// in_data = input data symbols
	
	unsigned int Nrx=in_data.rows();	
	unsigned int dat_length=in_data.cols();
 	cvec ratio_num(Nrx);ratio_num.clear();
	vec ratio_den(Nrx);ratio_den.clear();
	unsigned int row_index=0;

#if USE_MAX_ENERGY_ANTENNA
  double max_energy=0; unsigned int max_antenna=0;
#endif
	for (; row_index<Nrx; row_index++)
	{
		ratio_num(row_index)=ratio_num(row_index)+sum(sum(elem_mult(in_data.get(row_index,row_index,0,L-1),conj(in_data.get(row_index,row_index,D,L+D-1)))));
		ratio_den(row_index)=ratio_den(row_index)+sqr(norm(in_data.get(row_index,row_index,D,L+D-1),2));
#if USE_MAX_ENERGY_ANTENNA
    if (max_energy<ratio_den(row_index)) {max_energy=ratio_den(row_index); max_antenna=row_index;}
	}
  double correlation_ratio=abs(ratio_num(max_antenna)/ratio_den(max_antenna));
  max_energy=0; max_antenna=0;  // reset max-energy parameters
#else
	}
	double correlation_ratio=max(elem_div(abs(ratio_num),ratio_den));
#endif

#if CHECK_DETECTION_FLOOR
  // Check if correlation drops below floor before enabling detection
  bool detection_enabled = (correlation_ratio<FRAME_DETECTION_FLOOR);
  if (!detection_enabled) correlation_ratio = 0.0;
#endif

	int b=1; // need index outside of loop
	if (correlation_ratio<Th) // make sure index 0 isn't a packet
	{
		// update decision statistic
		for (; (b+L+D<dat_length)&&(correlation_ratio<Th); b++)
		{ 
			row_index=0;
			for (; row_index<Nrx; row_index++)
			{ 
				ratio_num(row_index)=ratio_num(row_index)+in_data(row_index,L+b-1)*conj(in_data(row_index,L+D+b-1));
				ratio_num(row_index)=ratio_num(row_index)-in_data(row_index,b-1)*conj(in_data(row_index,D+b-1));
				ratio_den(row_index)=ratio_den(row_index)+std::real(in_data(row_index,L+D+b-1)*conj(in_data(row_index,L+D+b-1)));
				ratio_den(row_index)=ratio_den(row_index)-std::real(in_data(row_index,D+b-1)*conj(in_data(row_index,D+b-1)));
#if USE_MAX_ENERGY_ANTENNA
        if (max_energy<ratio_den(row_index)) {max_energy=ratio_den(row_index); max_antenna=row_index;}
      }
      correlation_ratio=abs(ratio_num(max_antenna)/ratio_den(max_antenna));
      max_energy=0; max_antenna=0;
#else
			}
			correlation_ratio=max(elem_div(abs(ratio_num),ratio_den));
#endif

#if CHECK_DETECTION_FLOOR
      if (!detection_enabled) {
        detection_enabled = (correlation_ratio<FRAME_DETECTION_FLOOR);
        if (!detection_enabled) correlation_ratio = 0.0;
      }
#endif
		}
	}
	else
		unsigned int b=0;

	if (correlation_ratio<Th) // check why loop stopped
	{
		return -1;
	}
	else
	{
		return b; // indicates index where threshold was crossed
	}
}

/*
int Frame_Detection_11n::egc_frame_detect(cmat in_data)
{
// equal gain combining packet detection
// returns symbol index for packet start
// in_data = input data symbols
	
	unsigned int Nrx=in_data.rows();	
	unsigned int dat_length=in_data.cols();
	std::complex<double> ratio_num=sum(sum(elem_mult(in_data.get_cols(0,L-1),conj(in_data.get_cols(D,L+D-1)))));
	double ratio_den=sqr(norm(in_data.get_cols(D,L+D-1),2));
	double correlation_ratio=abs(ratio_num)/ratio_den;
	
	vec ratios=zeros(1);ratios(0)=correlation_ratio; // for debug...stores past ratios
	
	int b=1; // need index outside of loop
	if (correlation_ratio<Th) // make sure index 0 isn't a packet
	{
		// update decision statistic
		for (; (b+L+D<dat_length)&&(correlation_ratio<Th); b++)
		{ 
			ratio_num+=sum(elem_mult(in_data.get_col(L+b-1),conj(in_data.get_col(L+D+b-1))));
			ratio_num-=sum(elem_mult(in_data.get_col(b-1),conj(in_data.get_col(D+b-1))));
			ratio_den+=sqr(norm(in_data.get_col(L+D+b-1),2));
			ratio_den-=sqr(norm(in_data.get_col(D+b-1),2));
			correlation_ratio=abs(ratio_num)/(ratio_den);
			ratios=concat(ratios,correlation_ratio);
		}
	}
	else
		unsigned int b=0;

	if (correlation_ratio<Th) // check why loop stopped
	{
		return -1;
	}
	else
	{
		return b; // indicates index where threshold was crossed
	}

}
*/
