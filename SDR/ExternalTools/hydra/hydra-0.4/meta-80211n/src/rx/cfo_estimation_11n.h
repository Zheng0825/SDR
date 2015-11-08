/*  	
 * Title:  Fine frequency estimation for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Header for "CFO_Estimation_11n.cc"
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


#ifndef CFO_Estimation_11N_H
#define CFO_Estimation_11N_H

#include <itpp/itbase.h>

class CFO_Estimation_11n
{
	public:
		CFO_Estimation_11n(unsigned int len_window, double samp_freq); // for init
		double schmidl_cox_egc(itpp::cmat estimation_block, bool is_coarse);
		double schmidl_cox_mrc(itpp::cmat estimation_block, itpp::cvec rx_ratios);
		double moose();
		void cfo_correct(double cfo_offset, itpp::cmat& cfo_data);
	private:
		unsigned int L, D;
		double Ts, decision_metric;
};
#endif
