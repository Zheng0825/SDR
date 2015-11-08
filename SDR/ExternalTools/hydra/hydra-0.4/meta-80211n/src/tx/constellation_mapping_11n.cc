/*   	
 * Title: 802.11n Bit Mapping
 * Created By: Robert Daniels
 * Creation Date: 10/04/2006
 * 
 * Description: Modulating data bits for 802.11n systems
 * 	
 * Revision: v0.00 - 10/04/2006 - Initial release
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

#include "constellation_mapping_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Constellation_Mapping_11n::Constellation_Mapping_11n() : ND_UQAM(1,4)
{
}

void Constellation_Mapping_11n::modulate_bits(unsigned int Nbpsc, bmat const& binary_matrix, cmat& complex_matrix)
{
	Nss=binary_matrix.rows();
	M=pow2i(Nbpsc)*ones_i(Nss);
	logM=int(Nbpsc)*ones_i(Nss);
	int Ncols=binary_matrix.cols()/max(logM);
	complex_matrix=zeros_c(Nss,Ncols);
	if (Nbpsc==1) // BPSK
	{
		ND_UPSK bcon(Nss,2);
		for (unsigned int j=0; j<Ncols; j++)
		{
			complex_matrix.set_col(j,bcon.modulate_bits(binary_matrix.get_col(j)));
		}
	}	
	else // QAM class only works for QPSK and above
	{
		ND_UQAM::set_M(Nss,M);
		ivec place_index=zeros_i(Nss);
		bvec bit_col=zeros_b(sum(logM));
		for (unsigned int l=0; l<Ncols; l++)
		{
			get_bit_vector(place_index,binary_matrix,bit_col);
			complex_matrix.set_col(l,ND_UQAM::modulate_bits(bit_col));
		}
	}
}

void Constellation_Mapping_11n::get_bit_vector(ivec& col_index, bmat const& bit_mat, bvec& bit_vec)
{
	int vec_index=0;
	for (int a=0; a<Nss; a++)
	{
		for (int b=0; b<logM(a); b++)
		{
				bit_vec(vec_index)=bit_mat(a,col_index(a));
				col_index(a)=col_index(a)+1;
				vec_index++;
		}
	}
}
