/*   	
 * Title:  Assigning pilot tones for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 10/10/2006
 * 
 * Description: Header for "Pilot_Formatting_11n.cc"
 * 
 * Revision: v0.00 - 10/10/2006 - Initial Release
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


#ifndef Pilot_Formatting_11N_H
#define Pilot_Formatting_11N_H

#include <itpp/itbase.h>

class Pilot_Formatting_11n
{
	public:
		Pilot_Formatting_11n(unsigned int num_space_time_streams);
		void add_pilots(itpp::cmat& data_symbols);
	private:
		unsigned int Nsts, Nsp, Nsym, z, Nsd, Nst;
		itpp::ivec p, pilot_index, data_index;
		itpp::imat pilots;
};
#endif
