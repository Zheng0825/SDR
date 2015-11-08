/*  	
 * Title:  Error Control Decoding at receiver for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Header for "Soft_Bits_Decoding_11n.cc"
 *
 * Revision: v0.00 - 1/02/2007 - Initial Release
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


#ifndef Soft_Bits_Decoding_11N_H
#define Soft_Bits_Decoding_11N_H

#include <itpp/itbase.h>

class Soft_Bits_Decoding_11n
{
	public:
		Soft_Bits_Decoding_11n(unsigned int code_rate); // for init
		itpp::bvec decode(unsigned int code_type, itpp::vec code_input); 
		void encode(unsigned int code_type, itpp::bvec& code_input);
		itpp::bmat puncture_matrix;
		itpp::ivec generator;
	private:
};
#endif
