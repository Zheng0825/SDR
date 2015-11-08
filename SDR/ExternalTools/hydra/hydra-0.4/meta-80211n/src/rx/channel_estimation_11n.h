/*  	
 * Title:  Spatial Channel Estimation (SCE)
 * Created By: Robert Daniels
 * Creation Date:
 * 
 * Description: Header for "Channel_Estimation_11n.cc"
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

#ifndef Channel_Estimation_11N_H
#define Channel_Estimation_11N_H

#include <itpp/itbase.h>
#include <itpp/itsignal.h>
#include <itpp/itstat.h>
#include "rxvector.h"

class Channel_Estimation_11n
{
	public:
		Channel_Estimation_11n(rxvector const& rx_param);
		
		itpp::cmat egc_ls_fde(itpp::cmat channel_block);
		void estimate(itpp::cmat receive_stream, unsigned int est_mode, itpp::Array<itpp::cmat>& returned_array); // Ness=0
		void extension_estimate(itpp::cmat receive_stream, unsigned int est_mode, itpp::Array<itpp::cmat>& returned_array); // Ness>0
		void smooth_channel_estimate(unsigned int smoothing_index, itpp::Array<itpp::cmat>& channel_estimate);
		void meta_channel_conversion(itpp::Array<itpp::cmat> const& channel_estimate, itpp::Array<itpp::cmat>& meta_estimate);	
		inline itpp::mat get_subcarrier_energy() {return subcarrier_energy;}		
		double get_average_snr () { return average_snr; }		
		double get_noise_variance() { return noise_variance; }
		
	private:
		unsigned int Nst, Nfft, Ngi, Nofdm, Nsts, Ness, Nsts_meta; // internal parameters
		double average_snr;
		double noise_variance;
		itpp::ivec subcarrier_indices;
		itpp::cvec train;
		itpp::mat subcarrier_energy;
		itpp::cmat PHTLTF;
		void unwrap_channel_phase(itpp::Array<itpp::mat>& channel_phase);
};
#endif
