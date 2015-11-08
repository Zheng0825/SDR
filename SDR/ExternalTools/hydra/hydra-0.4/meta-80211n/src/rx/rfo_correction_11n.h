/*  	
 * Title:  Residual frequency offset correction for 802.11n
 * Created By: Wonsoo Kim, Ketan Mandke
 * Creation Date: 6/26/2009
 * 
 * Description: Header for "rfo_correction_11n.cc"
 *
 * Revision: v0.00 - 6/26/2009 - Initial Release
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


#ifndef RFO_CORRECTION_11N_H
#define RFO_CORRECTION_11N_H

#include <itpp/itbase.h>

class RFO_Correction_11n
{
	public:
		RFO_Correction_11n(unsigned int num_space_time_streams, unsigned int num_total_subcarriers);
    void one_shot_estimate(itpp::cmat const& ofdm_symbol, unsigned int symbol_index, itpp::mat& phase_coefficient_matrix);
		void correct(itpp::cmat& symbol, itpp::mat const& phase_coefficient_matrix);	
	private:
		unsigned int Nst, Nsts, z, num_pilots;
    int history_size; 
		itpp::cvec pilots_polarity;
		itpp::cmat pilots;
    itpp::mat  phase_tracking, phase_tracking_smoothing, past_phase_shift; 
    itpp::Array<itpp::mat> past_phase; 
		itpp::ivec pilot_locations, pilot_index, subcarrier_index;
};
#endif
