/*   	
 * Title:  Data field construction for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 01/16/2007
 * 
 * Description: Header for "Payload_Formatting_11n.cc"
 * 
 * Revision: v0.00 - 01/13/2007 - Initial Release
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

#ifndef Payload_Formatting_11N_H
#define Payload_Formatting_11N_H

#include <itpp/itbase.h>
#include "txvector.h"

class Payload_Formatting_11n
{
	public:
		Payload_Formatting_11n(); 
		void format_payload(txvector const& tx_parameters, itpp::cmat& tx_meta, itpp::cmat& tx_data, itpp::bvec const& meta_input, itpp::bvec const& data_input, itpp::Array<itpp::cmat> const& Qmeta, itpp::Array<itpp::cmat> const& Q);
		inline void set_modes(bool data_mode, bool fixed_mode) {is_data_analysis_mode=data_mode; is_fixed_input_mode=fixed_mode;}
		inline itpp::bvec get_coded_bits() {return coded_bits;}
	private:	
		void ht_data(txvector const& tx_parameters, itpp::cmat& tx_data, itpp::bvec const& tx_input, itpp::Array<itpp::cmat> const& Q);
		void ht_meta(txvector const& tx_parameters, itpp::cmat& tx_meta, itpp::bvec const& tx_input, itpp::Array<itpp::cmat> const& Q);
		unsigned int Nsym, Npads;
		itpp::bvec coded_bits, binary_stream;
		itpp::cmat symbol_matrix;
		itpp::bmat binary_matrix;
		bool is_data_analysis_mode;
		bool is_fixed_input_mode;
};
#endif
