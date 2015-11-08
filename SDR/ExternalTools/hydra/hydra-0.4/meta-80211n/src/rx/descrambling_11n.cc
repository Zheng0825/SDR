/*  	
 * Title: Bit Descrambler for 802.11n Systems
 * Created By: Robert Daniels
 * Creation Date: 10/02/2006
 * 
 * Description: Bit level descrambler (bits in -> bits out)
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

#include "descrambling_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Descrambling_11n::Descrambling_11n()
{
}

void Descrambling_11n::descramble(bvec& in_bits)
{ // scramble data_bits with generator polynomial (17.3.5.4)
	bvec binary_seed=zeros_b(7);
	
	// estimate scrambler seed from SERVICE
	binary_seed(0)=in_bits(2)+in_bits(6); // x1
	binary_seed(1)=in_bits(1)+in_bits(5); // x2
	binary_seed(2)=in_bits(0)+in_bits(4); // x3
	binary_seed(3)=in_bits(2)+in_bits(3)+in_bits(6); // x4
	binary_seed(4)=in_bits(1)+in_bits(2)+in_bits(5); // x5
	binary_seed(5)=in_bits(0)+in_bits(1)+in_bits(4); // x6
	binary_seed(6)=in_bits(0)+in_bits(2)+in_bits(3)+in_bits(6); // x7

	// scramble descramble are the same operation
	scramble(in_bits,binary_seed);
	// delete SERVICE BITS
	in_bits.del(0,15);
}

void Descrambling_11n::scramble(bvec& in_bits, bvec const& binary_seed)
{
	int data_size=size(in_bits);
	// scramble the bits
	bin flip_bit;bvec breg=binary_seed;
	for (int k=0; k<data_size; k++)
	{
		flip_bit=breg(6)+breg(3);
		in_bits[k]+=flip_bit;
		breg.shift_right(flip_bit);
	}
} 
