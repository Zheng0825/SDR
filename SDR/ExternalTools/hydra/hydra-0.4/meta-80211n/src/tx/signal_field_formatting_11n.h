/*   	
 * Title: SIGNAL field creation for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 01/07/2007
 * 
 * Description: Header for "Signal_Field_Formatting_11n.C"
 * 
 * Revision: v0.00 - 01/07/2007 - Initial Release
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

#ifndef Signal_Field_Formatting_11N_H
#define Signal_Field_Formatting_11N_H

#include "txvector.h"
#include <itpp/itbase.h>
#include <itpp/itsignal.h>

class Signal_Field_Formatting_11n
{
	public:
		Signal_Field_Formatting_11n(txvector const& tx_parameters);
		void ht(txvector const& tx_parameters, itpp::cmat& tx_preamble, itpp::Array<itpp::cmat> const& Q, unsigned int& column_index);
	private:
		void generate_20mhz_greenfield(itpp::cmat& tx_preamble,itpp::Array<itpp::cmat> const& Q, unsigned int& column_index);
		unsigned int Nsts, Ntx, Ntone, Nfft;
		itpp::ivec Tcs;
		itpp::cmat sig_matrix, Q_tmp;
		itpp::bvec HTSIG1,HTSIG2,HTSIG3;

};
#endif

