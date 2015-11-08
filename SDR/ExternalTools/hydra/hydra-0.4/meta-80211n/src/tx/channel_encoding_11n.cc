/*  	
 * Title:  Bit level error control coding for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/08/2006
 * 
 * Description: Allow coding at bit level (convolutional or LDPC)
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

#include <itpp/itcomm.h>

#include "channel_encoding_11n.h"

using namespace itpp;

Channel_Encoding_11n::Channel_Encoding_11n(unsigned int code_rate)
{
	switch (code_rate) // defining appropriate puncture matrix (17.3.5.6)
	{
		case 0: // 1/2
			puncture_matrix = "1;1";
			break;
		case 1: // 2/3
			puncture_matrix = "1 1;1 0";
			break;
		case 2: // 3/4
			puncture_matrix = "1 1 0;1 0 1";
			break;
		case 3: // 5/6
			puncture_matrix= "1 1 0 1 0;1 0 1 0 1";
			break;
		default:
			puncture_matrix = "1;1";
	}
}

void Channel_Encoding_11n::encode(unsigned int code_type, bvec& code_input, unsigned int column_index) //unsigned int code_len,unsigned int column_index)
{
// LDPC and punctured binary convolutional codes
	switch (code_type) // "1" indicates LDPC, all else for BCC
	{	
		case 1: // LDPC codes of rate 1/2, 2/3, 3/4, 5/6
			// TBD
			break;
		default: // punctured codes 
			Punctured_Convolutional_Code code; // IT++ punctured BCC class
			Convolutional_Code states; // IT++ BCC class
			
			generator.set_size(2,false); // create array
  			generator(0) = 0133; // define BCC generators
  			generator(1) = 0171;

			code.set_generator_polynomials(generator, 7); // as name suggests
			code.set_puncture_matrix(puncture_matrix); // as name suggests
			states.set_start_state(0); // start at 0 state
			
			code_input=code.encode_tail(code_input(0,column_index-1)); // encode the stream
			break;
	}	
}


