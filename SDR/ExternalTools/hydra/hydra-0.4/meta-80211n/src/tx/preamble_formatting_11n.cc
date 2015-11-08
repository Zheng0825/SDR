/*   	
 * Title:  Preamble construction for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/31/2006
 * 
 * Description: Creating preambles to support 802.11n
 * 
 * Revision: v0.00 - 11/7/2006 - Initial Release (only supports Greenfield operation under 20 MHz channels)
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

#include "preamble_formatting_11n.h"
#include "stf_formatting_11n.h"
#include "ltf_formatting_11n.h"
#include "signal_field_formatting_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Preamble_Formatting_11n::Preamble_Formatting_11n()
{
}

void Preamble_Formatting_11n::format_preamble(txvector const& tx_parameters,cmat& tx_preamble,Array<cmat> const& Q,Array<cmat> const& Qext)
{ 
	unsigned int column_index=0;
	greenfield_preamble(tx_parameters,tx_preamble,Q,Qext,column_index);
}

void Preamble_Formatting_11n::greenfield_preamble(txvector const& tx_parameters, cmat& tx_preamble, Array<cmat> const& Q, Array<cmat> const& Qext, unsigned int& column_index)
{
	STF_Formatting_11n short_field(tx_parameters); // initiate short training object
	LTF_Formatting_11n long_field(tx_parameters); // initiate long training object
	Signal_Field_Formatting_11n signal_field(tx_parameters); // initiate signal field object
	
	short_field.greenfield(tx_preamble,Q,column_index);
	long_field.greenfield(tx_preamble,Q,column_index);
	signal_field.ht(tx_parameters,tx_preamble,Q,column_index);
	long_field.spatial(tx_preamble,Q,Qext,column_index);
}
