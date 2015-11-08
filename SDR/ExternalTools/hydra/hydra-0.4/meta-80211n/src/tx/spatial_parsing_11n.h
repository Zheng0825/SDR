/*   	
 * Title: 802.11n Spatial Bit Streams Creator
 * Created By: Robert Daniels
 * Creation Date: 10/04/2006
 * 
 * Description: Header for "Spatial_Parsing_11n.cc"
 * 
 * Revision: v0.00 - 10/04/2006 - Initial Release
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

#ifndef Spatial_Parsing_11N_H
#define Spatial_Parsing_11N_H

#include <itpp/itbase.h>

class Spatial_Parsing_11n
{
	public:
		Spatial_Parsing_11n(unsigned int modulation_index, unsigned int num_spatial_streams); // for init
		void parse(itpp::bvec const& binary_stream, itpp::bmat& binary_matrix); // spatial stream	
	private:
		unsigned int Nss, bits_per_axis; // bits per axis
};
#endif


