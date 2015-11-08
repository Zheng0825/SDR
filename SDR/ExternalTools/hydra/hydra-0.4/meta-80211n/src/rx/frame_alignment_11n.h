/*  	
 * Title:  Exact packet detection for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Header for "Frame_Alignment_11n.cc"
 *
 * Revision: v0.00 - 1/02/2007 - Initial Release
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


#ifndef Frame_Alignment_11N_H
#define Frame_Alignment_11N_H

#include <itpp/itbase.h>
#include <itpp/itstat.h>

class Frame_Alignment_11n
{
	public:
		Frame_Alignment_11n( unsigned int win_size); // for init
		unsigned int schmidl_cox_egc(itpp::cmat in_data,  unsigned int anticipation_symbols);
		unsigned int schmidl_cox_mrc(itpp::cmat in_data, itpp::cvec rx_ratios, unsigned int anticipation_symbols);
		unsigned int zhou_saito_egc(itpp::cmat in_data); // TBD
		unsigned int zhou_saito_mrc(itpp::cmat in_data, itpp::cvec rx_ratios); // TBD
	private:
		unsigned int L, D;
		double Th, decision_metric;
};
#endif
