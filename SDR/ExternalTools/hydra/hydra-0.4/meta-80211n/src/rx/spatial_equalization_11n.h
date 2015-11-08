/*  	
 * Title:  Decoupled Equalization (DEQ)
 * Created By: Robert Daniels
 * Creation Date:
 * 
 * Description: Header for "Spatial_Equalization_11n.cc"
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

#ifndef Spatial_Equalization_11N_H
#define Spatial_Equalization_11N_H

#include <itpp/itbase.h>
#include <itpp/itstat.h>
#include <itpp/itsignal.h>

class Spatial_Equalization_11n
{
	public:
		Spatial_Equalization_11n(unsigned int num_space_time_streams, unsigned int num_total_subcarriers);
		void set_equalization_mode(unsigned int eq_mode);
		void set_noise_variance(double noisevar);
		void equalize(itpp::cmat& modulated_symbol, itpp::cmat& modulated_freq_symbol, itpp::Array<itpp::cmat> const& channels);
		void compute_post_process_snr(itpp::Array<itpp::cmat> const& channels, itpp::mat& post_process_snr);
		
	private:
		void fde(itpp::Array<itpp::cmat> const& H, itpp::cmat& eq_data);
		unsigned int equalization_mode;
		unsigned int Nsts, Nst, Nfft, Nrx;
		int deq_index;
		itpp::ivec subcarrier_index;
		double noise_variance, normalization_factor, channel_energy;
		std::complex<double> x_scalar;
		itpp::cvec fft_vector;
		itpp::cmat G,H_tmp,H_tmp_H,H_I,y_mat,x_mat,N;
};
#endif
