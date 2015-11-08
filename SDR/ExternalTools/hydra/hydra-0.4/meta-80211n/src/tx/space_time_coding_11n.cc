/*!  	
 *	\brief Space-time block encoder for 802.11n
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

#include "space_time_coding_11n.h"

using namespace itpp;
using namespace std;
 
Space_Time_Coding_11n::Space_Time_Coding_11n(unsigned int num_space_time_streams, unsigned int num_spatial_streams, unsigned int num_subcarriers)
{
	Nsts=num_space_time_streams;
	Nss=num_spatial_streams;
	Nsd=num_subcarriers;
}

void Space_Time_Coding_11n::encode(itpp::cmat& input_symbols)
{
	// input_symbols will be a sequence of OFDM symbols where each OFDM
	// symbol consists of nst columns of the input_symbols matrix and NSS rows
	// of the input_symbols matrix; encoding may take place over several OFDM
	// symbols at a time, depending on the dimensions of the system;  After
	// encoding the input_symbols matrix will hold the encoded symbols which
	// will consist of the same number of columns but instead NSTS rows;  see the
	// 802.11n standard for more details
    
	unsigned int Nsym=input_symbols.cols()/Nsd;
	cmat reference_matrix=input_symbols;
	input_symbols.set_size(Nsts,input_symbols.cols(),true);
	switch (Nsts*10+Nss)
	{
		case 21: // Nsts=2, Nss=1
			{
				for (int i=0; i<(Nsym/2); i++)
				{
					for (int j=0; j<Nsd; j++)
					{
						input_symbols(1,j+2*Nsd*i)=-1*std::conj(input_symbols(0,j+2*Nsd*i+Nsd));
						input_symbols(1,j+2*Nsd*i+Nsd)=std::conj(input_symbols(0,j+2*Nsd*i));
					}
				}
				
			}
			break;
		case 32: // Nsts=3, Nss=2
			{
				for (int i=0; i<(Nsym/2); i++)
				{
					for (int j=0; j<Nsd; j++)
					{
						input_symbols(1,j+2*Nsd*i)=-1*std::conj(input_symbols(0,j+2*Nsd*i+Nsd));
						input_symbols(1,j+2*Nsd*i+Nsd)=std::conj(input_symbols(0,j+2*Nsd*i));
					}
				}
				input_symbols.set_row(2,reference_matrix.get_row(1));
			}
			break;
		case 42: // Nsts=4, Nss=2
			{
				for (int i=0; i<(Nsym/2); i++)
				{
					for (int j=0; j<Nsd; j++)
					{
						input_symbols(1,j+2*Nsd*i)=-1*std::conj(input_symbols(0,j+2*Nsd*i+Nsd));
						input_symbols(1,j+2*Nsd*i+Nsd)=std::conj(input_symbols(0,j+2*Nsd*i));
						input_symbols(3,j+2*Nsd*i)=-1*std::conj(input_symbols(1,j+2*Nsd*i+Nsd));
						input_symbols(3,j+2*Nsd*i+Nsd)=std::conj(input_symbols(1,j+2*Nsd*i));
					}
				}
				input_symbols.set_row(2,reference_matrix.get_row(1));
			}
			break;
		case 43: // Nsts=4, Nss=3
			{
				for (int i=0; i<(Nsym/2); i++)
				{
					for (int j=0; j<Nsd; j++)
					{
						input_symbols(1,j+2*Nsd*i)=-1*std::conj(input_symbols(0,j+2*Nsd*i+Nsd));
						input_symbols(1,j+2*Nsd*i+Nsd)=std::conj(input_symbols(0,j+2*Nsd*i));
					}
				}
				input_symbols.set_row(2,reference_matrix.get_row(1));
				input_symbols.set_row(3,reference_matrix.get_row(2));
			}
			break;
	}
}


