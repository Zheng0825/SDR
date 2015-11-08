/*  	
 * Title: Phase Correction (PHC) 
 * Created By: Robert Daniels
 * Creation Date:
 * 
 * Description: Header for "Pilot_Estimation_11n.cc"
 *
 * Revision: v0.00 
 *
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

#ifndef Pilot_Estimation_11N_H
#define Pilot_Estimation_11N_H

// Correct channel magnitude variations using pilot tones
#define CORRECT_MAGNITUDE     0

#include <itpp/itbase.h>

class Pilot_Estimation_11n
{
	public:
		Pilot_Estimation_11n(unsigned int num_space_time_streams, unsigned int num_total_subcarriers);
#if CORRECT_MAGNITUDE
 		void one_shot_estimate(itpp::cmat & symbol, unsigned int symbol_index, itpp::mat& phase_coefficient_matrix);
#else
		void one_shot_estimate(itpp::cmat const& symbol, unsigned int symbol_index, itpp::mat& phase_coefficient_matrix);
#endif
		void digital_phase_smoothing(unsigned int symbol_index, itpp::mat& phase_coefficient_matrix);
		void correct(itpp::cmat& symbol, itpp::mat const& phase_coefficient_matrix);	
	private:
		unsigned int Nst, Nsts, z, num_pilots, history_size;
		bool positive_phase, negative_phase;
		itpp::cvec pilots_polarity;
	       	itpp::vec phase_shift, past_phase;
		itpp::mat phase_history;	
		itpp::cmat pilots;
		itpp::ivec pilot_locations, pilot_index, subcarrier_mapping, subcarrier_index;
};
#endif
