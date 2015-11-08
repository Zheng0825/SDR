/*!  	
 *	\brief Space-time block encoder for 802.11n
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

#ifndef Space_Time_Coding_11N_H
#define Space_Time_Coding_11N_H
#endif

#include <itpp/itbase.h>
//! \class Space-time block encoding for 802.11n
class Space_Time_Coding_11n
{
	public:
		/*! \brief Constructor initializes the encoder by providing the number
		 *  of space-time streams as well as the number of spatial streams
		 *
		 *  \param num_space_time_streams referred to as "Nsts" in the 802.11n
		 *  standard
		 *  \param num_spatial_streams referred to as "Nss" in the 802.11n
		 *  standard
         
         nst = total number of sub-carriers
         nss = num spatial stream
         
		 *  \param num_subcarriers referred to as "Nsd" in the 802.11n standard
		 */
		Space_Time_Coding_11n(unsigned int num_space_time_streams, unsigned int
num_spatial_streams, unsigned int num_subcarriers);
		
		/*! \brief Create the encoded space-time block encoded OFDM symbols
		 *	
		 *  \param input_symbols is spatial stream inputs to be encoded and
		 *  yield space-time stream outputs; passed by reference 
		 */ 
		void encode(itpp::cmat& input_symbols);

	private:
		unsigned int Nsts, Nss, Nsd;
};

