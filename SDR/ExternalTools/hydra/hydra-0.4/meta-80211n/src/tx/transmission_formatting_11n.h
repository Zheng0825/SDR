/*   	
 * Title: Conversion of spatially mapped data to transmitted data
 * Created By: Robert Daniels
 * Creation Date: 1/18/2007
 * 
 * Description: Header for "Transmission_Formatting_11n.cc"
 * 
 * Revision: v0.00 - 1/18/2007 - Initial Release
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


#ifndef Transmission_Formatting_11N_H
#define Transmission_Formatting_11N_H

#include <itpp/itbase.h>
#include <itpp/itsignal.h>

class Transmission_Formatting_11n
{
	public:
		Transmission_Formatting_11n(unsigned int num_sts);
		void format_ifft(itpp::cmat& data_symbols);
	private:
		unsigned int Ntx, Nsts, Nst, Nsym, Nfft, Nnull, Ngi, Nofdm;
		itpp::ivec null_index, sub_index,cyclic_prefix_index;
};
#endif
