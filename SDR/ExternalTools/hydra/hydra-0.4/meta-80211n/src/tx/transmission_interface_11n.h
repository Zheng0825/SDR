/*   	
 * Title: Transmitter Interface (TXI) for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/31/2006
 * 
 * Description: Header for "Transmission_Interface_11n.cc"
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

#ifndef Transmission_Interface_11N_H
#define Transmission_Interface_11N_H

#include <itpp/itbase.h>
#include "txvector.h"

class Transmission_Interface_11n
{
	public:
		Transmission_Interface_11n();

		/*! Given the input bitstream and the txvector defined parameters the
 		 * complex baseband physical layer samples of a 802.11n frame is
 		 * computed
		 *
		 * \param tx_parameters referenced txvector object with the 
		 * parameters that define the tranmission mode
	     * \param tx_input single bitstream representing the PSDU
		 * \paramtx_output 802.11n waveform represented by complex 
		 * baseband samples in the IT++ complex matrix class; rows of
		 * natrix denote the different waveforms for each tx antenna
		 */
		void format_frame(txvector& tx_parameters, itpp::bvec const& meta_input, itpp::bvec const& data_input, itpp::cmat& tx_output);

		/*! \brief Trigger data analysis mode
		 * 
		 *  \param data_mode input argument of /a true/false turns data analysis
		 *  on/off; data analysis mode is required for saving the coded bits 
		 */
		inline void trigger_data_analysis_mode(bool data_mode) {is_data_analysis_mode=data_mode;}
		
		/*! \brief Trigger fixed input mode
		 * 
		 *  \param fixed_mode input argument of /a true/false turns fixed input
		 *  on/off; fixed input mode may be used at the receiver to measure the
		 *  bit error rate 
		 */
		inline void trigger_fixed_input_mode(bool fixed_mode) {is_fixed_input_mode=fixed_mode;}
		
		/*! \brief Get the coded bits 
		 *
		 *  \retval get_coded_bits(...) returns the IT++ binary vector of bits
		 *  for the input after the encoder
		 */
		inline itpp::bvec get_coded_bits() {return coded_bits;}

	private:
		bool is_data_analysis_mode; // 'true' turns on data analysis mode
		bool is_fixed_input_mode; // 'true' uses fixed inputs
		itpp::bvec coded_bits; // contains bits after error control coding
		
};
#endif


