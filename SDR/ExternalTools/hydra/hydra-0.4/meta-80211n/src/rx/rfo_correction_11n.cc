/*  	
 * Title:  Residual frequency offset correction for 802.11n
 * Created By: Robert C. Daniels
 * Revised By: Wonsoo Kim, Ketan Mandke
 * Creation Date: 6/26/2009
 * 
 * Description: Fix the residual frequency offset/Phase noise of each OFDM symbol
 *              using interpolation-based scheme
 *
 * Revision: v0.00 - 6/26/2009 - Initial Release
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

#include "rfo_correction_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

RFO_Correction_11n::RFO_Correction_11n(unsigned int num_space_time_streams, unsigned int num_total_subcarriers)
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
		pilots.set_row(2,concat(ones_c(1),zeros_c(2),ones_c(1))-concat(zeros_c(1),ones_c(2),zeros_c(1)));
	}
	else if (Nsts==4)
	{
		pilots=zeros_c(4,4);
		pilots.set_row(0,concat(ones_c(3),zeros_c(1))-concat(zeros_c(3),ones_c(1))); 	
		pilots.set_row(1,concat(ones_c(2),zeros_c(1),ones_c(1))-concat(zeros_c(2),ones_c(1),ones_c(1))); 	
		pilots.set_row(2,concat(ones_c(1),zeros_c(1),ones_c(2))-concat(zeros_c(1),ones_c(1),zeros_c(2)));
		pilots.set_row(3,concat(zeros_c(1),ones_c(3))-concat(ones_c(1),zeros_c(3)));
	}
	pilot_locations.set_size(4); // logically, at +/-7,21, so appropriate indices defined
	pilot_locations(2)=7;pilot_locations(3)=21;
	pilot_locations(0)=43;pilot_locations(1)=57;
	pilot_index.set_size(4);
	pilot_index(0)=-21;pilot_index(1)=-7;
	pilot_index(2)=7;pilot_index(3)=21;
  history_size=1;
  past_phase.set_size(history_size);
}

void RFO_Correction_11n::one_shot_estimate(cmat const& ofdm_symbol, unsigned int symbol_index, mat& phase_coefficient_matrix)
{ 
  // performs estimation of phase of pilots and then interpolation per symbols basis

	std::complex<double> p=pilots_polarity(mod(symbol_index+z,127)); 
	std::complex<double> P;
  itpp::mat tmp_mat; 

  // initialize parameters`
  if(symbol_index==0)
  {
    past_phase_shift=zeros(Nsts,num_pilots);
    for(unsigned int a=0; a<history_size; a++)
      past_phase(a)=zeros(Nsts,64);
  }
  phase_tracking=zeros(Nsts,64);
  phase_tracking_smoothing=zeros(Nsts,64);
  tmp_mat=zeros(Nsts,num_pilots);

  //printf("symbol_index=%d\n",symbol_index);
  // Estimate phase of pilots based on current OFDM symbol
	for (unsigned int k=0; k<Nsts; k++)
	{	
    // estimate phase of pilots
		for (unsigned int l=0; l<num_pilots; l++)
		{
			P=pilots(k,mod(symbol_index+l,num_pilots));
			phase_tracking(k,pilot_locations(l))=std::arg(P*p*ofdm_symbol(k,pilot_locations(l)));
      //printf("phase_pilots(%d)=%f\n", l, phase_tracking(k,pilot_locations(l)));
		}
    
    // check the transition from negative (or positive) to positive (or negative)
    // if so, correct the phase by adding 2pi or subtracting 2pi
    bool phase_transition=true; 

    if((phase_tracking(k,7)>0)&&(phase_tracking(k,21)>0)&&(phase_tracking(k,43)>0)&&(phase_tracking(k,57)>0))
      phase_transition=false; 
    if((phase_tracking(k,7)<0)&&(phase_tracking(k,21)<0)&&(phase_tracking(k,43)<0)&&(phase_tracking(k,57)<0))
      phase_transition=false; 

		for (unsigned int l=0; l<num_pilots; l++)
		{
      if((phase_transition)&&(past_phase_shift(k,l)>pi/2)&&(phase_tracking(k,pilot_locations(l))<-pi/2))
        phase_tracking(k,pilot_locations(l))+=2*pi; 
      if((phase_transition)&&(past_phase_shift(k,l)<-pi/2)&&(phase_tracking(k,pilot_locations(l))>pi/2))
        phase_tracking(k,pilot_locations(l))-=2*pi; 
      
      //printf("past_phase(%d)=%f, revised_phase_pilots(%d)=%f\n", l, past_phase_shift(k,l), l, phase_tracking(k,pilot_locations(l)));

      tmp_mat(k,l)=phase_tracking(k,pilot_locations(l));
    }
    // update past phase 
    past_phase_shift=tmp_mat;

    //smoothing based on previous symbols to reduce estimation error 
    past_phase(mod(symbol_index,history_size))=phase_tracking;
    for(unsigned int a=0; a<history_size; a++)
      phase_tracking_smoothing+=past_phase(a);

    if(symbol_index<(history_size-1))
      phase_tracking_smoothing=phase_tracking_smoothing/(double)(symbol_index+1); 
    else
      phase_tracking_smoothing=phase_tracking_smoothing/(double)history_size; 

    // interpolation-based phase tracking method
    for(unsigned int m=0; m<64; m++)
    {
      if(m>57)
        phase_tracking_smoothing(k,m)=phase_tracking_smoothing(k,57)+(m-57)*(phase_tracking_smoothing(k,7)-phase_tracking_smoothing(k,57))/14;
      else if ((m>=0)&&(m<7))
        phase_tracking_smoothing(k,m)=phase_tracking_smoothing(k,57)+(m+7)*(phase_tracking_smoothing(k,7)-phase_tracking_smoothing(k,57))/14;
      else if ((m>7)&(m<21))
        phase_tracking_smoothing(k,m)=phase_tracking_smoothing(k,7)+(m-7)*(phase_tracking_smoothing(k,21)-phase_tracking_smoothing(k,7))/14;
      else if ((m>43)&&(m<57))
        phase_tracking_smoothing(k,m)=phase_tracking_smoothing(k,43)+(m-43)*(phase_tracking_smoothing(k,57)-phase_tracking_smoothing(k,43))/14;
      else if ((m>21)&&(m<29))
        phase_tracking_smoothing(k,m)=phase_tracking_smoothing(k,21)+(m-21)*(phase_tracking_smoothing(k,21)-phase_tracking_smoothing(k,7))/14;
      else if ((m>35)&&(m<43))
        phase_tracking_smoothing(k,m)=phase_tracking_smoothing(k,43)-(43-m)*(phase_tracking_smoothing(k,57)-phase_tracking_smoothing(k,43))/14;
        
    }

  }
}

void RFO_Correction_11n::correct(cmat& ofdm_symbol, mat const& phase_coefficient_matrix)
{	
  std::complex<double> iorj(0,1);
	for (unsigned int k=0; k<Nsts; k++)
	{
		for (unsigned int l=0; l<Nst; l++)
			ofdm_symbol(k,subcarrier_index(l))*=exp(-iorj*(phase_tracking_smoothing(k,subcarrier_index(l))));
	}
}
