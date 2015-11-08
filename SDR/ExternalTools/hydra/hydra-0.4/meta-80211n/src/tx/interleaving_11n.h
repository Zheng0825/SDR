/*   	
 * Title:  Bit level interleaving in 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/08/2006
 * 
 * Description: Header for "Interleaving_11n.cc"
 * 
 * Revision: v0.00 - 10/08/2006 - Initial Release
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


#ifndef Interleaving_11N_H
#define Interleaving_11N_H

#include <itpp/itbase.h>

class Interleaving_11n
{
	public:
		Interleaving_11n(unsigned int ilv_rows, unsigned int ilv_cols, unsigned int Nbpsc, unsigned int num_rotations); // for init
		void interleave(itpp::bmat& binary_matrix); // interleaving (21.3.7.3)
	private:
		void interleave(itpp::bvec& data_bits, unsigned int stream_index); // with frequency rotation
		unsigned int Nrot, rows, cols, Nbpsc, Nblock; // private variables
	friend class Signal_Field_Formatting_11n;
};
#endif
		
