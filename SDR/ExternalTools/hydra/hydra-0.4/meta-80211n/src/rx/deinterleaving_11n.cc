/*  	
 * Title:  Bit level de-interleaving at receiver for 802.11n
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

#include "deinterleaving_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Deinterleaving_11n::Deinterleaving_11n(unsigned int ilv_rows, unsigned int ilv_cols, unsigned int N_BPSC)
// for 802.11n cols=13, rows=4*N_BPSC in 20 MHz channels
// for 802.11n cols=18, rows=6*N_BPSC in 40 MHz channels
{
	rows=ilv_cols; // initializing private variables
	cols=ilv_rows; // cols/rows reversed from interleaver
	sym_bits=N_BPSC;
	//if (((rows!=13)&&(rows!=18))||((cols!=sym_bits/4)&&(cols!=sym_bits/6)))
	//	cout << "Warning: This interleaver configuration is not 802.11n compliant!" << endl;
	blksize=rows*cols;
	N_ROT=11;
}

void Deinterleaving_11n::deinterleave(mat& data_syms) // frequency deinterleaving of soft spatial streams
{
	unsigned int Nss=data_syms.rows();
	vec soft_bits_vector;
	for (unsigned int s=0; s<Nss; s++)
	{
		soft_bits_vector=data_syms.get_row(s);
		deinterleave(soft_bits_vector,s);
		data_syms.set_row(s,soft_bits_vector);
	}
}

void Deinterleaving_11n::deinterleave(vec& data_syms, unsigned int stream_index)
// frequency interleaving with rotation
// for 802.11n N_ROT=11 for 20 MHz
// for 802.11n N_ROT=29 for 40 MHz
// stream_index=0,1,2,3 for spatial streams 1,2,3,4 respectively
{
	//if (N_ROT!=11 && N_ROT!=29)
	//	cout << "Warning: N_ROT needs to be 11 or 29 for 802.11n compliance!" << endl;
	int freq_shift=((((stream_index*2)%3)+3*(stream_index==3))*N_ROT*sym_bits)%blksize;
	insize=size(data_syms);
    	num_blocks=length(data_syms)/blksize; // number of interleaving blocks
    	
	// perform frequency shift (rotation) on each block
	if (stream_index != 0)
	{
		for (int i=0; i<num_blocks; i++){
			data_syms.replace_mid(i*blksize,concat(data_syms((i+1)*blksize-freq_shift,(i+1)*blksize-1),data_syms(i*blksize,(i+1)*blksize-freq_shift-1))); // replace each block with circularly right shifted by freq_shift
;
		}
	}	
	
	// conventional deinterleaving
	vec temp_syms(blksize*num_blocks);
    	for (int i=0; i<num_blocks; i++){
		for (int j=0; j<rows; j++){ // interleaver loop (21.3.10.7.3)
			for (int k=0; k<cols; k++){
				temp_syms(i*blksize+j*cols+k) = data_syms(i*blksize+k*rows+j);
			}
		}
	}
	data_syms=temp_syms;	
}

