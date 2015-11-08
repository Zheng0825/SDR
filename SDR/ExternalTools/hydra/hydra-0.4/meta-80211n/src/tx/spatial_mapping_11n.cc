/*   	
 * Title:  Spatial Mapping for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/25/2006
 * 
 * Description: Spatial Mapping operations for 802.11n
 * 
 * Revision: v0.00 - 10/25/2006 - Initial Release only include 2 antenna scenario
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

#include "spatial_mapping_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Spatial_Mapping_11n::Spatial_Mapping_11n()
{
	Nfft=64;
	subcarrier_index="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
	Nst=56;
	Tcs="0 0 0 0"; // negative ns/50
	Nsym=0;Nsts=0;Ntx=0;
}

void Spatial_Mapping_11n::spatial_map(cmat& data_symbols, Array<cmat> const& Q) 
{
	Ntx=Q(0).rows();Nsts=data_symbols.rows();
	Nsym=data_symbols.cols()/Nst;
	std::complex<double>  iorj(0,1);	
	
	
	// check spatial mapping matrix is correct size
	if ((Ntx<Nsts)||(Q(0).cols()!=Nsts))
	{
		cout << "ADMIN: Transmitter" << endl;
		cout << "------> ERROR: Spatial mapping matrix does not contain the correct dimensions!" << endl;
	}
	
	// perform spatial mapping
	if (Ntx==1) // for trivial case, no spatial stream combining 
	{
		// iterate through all transmit antennas and OFDM symbols
		for (unsigned int k=0; k<Nsym; k++)
		{	
			// rearrange order of subcarriers for fft (start with DC tone)
			for (unsigned int swap_index=0;swap_index<Nst/2;swap_index++)
				data_symbols.swap_cols(k*Nst+swap_index,k*Nst+Nst/2+swap_index);
			
			// multiply by spatial mapping value (abs=1)
			for (unsigned int l=0; l<Nst; l++)
			{
				data_symbols(0,Nst*k+l)*=Q(l)(0,0);
			}
		}
	}
	else
	{
		ivec data_domain(Nst);
		for (unsigned int a=0; a<Nst; a++)
		{
			data_domain(a)=mod(a+Nst/2,Nst); // for rearranging order of subcarriers
		}
		// add rows to symbol matrix for spatial expansion mapping
		if (Ntx>Nsts)
		{
			data_symbols=concat_vertical(data_symbols,zeros_c(Ntx-Nsts,data_symbols.cols()));
		}
		cmat spm_matrix(Ntx,Nst);

		// iterate through all transmit antennas and OFDM symbols
		unsigned int col_calc;
		for (unsigned int k=0;k<Nsym;k++)
		{
			spm_matrix.clear();	
			for (unsigned int i=0; i<Ntx; i++)
			{
				for (unsigned int l=0; l<Nst; l++)
				{
					col_calc=k*Nst+data_domain(l);
					for (unsigned int j=0; j<Nsts; j++)
					{
						spm_matrix(i,l)+=data_symbols(j,col_calc)*Q(l)(i,j)*exp(iorj*2*pi*subcarrier_index(l)*Tcs(j)/double(Nfft));
					}
				}
			}
			data_symbols.set_submatrix(0,k*Nst,spm_matrix);
		}
		//if (Nsts!=Ntx)
		//	data_symbols*=sqrt(double(Nsts)/double(Ntx)); // energy normalization
	}
	
	// phase rotate upper subcarriers for 40 mhz signals
	if (Nst>64)
	{
		for (unsigned int k=0; k<Nsym;k++)
		{
			for (unsigned int l=0;l<Nst/2;l++)
			{
				data_symbols.set_col(k*Nst+l,data_symbols.get_col(k*Nst+l)*iorj);
			}
		}
	}
}
