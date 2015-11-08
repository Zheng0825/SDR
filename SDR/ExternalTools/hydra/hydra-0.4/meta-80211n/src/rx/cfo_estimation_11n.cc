/*  	
 * Title:  Fine Frequency Estimation (FFE)
 * Created By: Robert Daniels
 * Creation Date: 
 * 
 * Description: Fine frequency estimation for 802.11n preambles
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

#include "cfo_estimation_11n.h"

// correct frequency offset by using "best" antenna
#define CFO_WITH_MAX    1

using namespace itpp;
using std::cout;
using std::endl;

CFO_Estimation_11n::CFO_Estimation_11n(unsigned int len_window, double samp_freq)
{
	L=len_window;
	D=64;
	Ts=1/samp_freq;
}

double CFO_Estimation_11n::schmidl_cox_egc(cmat estimation_block, bool is_coarse)
{
// estimation_block contains three periods of cfo estimation symbols
// equal gain combining of streams for schmidl-cox CFO estimation

	if (is_coarse) // different delays for coarse estimation
		D/=4;
  /*
   * FIXME: This is new code added to use a different window size for second
   * metric computed in coarse estimation.
   */
  unsigned int old_L = L;
  unsigned int D2 = D;
  if (is_coarse) D2 = D*2;
  if (is_coarse) L += 1*D;

	unsigned int Nrx=estimation_block.rows();
#if CFO_WITH_MAX
  cvec corr1; cvec corr2;
  corr1.set_size(Nrx); corr2.set_size(Nrx);
  for (unsigned int k=0; k<Nrx; k++) {
    corr1(k) = std::complex<double>(0,0); corr2(k) = std::complex<double>(0,0);
  }
#endif
	double z1=0,z2=0;
#if CFO_WITH_MAX
	for (unsigned int row_index=0; row_index<Nrx; row_index++)
	{
    corr1(row_index)=sum(sum(elem_mult(estimation_block.get(row_index,row_index,0,L-1),conj(estimation_block.get(row_index,row_index,D,L+D-1)))));
    if (is_coarse)
      corr2(row_index)=sum(sum(elem_mult(estimation_block.get(row_index,row_index,0,L-1),conj(estimation_block.get(row_index,row_index,D2,D2+L-1)))));
	}
  unsigned int max_antenna1=0, max_antenna2=0;
  double max_corr1=0, max_corr2=0;
  for (unsigned int row_index=0; row_index<Nrx; row_index++)
  {
    if (max_corr1<abs(corr1(row_index)))   {max_antenna1=row_index; max_corr1=abs(corr1(row_index));}
    if (is_coarse)
      if (max_corr2<abs(corr2(row_index))) {max_antenna2=row_index; max_corr2=abs(corr2(row_index));}
  }
  z1 = arg(corr1(max_antenna1));
  //z1 = arg(corr1(max_antenna2));
  if (is_coarse) {
    z2 = arg(corr2(max_antenna1));
    //z2 = arg(corr2(max_antenna2));
  }
  Nrx = 1;  // reset Nrx for normalizing angle
#else
	for (unsigned int row_index=0; row_index<Nrx; row_index++)
	{
		z1+=arg(sum(sum(elem_mult(estimation_block.get(row_index,row_index,0,L-1),conj(estimation_block.get(row_index,row_index,D,L+D-1))))));
		if (is_coarse)
			z2+=arg(sum(sum(elem_mult(estimation_block.get(row_index,row_index,0,L-1),conj(estimation_block.get(row_index,row_index,D2,D2+L-1))))));
	}
#endif
	z1*=-1/(2*pi*D*Ts*Nrx); // Schmidl-Cox estimation
	if (is_coarse)
	{	
		z2*=-1/(2*pi*D2*Ts*Nrx);
		D*=4; // reset delay
	}
  /*
   * FIXME: Restore window size L. Extended window only used for coarse
   * estimation.
   */
  if (is_coarse) L = old_L;

	return 0.5*(z1+z2); 
}

double CFO_Estimation_11n::schmidl_cox_mrc(cmat estimation_block, cvec rx_ratios)
{
	return 0;
}


double CFO_Estimation_11n::moose()
{

}

void CFO_Estimation_11n::cfo_correct(double cfo_offset, cmat& cfo_data)
{
	unsigned int data_len=cfo_data.cols();
	std::complex<double>  iorj(0,1); 
	for (int a=0;a<data_len;a++)
		cfo_data.set_col(a,cfo_data.get_col(a)*exp(-1*iorj*2*pi*cfo_offset*a*Ts));
}

