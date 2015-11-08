/*   	
 * Title:  Conversion of spatially mapped data for transmission
 * Created By: Robert Daniels
 * Creation Date: 1/18/2006
 * 
 * Description: IFFT, GI insertion, and Cyclic shift
 * Revision: v0.00 - 1/18/2006 - Initial Release only for two transmit antennas
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

#include "transmission_formatting_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Transmission_Formatting_11n::Transmission_Formatting_11n(unsigned int num_sts)
{
	Nsts=num_sts;
	Nfft=64;
	sub_index="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
	null_index="0 29 30 31 32 33 34 35";
	Nst=56;
	cyclic_prefix_index="48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
	Nnull=Nfft-Nst;
	Ngi=length(cyclic_prefix_index);
	Nofdm=Nfft+Ngi;
}

void Transmission_Formatting_11n::format_ifft(cmat& data_symbols)
{
	Ntx=data_symbols.rows();
	Nsym=data_symbols.cols()/Nst;
	data_symbols.set_size(Ntx,Nsym*Nofdm,true);
	cvec null_vector=zeros_c(Ntx);
	unsigned int row_holder=0;
	unsigned int col_holder=0;
	{
		for (int k=Nsym-1; k>=0; k--)
		{
			row_holder=Nofdm*k+Ngi;
			col_holder=Nst*k;
			{
				for (int l=Nst-1; l>=0; l--)
				{
					data_symbols.copy_col(row_holder+sub_index(l),col_holder+l);
				}
			}
			{
				for (int l=Nnull-1; l>=0; l--)
				{
					data_symbols.set_col(row_holder+null_index(l),null_vector);
				}
			}	
		}
	}
	// take IFFT
	double normalization_constant=(double(Nfft)/sqrt(Nst*Nsts));
	cmat fft_matrix(Ntx,Nfft);
	{
		for (unsigned int k=0; k<Nsym; k++)
		{
			row_holder=k*Nofdm+Ngi;
			fft_matrix=data_symbols.get_cols(row_holder,(k+1)*Nofdm-1);
			for (unsigned int l=0; l<Ntx; l++)
			{
				fft_matrix.set_row(l,normalization_constant*ifft(fft_matrix.get_row(l),Nfft));
			}
			data_symbols.set_submatrix(0,row_holder,fft_matrix);
		}
	}
	// add cyclic prefix
	{
		for (int k=Nsym-1; k>=0; k--)
		{
			row_holder=Nofdm*k;
			col_holder=Nofdm*k+Nfft;
			{
				for (int l=0; l<Ngi; l++)
				{
					data_symbols.copy_col(row_holder+l,col_holder+l);
				}
			}	
		}
	}
}
