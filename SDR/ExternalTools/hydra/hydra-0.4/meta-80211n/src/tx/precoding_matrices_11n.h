/*! \brief Linear combining of received 802.11n streams
 *	\author Robert Daniels
 * 	\date: 
 *	
 * 	Description: 
 * 
 *	Revision:
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


#ifndef Precoding_Matrices_11N_H
#define Precoding_Matrices_11N_H

/*
 * FIXME: Added a fix that causes a non-uniform gain to be applied to the
 *        subcarriers to compensate for transmit filter shaping.
 */
#define USRP_COMPENSATION_FILTER    1
#define USRP_COMPENSATION_GAIN      2.0

#include "txvector.h"
#include <itpp/itbase.h>

class Precoding_Matrices_11n
{
	public:
		Precoding_Matrices_11n(unsigned int Ntx, unsigned int Nsts, unsigned int Nsts_meta);
		void format_data_mapping(itpp::Array<itpp::cmat>& Q, txvector const& tx_parameters);	
		void format_meta_mapping(itpp::Array<itpp::cmat> const& Q, itpp::Array<itpp::cmat>& Qmeta, txvector const& tx_parameters);	
	
		void format_data_mapping(itpp::Array<itpp::cmat>& Q, txvector const& tx_parameters, itpp::ivec const& cyclic_shifts);	
		void format_extension_mapping(itpp::Array<itpp::cmat>& Qext, txvector const& tx_parameters);	
	private:
		itpp::ivec subcarrier_mapping, Tcs;
		itpp::cmat D, D_meta;
#if USRP_COMPENSATION_FILTER
    itpp::vec compensation_gain;
    double max_compensation_gain;
#endif
};
#endif
