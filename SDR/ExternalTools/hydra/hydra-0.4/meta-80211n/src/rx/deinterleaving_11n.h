/*  	
 *	Title:  De-interleaving at receiver for 802.11n
 *	Created By: Robert Daniels
 *	Creation Date: 1/02/2007
 *	
 *	Description: Header for "Deinterleaving_11n.cc"
 *
 *	Revision: v0.00 - 1/02/2007 - Initial Release
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


#ifndef Deinterleaving_11N_H
#define Deinterleaving_11N_H

#include <itpp/itbase.h>

class Deinterleaving_11n
{
	public:
		Deinterleaving_11n( unsigned int ilv_rows, unsigned int ilv_cols, unsigned int N_BPSC); // for init
		void deinterleave(itpp::mat& data_syms);
	private:
		void deinterleave(itpp::vec& data_syms, unsigned int stream_index);
		unsigned int rows,cols,sym_bits,blksize,num_blocks,insize,N_ROT;
};
#endif
