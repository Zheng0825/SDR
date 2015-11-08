/*   	
 * Title:  Data field construction for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 01/16/2007
 * 
 * Description: Creating data symbols for supporting 802.11n
 * 
 * Revision: v0.00 - 01/16/2007 - Initial Release (only supports Greenfield operation under 20 MHz channels)
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

#include "payload_formatting_11n.h"
#include "channel_encoding_11n.h"
#include "scrambling_11n.h"
#include "spatial_parsing_11n.h"
#include "interleaving_11n.h"
#include "pilot_formatting_11n.h"
#include "spatial_mapping_11n.h"
#include "transmission_formatting_11n.h"
#include "constellation_mapping_11n.h"
#include "space_time_coding_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Payload_Formatting_11n::Payload_Formatting_11n()
{
	is_fixed_input_mode=false;
}

void Payload_Formatting_11n::format_payload(txvector const& tx_parameters, cmat& tx_meta, cmat& tx_data, bvec const& meta_input, bvec const& data_input, Array<cmat> const& Qmeta, Array<cmat> const& Q)
{ 
	if (tx_parameters.LENGTH_meta!=0) // if necessary, add meta data
		ht_meta(tx_parameters,tx_meta,meta_input,Qmeta);
	else
		tx_meta.set_size(0,0);
	if (tx_parameters.LENGTH!=0) // if necessary, add data
		ht_data(tx_parameters,tx_data,data_input,Q);
	else
		tx_data.set_size(0,0);
}

void Payload_Formatting_11n::ht_meta(txvector const& tx_parameters, cmat& tx_meta, bvec const& tx_input, Array<cmat> const& Q)
{	
	// allocate memory for parameters
	binary_stream.set_size(tx_parameters.Nmetacoded);
	binary_matrix.set_size(tx_parameters.Nss_meta,tx_parameters.Nmetacoded/tx_parameters.Nss_meta);

	// initialize binary data processing classes
	Scrambling_11n SCR; // initialize scrambler (17.3.5.4)
	Channel_Encoding_11n FEC(tx_parameters.code_rate_meta); // initialize forward error correction (17.3.5.5, 21.3.10.5) 
	Spatial_Parsing_11n PAR(tx_parameters.mod_index_meta,tx_parameters.Nss_meta); // initialize stream parsing (21.3.10.7.2)
	unsigned int Nrot=11;unsigned int Ncols=13;
	Interleaving_11n ILV(4*tx_parameters.Nbpsc_meta,Ncols,tx_parameters.Nbpsc_meta,Nrot); // intialize frequency interleaver (21.3.10.7.3)
	Constellation_Mapping_11n MOD;

	// initialize complex data processing classes
	Pilot_Formatting_11n PIL(tx_parameters.Nsts_meta);
	Spatial_Mapping_11n SPM;
	Space_Time_Coding_11n STC(tx_parameters.Nsts_meta,tx_parameters.Nss_meta,tx_parameters.Nsd);
	Transmission_Formatting_11n TXM(tx_parameters.Nsts_meta);
	
	// determine the number of symbols (eq: 21-28)
	//unsigned int Nsym;
	//if (tx_parameters.STBC==0)
	//	Nsym=ceil_i(double(8*tx_parameters.LENGTH_meta+16+6*tx_parameters.Nes)/double(tx_parameters.Ndbps_meta)); 
	//else
	//	Nsym=2*ceil_i(double(8*tx_parameters.LENGTH_meta+16+6*tx_parameters.Nes)/double(2*tx_parameters.Ndbps_meta));
	
	// zero padding bits
	unsigned int Npads=tx_parameters.Nsym_meta*tx_parameters.Ndbps_meta-8*tx_parameters.LENGTH_meta-16-6*tx_parameters.Nes; 
	unsigned int Ntail=6*tx_parameters.Nes;
	
	// format binary input
	binary_stream.set_size(tx_parameters.Nmetacoded);

	unsigned int column_index=0;
	binary_stream.replace_mid(0,zeros_b(16));

	// in fixed mode, use fixed input
	if (!is_fixed_input_mode)
		binary_stream.replace_mid(16,tx_input);
	else
	{
		bvec fixed_input(tx_parameters.LENGTH_meta*8);
		int data_element;
		for (int a=0; a<tx_parameters.LENGTH_meta; a++)
		{
			data_element=mod(a,256);
			fixed_input.replace_mid(a*8,dec2bin(8,data_element));
		}
		binary_stream.replace_mid(16,fixed_input);
	}

	// complete binary input stream with zero padding
	binary_stream.replace_mid(16+8*tx_parameters.LENGTH_meta,zeros_b(Ntail+Npads));
	column_index+=(16+tx_parameters.LENGTH_meta*8+Ntail+Npads);
	
	// scramble bit sequence
	if (is_fixed_input_mode)
		SCR.scramble_with_seed(binary_stream,column_index,zeros_b(7));
	else	
		SCR.scramble(binary_stream,column_index,7);
	
	// return tail to zero state
	binary_stream.replace_mid((column_index-Ntail),zeros_b(Ntail)); 
	
	// encode the bit sequence
	if (tx_parameters.LDPC_CODING==0) 
	{ // binary convolutional encoding
		if (tx_parameters.Nes==1) // for one encoder stream
		{
			FEC.encode(0,binary_stream,column_index); // code the bistream using MCS
			binary_stream.set_size(tx_parameters.Nsym_meta*tx_parameters.Ncbps_meta,true);
					
			if (is_data_analysis_mode)
			{
				coded_bits.set_size(length(binary_stream),false);
				coded_bits=binary_stream;
			}
			PAR.parse(binary_stream,binary_matrix);
			ILV.interleave(binary_matrix);
		}
		else
		{

				cout << "ADMIN: Transmitter" << endl;
				cout << "------> ERROR: Two encoder streams not currently supported!" << endl;
		}
	}
	else // LDPC coding
	{
		cout << "ADMIN: TRANSMITTER" << endl;
		cout << "------> ERROR: LDPC coding not yet supported!" << endl;
	}

	// proceed with complex symbol formatting
	MOD.modulate_bits(tx_parameters.Nbpsc_meta,binary_matrix,tx_meta);
	if (tx_parameters.STBC>0)
	{
		STC.encode(tx_meta); // space-time block encoding
	}
	PIL.add_pilots(tx_meta); // add pilot symbols to each spatial stream
	SPM.spatial_map(tx_meta,Q); // complete spatial mapping with Q matrices
	TXM.format_ifft(tx_meta); // complete ifft and cyclic prefix for each transmitted stream
}

void Payload_Formatting_11n::ht_data(txvector const& tx_parameters, cmat& tx_data, bvec const& tx_input, Array<cmat> const& Q)
{	
	// allocate memory for parameters
	binary_stream.set_size(tx_parameters.Ndatacoded);
	binary_matrix.set_size(tx_parameters.Nss,tx_parameters.Ndatacoded/tx_parameters.Nss);

	// initialize binary data processing classes
	Scrambling_11n SCR; // initialize scrambler (17.3.5.4)
	Channel_Encoding_11n FEC(tx_parameters.code_rate); // initialize forward error correction (17.3.5.5, 21.3.10.5) 
	Spatial_Parsing_11n PAR(tx_parameters.mod_index,tx_parameters.Nss); // initialize stream parsing (21.3.10.7.2)
	unsigned int Nrot=11;unsigned int Ncols=13;
	Interleaving_11n ILV(4*tx_parameters.Nbpsc,Ncols,tx_parameters.Nbpsc,Nrot); // intialize frequency interleaver (21.3.10.7.3)
	Constellation_Mapping_11n MOD;

	// initialize complex data processing classes
	Pilot_Formatting_11n PIL(tx_parameters.Nsts);
	Spatial_Mapping_11n SPM;
	Space_Time_Coding_11n STC(tx_parameters.Nsts,tx_parameters.Nss,tx_parameters.Nsd);
	Transmission_Formatting_11n TXM(tx_parameters.Nsts);
	
	// determine the number of symbols (eq: 21-28)
	//unsigned int Nsym;
	//if (tx_parameters.STBC==0)
	//	Nsym=ceil_i(double(8*tx_parameters.LENGTH+16+6*tx_parameters.Nes)/double(tx_parameters.Ndbps)); 
	//else
	//	Nsym=2*ceil_i(double(8*tx_parameters.LENGTH+16+6*tx_parameters.Nes)/double(2*tx_parameters.Ndbps));
	
	// zero padding bits
	unsigned int Npads=tx_parameters.Nsym*tx_parameters.Ndbps-8*tx_parameters.LENGTH-16-6*tx_parameters.Nes; 
	unsigned int Ntail=6*tx_parameters.Nes;
	
	// format binary input
	binary_stream.set_size(tx_parameters.Ndatacoded);

	unsigned int column_index=0;
	binary_stream.replace_mid(0,zeros_b(16));
			
	// in fixed mode, use fixed input
	if (!is_fixed_input_mode)
		binary_stream.replace_mid(16,tx_input);
	else
	{
		bvec fixed_input(tx_parameters.LENGTH*8);
		int data_element;
		for (int a=0; a<tx_parameters.LENGTH; a++)
		{
			data_element=mod(a,256);
			fixed_input.replace_mid(a*8,dec2bin(8,data_element));
		}
		binary_stream.replace_mid(16,fixed_input);
	}

	// complete binary input stream with zero padding
	binary_stream.replace_mid(16+8*tx_parameters.LENGTH,zeros_b(Ntail+Npads));
	column_index+=(16+tx_parameters.LENGTH*8+Ntail+Npads);
	// scramble bit sequence
	if (is_fixed_input_mode)
		SCR.scramble_with_seed(binary_stream,column_index,zeros_b(7));
	else	
		SCR.scramble(binary_stream,column_index,7);
	
	// return tail to zero state
	binary_stream.replace_mid((column_index-Ntail),zeros_b(Ntail)); 
	
	// encode the bit sequence
	if (tx_parameters.LDPC_CODING==0) 
	{ // binary convolutional encoding
		if (tx_parameters.Nes==1) // for one encoder stream
		{

			FEC.encode(0,binary_stream,column_index); // code the bistream using MCS
			binary_stream.set_size(tx_parameters.Nsym*tx_parameters.Ncbps,true);
			if (is_data_analysis_mode)
			{
				coded_bits.set_size(length(binary_stream),false);
				coded_bits=binary_stream;
			}
			PAR.parse(binary_stream,binary_matrix);
			ILV.interleave(binary_matrix);
		}
		else
		{

				cout << "ADMIN: Transmitter" << endl;
				cout << "------> ERROR: Two encoder streams not currently supported!" << endl;
		}
	}
	else // LDPC coding
	{
		cout << "ADMIN: TRANSMITTER" << endl;
		cout << "------> ERROR: LDPC coding not yet supported!" << endl;
	}

	// proceed with complex symbol formatting
	MOD.modulate_bits(tx_parameters.Nbpsc,binary_matrix,tx_data);
	if (tx_parameters.STBC>0)
	{
		STC.encode(tx_data); // space-time block encoding
	}
	PIL.add_pilots(tx_data); // add pilot symbols to each spatial stream
	SPM.spatial_map(tx_data,Q); // complete spatial mapping with Q matrices
	TXM.format_ifft(tx_data); // complete ifft and cyclic prefix for each transmitted stream
}
