/*   	
 * Title:  Spatial Mapping for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/25/2006
 * 
 * Description: Header for "Spatial_Mapping_11n.C"
 * 
 * Revision: v0.00 - 10/31/2006 - Initial Release
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

#ifndef Spatial_Mapping_11N_H
#define Spatial_Mapping_11N_H

#include <itpp/itbase.h>

class Spatial_Mapping_11n
{
	public:
		Spatial_Mapping_11n();
		void spatial_map(itpp::cmat& data_symbols, itpp::Array<itpp::cmat> const& Q);
	private:
		itpp::ivec subcarrier_index, Tcs;
		unsigned int Ntx, Nst, Nsym, Nsts, Nfft;
		itpp::cmat Q_tmp;
};
#endif
