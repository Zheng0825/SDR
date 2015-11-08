/*  	
 * Title:  Phase Correction (PHC) with Pilot tracking in 802.11n OFDM symbols
 * Created By: Robert Daniels
 * Creation Date: 
 * 
 * Description: Fix the phase offset of each OFDM symbol
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

#include "pilot_estimation_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Pilot_Estimation_11n::Pilot_Estimation_11n(unsigned int num_space_time_streams, unsigned int num_total_subcarriers)
{
	Nst=num_total_subcarriers;
	Nsts=num_space_time_streams;
	pilots_polarity="1 1 1 1 0 0 0 1 0 0 0 0 1 1 0 1 0 0 1 1 0 1 1 0 1 1 1 1 1 1 0 1 1 1 0 1 1 0 0 1 1 1 0 1 0 0 0 1 0 1 0 0 1 0 0 1 1 1 1 1 0 0 1 1 0 0 1 0 1 0 1 1 0 0 0 1 1 0 0 0 0 1 0 0 1 0 1 1 1 1 0 1 0 1 0 1 0 0 0 0 0 1 0 1 1 0 1 0 1 1 1 0 0 1 0 0 0 1 1 1 0 0 0 0 0 0 0";
	pilots_polarity-="0 0 0 0 1 1 1 0 1 1 1 1 0 0 1 0 1 1 0 0 1 0 0 1 0 0 0 0 0 0 1 0 0 0 1 0 0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 1 0 1 1 0 0 0 0 0 1 1 0 0 1 1 0 1 0 1 0 0 1 1 1 0 0 1 1 1 1 0 1 1 0 1 0 0 0 0 1 0 1 0 1 0 1 1 1 1 1 0 1 0 0 1 0 1 0 0 0 1 1 0 1 1 1 0 0 0 1 1 1 1 1 1 1";
	z=2;
	subcarrier_index="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
	num_pilots=4;
	if (Nsts==1)
	{
		pilots=zeros_c(1,4);
		pilots.set_row(0,concat(ones_c(3),zeros_c(1))-concat(zeros_c(3),ones_c(1))); 				
	}
	else if (Nsts==2)
	{
		pilots=zeros_c(2,4);
		pilots.set_row(0,concat(ones_c(2),zeros_c(2))-concat(zeros_c(2),ones_c(2)));
		pilots.set_row(1,concat(ones_c(1),zeros_c(2),ones_c(1))-concat(zeros_c(1),ones_c(2),zeros_c(1)));
	}
	else if (Nsts==3)
	{
		pilots=zeros_c(3,4);
		pilots.set_row(0,concat(ones_c(2),zeros_c(2))-concat(zeros_c(2),ones_c(2)));
		pilots.set_row(1,concat(ones_c(1),zeros_c(1),ones_c(1),zeros_c(1))-concat(zeros_c(1),ones_c(1),zeros_c(1),ones_c(1)));
		pilots.set_row(2,concat(zeros_c(1),ones_c(2),zeros_c(1))-concat(ones_c(1),zeros_c(2),ones_c(1)));
	}
	else if (Nsts==4)
	{
		pilots=zeros_c(4,4);
		pilots.set_row(0,concat(ones_c(3),zeros_c(1))-concat(zeros_c(3),ones_c(1))); 	
		pilots.set_row(1,concat(ones_c(2),zeros_c(1),ones_c(1))-concat(zeros_c(2),ones_c(1),zeros_c(1))); 	
		pilots.set_row(2,concat(ones_c(1),zeros_c(1),ones_c(2))-concat(zeros_c(1),ones_c(1),zeros_c(2)));
		pilots.set_row(3,concat(zeros_c(1),ones_c(3))-concat(ones_c(1),zeros_c(3)));
	}
	pilot_locations.set_size(4); // logically, at +/-7,21, so appropriate indices defined
	pilot_locations(2)=7;pilot_locations(3)=21;
	pilot_locations(0)=43;pilot_locations(1)=57;
	pilot_index.set_size(4);
	pilot_index(0)=-21;pilot_index(1)=-7;
	pilot_index(2)=7;pilot_index(3)=21;
	subcarrier_mapping="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 -28 -27 -26 -25 -24 -23 -22 -21 -20 -19 -18 -17 -16 -15 -14 -13 -12 -11 -10 -9 -8 -7 -6 -5 -4 -3 -2 -1";
	phase_shift=zeros(Nsts);
	past_phase=zeros(Nsts);
  history_size=150;  //3;
	phase_history=zeros(Nsts,history_size);
	positive_phase=false;
	negative_phase=false;
}

#if CORRECT_MAGNITUDE
void Pilot_Estimation_11n::one_shot_estimate(cmat & ofdm_symbol, unsigned int symbol_index, mat& phase_coefficient_matrix)
#else
void Pilot_Estimation_11n::one_shot_estimate(cmat const& ofdm_symbol, unsigned int symbol_index, mat& phase_coefficient_matrix)
#endif
{ // performs least-square estimation of phase rotation and offset on a symbol
  // per symbols basis

	mat left_side_matrix=ones(num_pilots,2);
	vec right_side_vector; right_side_vector.set_size(num_pilots);
#if CORRECT_MAGNITUDE
  vec pilot_mag_vector; pilot_mag_vector.set_size(num_pilots);
#endif
	std::complex<double> p=pilots_polarity(mod(symbol_index+z,127)); 
	std::complex<double> P;
	past_phase=phase_coefficient_matrix.get_col(0);
	for (unsigned int k=0; k<Nsts; k++)
	{	
		positive_phase=false;negative_phase=false;
		for (unsigned int l=0; l<num_pilots; l++)
		{
			P=pilots(k,mod(symbol_index+l,num_pilots));
			right_side_vector(l)=std::arg(P*p*ofdm_symbol(k,pilot_locations(l)));
#if CORRECT_MAGNITUDE
      pilot_mag_vector(l)=std::abs(P*p*ofdm_symbol(k,pilot_locations(l)));
#endif
			left_side_matrix(l,1)=pilot_index(l);
			if (right_side_vector(l)>pi/2)
				positive_phase=true;
			if (right_side_vector(l)<(-pi/2))
				negative_phase=true;
		}
		phase_coefficient_matrix.set_row(k,(ls_solve_od(left_side_matrix,right_side_vector)));
		if(positive_phase&&negative_phase)
		{
			if ((past_phase(k)-phase_shift(k))<0)
				phase_coefficient_matrix(k,0)=-pi;
			else
				phase_coefficient_matrix(k,0)=pi;
		}
#if CORRECT_MAGNITUDE
    /*
     * FIXME: This is a hack to allow for pilot tones to be used to compensate
     *        changes in the channel magnitude that may have occurred since the
     *        channel was estimated.
     */
     double meq = (1.0*num_pilots)/sum(pilot_mag_vector);
     cvec tmp = ofdm_symbol.get_row(k);
     ofdm_symbol.set_row(k, tmp*meq);
#endif
	}
	for (unsigned int m=0; m<Nsts; m++)
	{
		// check if phase enters positive transition region
		if ((past_phase(m)>(phase_shift(m)+pi/2))&&(phase_coefficient_matrix(m,0)<(-pi/2)))
		{	
			phase_shift(m)+=2*pi; // phase increases 2*pi
		}
		// check if phase enters negative transition region
		if ((past_phase(m)<(phase_shift(m)-pi/2))&&(phase_coefficient_matrix(m,0)>(pi/2)))
		{
			phase_shift(m)-=2*pi; // phase decreases 2*pi
		}
		phase_coefficient_matrix(m,0)+=phase_shift(m);
		past_phase(m)=phase_coefficient_matrix(m,0);
	}
}

void Pilot_Estimation_11n::digital_phase_smoothing(unsigned int symbol_index, mat& phase_coefficient_matrix)
{
	for (unsigned int a=0; a<Nsts; a++)
	{
		phase_history(a,mod(symbol_index,history_size))=phase_coefficient_matrix(a,0);
		//if (symbol_index>=(history_size-1))
		//	phase_coefficient_matrix(a,0)=sum(phase_history.get_row(a))/history_size;
	}
}

void Pilot_Estimation_11n::correct(cmat& ofdm_symbol, mat const& phase_coefficient_matrix)
{	std::complex<double> iorj(0,1);
	for (unsigned int k=0; k<Nsts; k++)
	{
		for (unsigned int l=0; l<Nst; l++)
		{
			ofdm_symbol(k,subcarrier_index(l))*=exp(-iorj*(phase_coefficient_matrix(k,0)));
		//	ofdm_symbol(k,subcarrier_index(l))*=exp(-iorj*(phase_coefficient_matrix(k,1)*subcarrier_mapping(l)));
		}
	}
}
