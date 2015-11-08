/*   	
 * Title: Bit Scrambler for 802.11n Systems
 * Created By: Robert Daniels
 * Creation Date: 10/02/2006
 * 
 * Description: Header for "Scrambling_11n.cc"
 * 
 * Revision: v0.00 - Initial release just includes 802.11 scrambler
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

#ifndef Scrambling_11n_H
#define Scrambling_11n_H

#include <itpp/itbase.h>

class Scrambling_11n
{
	public:
		Scrambling_11n(); 
		void scramble(itpp::bvec& datab, unsigned int data_size, unsigned int seed_size);
		void scramble_with_seed(itpp::bvec& databits, unsigned int datasize, itpp::bvec binaryseed);
	private:	
};
#endif

