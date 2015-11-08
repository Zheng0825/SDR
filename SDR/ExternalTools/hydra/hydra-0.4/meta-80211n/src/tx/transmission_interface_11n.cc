/*   	
 * Title:  Interface for 802.11n Transmitter
 * Created By: Robert Daniels
 * Creation Date: 10/31/2006
 * 
 * Description: Given txvector, and input bits, the transmitted data symbols are outputted
 * Revision: v0.00 - 10/31/2006 - Initial Release
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

#include "transmission_interface_11n.h"
#include "preamble_formatting_11n.h"
#include "payload_formatting_11n.h"
#include "precoding_matrices_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Transmission_Interface_11n::Transmission_Interface_11n()
{
	is_data_analysis_mode=false;
	is_fixed_input_mode=false;
}

void Transmission_Interface_11n::format_frame(txvector& tx_parameters, bvec const& meta_input, bvec const& data_input, cmat& tx_output)
{
	// calculate parameters given input txvector
	tx_parameters.calculate_parameters(true);
	
	// create spatial mapping matrices for each subcarrier
	Precoding_Matrices_11n BFM(tx_parameters.N_TX,tx_parameters.Nsts,tx_parameters.Nsts_meta);
	Array<cmat> Q;Array<cmat> Qext;Array<cmat> Qmeta;
	BFM.format_data_mapping(Q,tx_parameters);
	if (tx_parameters.Ness>0)
		BFM.format_extension_mapping(Qext,tx_parameters);
	if (tx_parameters.LENGTH_meta>0)
		BFM.format_meta_mapping(Q,Qmeta,tx_parameters);
		
	// make sure tx_parameters is valid
	if (!(tx_parameters.is_valid))
	{
		cout << "ADMIN: Transmitter" << endl;
		cout << "------> ERROR: Invalid txvector...ceasing format operation!" << endl;
		tx_output.set_size(1,1);
	}
	else // continue to format output data
	{
		Preamble_Formatting_11n preamble; // declare preamble class (21.3.9)
		Payload_Formatting_11n data_symbols; // declare data formatting class (21.3.10)
		
		// set analysis modes in data formatting class
		if (is_data_analysis_mode||is_fixed_input_mode)
			data_symbols.set_modes(is_data_analysis_mode,is_fixed_input_mode);
		
		// declare matrices to contain preamble and data
		cmat tx_preamble(tx_parameters.N_TX,tx_parameters.Npresamples);
		cmat tx_meta(tx_parameters.N_TX,tx_parameters.Ndatasamples);
		cmat tx_data(tx_parameters.N_TX,tx_parameters.Ndatasamples);
	
		// format the preamble in data1
		preamble.format_preamble(tx_parameters,tx_preamble,Q,Qext); // format preamble
		data_symbols.format_payload(tx_parameters,tx_meta,tx_data,meta_input,data_input,Qmeta,Q); // format data
		// in data analysis mode, log coded bits
		if (is_data_analysis_mode)
			coded_bits=data_symbols.get_coded_bits();
		// format the concatenated output	
		tx_output.set_size(tx_parameters.N_TX,tx_preamble.cols()+tx_data.cols()+tx_meta.cols());
		tx_output=concat_horizontal(concat_horizontal(tx_preamble,tx_meta),tx_data);// assemble transmitted streams
	}
}
