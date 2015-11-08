/*   	
 * Title:  Long training field for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 1/03/2007
 * 
 * Description: Header for "LTF_Formatting_11n.cc"
 * 
 * Revision: v0.00 - 1/03/2007 - Initial Release
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


#ifndef LTF_Formatting_11N_H
#define LTF_Formatting_11N_H

#include <itpp/itbase.h>
#include <itpp/itsignal.h>
#include "txvector.h"

class LTF_Formatting_11n
{
	public:
		LTF_Formatting_11n(txvector const& tx_parameters); // for init
		void non_ht(itpp::cmat& tx_preamble, unsigned int& column_index);
		void greenfield(itpp::cmat& tx_preamble, itpp::Array<itpp::cmat> const& Q, unsigned int& column_index);
		void spatial(itpp::cmat& tx_preamble, itpp::Array<itpp::cmat> const& Q, itpp::Array<itpp::cmat> const& Qext, unsigned int& column_index);
	private:
		unsigned int Ntx, Nsts, Ntone, Nprefix, Nfft;
		itpp::cvec ltf;itpp::cmat ltf_matrix;
		itpp::ivec Tcs,sub_index;
		itpp::imat PHTLTF;
		double normalization_factor;
		bool is_greenfield;
};
#endif
