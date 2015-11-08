/*   	
 * Title: 802.11n Spatial Bit Streams Creator
 * Created By: Robert Daniels
 * Creation Date: 10/04/2006
 * 
 * Description: Separate single bit stream into Nss spatial bit streams
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

#include "spatial_parsing_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Spatial_Parsing_11n::Spatial_Parsing_11n(unsigned int modulation_index, unsigned int num_spatial_streams)
{
	Nss=num_spatial_streams;
	
	// modindex: 0=BPSK,1=QPSK,2=16-QAM,3=64-QAM
	switch (modulation_index)
	{
		case 0:
			bits_per_axis=1; // bits-per-axis (see eq: 21-39)
			break;
		case 1:
			bits_per_axis=1;
			break;
		case 2:
			bits_per_axis=2;
			break;
		case 3:
			bits_per_axis=3;
			break;
		default:
			break;
	}
}

void Spatial_Parsing_11n::parse(bvec const& binary_stream, bmat& binary_matrix)
{ 	
	unsigned int total_bits=length(binary_stream); // number of bits total
	unsigned int spatial_stream_groups=total_bits/(bits_per_axis*Nss); // number of round-robin groups in each spatial stream
	binary_matrix.set_size(Nss,total_bits/Nss);
	if (Nss!=1)
	{
		// keep assigning bits in a round robin fashion to different streams
		for (unsigned int j=0; j<spatial_stream_groups; j++)
		{
			for (unsigned int i=0; i<Nss; i++)
			{
				for (unsigned int k=0; k<bits_per_axis; k++)
				{
					binary_matrix(i,j*bits_per_axis+k)=binary_stream(Nss*j*bits_per_axis+i*bits_per_axis+k);
				}
			}
		}
	}
	else
		binary_matrix.set_row(0,binary_stream);
}
