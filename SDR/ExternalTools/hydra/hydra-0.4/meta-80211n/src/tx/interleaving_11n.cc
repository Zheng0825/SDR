/*   	
 * Title:  Bit level interleaving for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/08/2006
 * 
 * Description: Frquency interleaving with Frequency Rotation
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

#include "interleaving_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Interleaving_11n::Interleaving_11n(unsigned int ilv_rows, unsigned int ilv_cols, unsigned int symbol_bits, unsigned int num_rotations)
// for 802.11n cols=13, rows=4*N_BPSC in 20 MHz channels
// for 802.11n cols=18, rows=6*N_BPSC in 40 MHz channels
{
	Nrot=num_rotations;
	rows=ilv_rows; // initializing private variables
	cols=ilv_cols;
	Nbpsc=symbol_bits;
	Nblock=rows*cols;
}

void Interleaving_11n::interleave(bmat& binary_matrix) // frequency interleaving without rotation
{
	unsigned int Nss=binary_matrix.rows();
	bvec binary_vector;
	for (unsigned int s=0; s<Nss; s++)
	{
		binary_vector=binary_matrix.get_row(s);
		interleave(binary_vector,s);
		binary_matrix.set_row(s,binary_vector);
	}
}

void Interleaving_11n::interleave(bvec& data_bits, unsigned int stream_index)
{
	unsigned int frequency_shift=((((stream_index*2)%3)+3*(stream_index==3))*Nrot*Nbpsc)%Nblock;
   	unsigned int Nilv=length(data_bits)/Nblock; // number of interleaving blocks
	if ((length(data_bits)%Nblock)!=0) // if the input stream isn't divisible by ilv block size
    	{
		cout << "ADMIN: Transmitter" << endl;
	    	cout << "------> ERROR: Interleave matrix not full (incorrect zero padding)!" << endl;
    	}
    	Nilv=length(data_bits)/Nblock; // number of interleaving blocks
    	bvec temp_bits(Nblock*Nilv);
    	for (int i=0; i<Nilv; i++)
	{
		for (int j=0; j<rows; j++)
		{ // interleaver loop (21.3.10.7.3)
			for (int k=0; k<cols; k++)
			{
				temp_bits(i*Nblock+j*cols+k) = data_bits(i*Nblock+k*rows+j);
			}
		}
	}
	// perform frequency shift (rotation) on each block
	if (stream_index != 0)
	{
		for (int i=0; i<Nilv; i++)
		{
			temp_bits.replace_mid(i*Nblock,concat(temp_bits(i*Nblock+frequency_shift,(i+1)*Nblock-1),temp_bits(i*Nblock,i*Nblock+frequency_shift-1))); // replace each block with circularly left shifted by frequency_shift
		}
	}			
	data_bits=temp_bits;	
}


