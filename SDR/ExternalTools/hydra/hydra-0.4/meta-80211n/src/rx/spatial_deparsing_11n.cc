/*  	
 * Title: Combine Spatial Streams (CSS)
 * Created By: Robert Daniels
 * Creation Date: 10/02/2006
 * 
 * Description: De-parse the spatial streams in 802.11n receivers
 * 	
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

#include "spatial_deparsing_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Spatial_Deparsing_11n::Spatial_Deparsing_11n(unsigned int num_data_subcarriers)
{
	nsd=num_data_subcarriers;
}

void Spatial_Deparsing_11n::combine(mat parsed_soft_bits, vec& deparsed_soft_bits, unsigned int modulation_scheme)
{
	switch (modulation_scheme)
	{
		case 0:
			bpa=1; // bits-per-axis (see eq: 21-39)
			break;
		case 1:
			bpa=1;
			break;
		case 2:
			bpa=2;
			break;
		case 3:
			bpa=3;
			break;
		default:
			break;
	}
	unsigned int nss=parsed_soft_bits.rows();
	if (nss!=1)
	{
		unsigned int number_of_groups=parsed_soft_bits.cols()/bpa;
		deparsed_soft_bits.set_size(nss*bpa*number_of_groups);
		unsigned int row_holder;
		unsigned int bpa_x_nss=bpa*nss;
		
		for (unsigned int b=0; b<nss; b++) 
		{
			row_holder=b*bpa;
			for (unsigned int c=0; c<bpa; c++)
			{
				for (unsigned int a=0; a<number_of_groups; a++)
					deparsed_soft_bits(a*bpa_x_nss+row_holder)=parsed_soft_bits(b,a*bpa+c);		
				row_holder++;
			}
		}
	}
	else
		deparsed_soft_bits=parsed_soft_bits.get_row(0);
} 
