/*   	
 * Title:  Preamble construction for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/31/2006
 * 
 * Description: Header for "Preamble_Formatting_11n.cc"
 * 
 * Revision: v0.00 - 01/09/2006 - Initial Release
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

#ifndef Preamble_Formatting_11N_H
#define Preamble_Formatting_11N_H

#include <itpp/itbase.h>
#include "txvector.h"

class Preamble_Formatting_11n
{
	public:
		Preamble_Formatting_11n();
		void format_preamble(txvector const& tx_parameters, itpp::cmat& tx_preamble,itpp::Array<itpp::cmat> const& Q, itpp::Array<itpp::cmat> const& Qext);
	private:
		void greenfield_preamble(txvector const& tx_parameters, itpp::cmat& tx_preamble, itpp::Array<itpp::cmat> const& Q, itpp::Array<itpp::cmat> const& Qext, unsigned int& column_index);
};
#endif
