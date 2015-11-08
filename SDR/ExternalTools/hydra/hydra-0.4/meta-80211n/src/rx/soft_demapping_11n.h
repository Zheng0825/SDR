/*  	
 * Title:  Demodulation (DEM)
 * Created By: Robert Daniels
 * Creation Date:
 * 
 * Description: Header for "Linear_Soft_Demapping_11n.cc"
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

#ifndef Soft_Demapping_11N_H
#define Soft_Demapping_11N_H

#include <itpp/itbase.h>
#include <itpp/itstat.h>
#include "itpp/comm/modulator_nd.h"
#include "itpp/comm/modulator.h"
#include "itpp/comm/llr.h"

class Soft_Demapping_11n : public itpp::ND_UQAM
{
	public:
		Soft_Demapping_11n(unsigned int Nbpsc, unsigned int Ncbps, bool is_ML, itpp::Array<itpp::cmat> H);
		itpp::mat demodulate_soft_bits(unsigned int Nbpsc, itpp::cmat const& symbol, itpp::cmat const& ref_symbol, itpp::mat& post_process_snr, double N0);
	private:
		itpp::vec soft_bits_row;
		itpp::ivec M, logM, submap;
		itpp::mat return_mat;
		itpp::Array<itpp::cmat> channels;
		bool isML;
		unsigned int Nss, Nrx, Nst, bitcols;
};
#endif
