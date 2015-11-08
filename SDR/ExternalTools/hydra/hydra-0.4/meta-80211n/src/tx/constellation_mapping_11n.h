/*   	
 * Title: 802.11n Bit Modulator
 * Created By: Robert Daniels
 * Creation Date: 10/04/2006
 * 
 * Description: Header for "Constellation_Mapping_11n.cc"
 * 
 * Revision: v0.00 - DATE - Initial Release
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

#ifndef Constellation_Mapping_11N_H
#define Constellation_Mapping_11N_H

#include <itpp/itbase.h>
#include <itpp/comm/modulator_nd.h>

class Constellation_Mapping_11n : public itpp::ND_UQAM
{
	public:
		Constellation_Mapping_11n(); // for init
		void modulate_bits(unsigned int Nbpsc, itpp::bmat const& binary_matrix, itpp::cmat& complex_matrix); // QAM modulation
		void get_bit_vector(itpp::ivec& col_index, itpp::bmat const& bit_mat, itpp::bvec& bit_vec); // assign bits from matrix for each symbol vector
	private:
		unsigned int Nss;
		itpp::ivec M, logM;
};
#endif


