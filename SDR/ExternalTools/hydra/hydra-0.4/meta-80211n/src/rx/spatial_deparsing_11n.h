/*  	
 * Title:  Combine Spatial Streams (CSS)
 * Created By: Robert Daniels
 * Creation Date:
 * 
 * Description: Header for "Spatial_Deparsing_11n.cc"
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

#ifndef Spatial_Deparsing_11N_H
#define Spatial_Deparsing_11N_H

#include <itpp/itbase.h>

class Spatial_Deparsing_11n
{
	public:
		Spatial_Deparsing_11n(unsigned int num_data_subcarriers);
		void combine(itpp::mat parsed_soft_bits, itpp::vec& deparsed_soft_bits, unsigned int nbpsc);
	private:
		unsigned int nsd,bpa;
};
#endif
