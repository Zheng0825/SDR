/*   	
 * Title: Bit Scrambler for 802.11 Systems
 * Created By: Robert Daniels
 * Creation Date: 10/02/2006
 * 
 * Description: Bit level scrambler (bits in -> bits out)
 * 	
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
 * Revision: v0.00 - Initial release just includes 802.11 scrambler
 */ 

#include "scrambling_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Scrambling_11n::Scrambling_11n()
{
	
}

void Scrambling_11n::scramble(bvec& datab,unsigned int data_size, unsigned int seed_size)
{ // scramble data_bits with generatory polynomial gen_poly (17.3.5.4)

	bvec breg=randb(seed_size);
	scramble_with_seed(datab,data_size,breg);
}

void Scrambling_11n::scramble_with_seed(bvec& databits, unsigned int datasize, bvec binaryseed)
{
	bin flip_bit;
	for (unsigned int k=0; k<datasize; k++)
	{
		flip_bit=binaryseed(6)+binaryseed(3);
		databits[k]+=flip_bit;
		binaryseed.shift_right(flip_bit);
	}
} 
