/*   	
 * Title:  Assigning tones and to 802.11n space-time streams
 * Created By: Robert Daniels
 * Creation Date: 10/10/2006
 * 
 * Description: Inserting pilot tones  for 802.11n.
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

#include "pilot_formatting_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Pilot_Formatting_11n::Pilot_Formatting_11n(unsigned num_space_time_streams)
{
	Nsts=num_space_time_streams;
	p="1 1 1 1 0 0 0 1 0 0 0 0 1 1 0 1 0 0 1 1 0 1 1 0 1 1 1 1 1 1 0 1 1 1 0 1 1 0 0 1 1 1 0 1 0 0 0 1 0 1 0 0 1 0 0 1 1 1 1 1 0 0 1 1 0 0 1 0 1 0 1 1 0 0 0 1 1 0 0 0 0 1 0 0 1 0 1 1 1 1 0 1 0 1 0 1 0 0 0 0 0 1 0 1 1 0 1 0 1 1 1 0 0 1 0 0 0 1 1 1 0 0 0 0 0 0 0"; // pilot polarity vector (size 127)
	p*=2;p+=-1;
	Nsp=4;
	pilot_index="7 21 34 48";
	data_index="0 1 2 3 4 5 6 8 9 10 11 12 13 14 15 16 17 18 19 20 22 23 24 25 26 27 28 29 30 31 32 33 35 36 37 38 39 40 41 42 43 44 45 46 47 49 50 51 52 53 54 55";
	z=2;
	Nsd=52;
	Nst=56;
	switch (Nsts)
	{
		case 1:
			pilots="1 1 1 0";
			pilots*=2;pilots-=1;
			break;
		case 2:
			pilots="1 1 0 0;1 0 0 1";
			pilots*=2;pilots-=1;
			break;
		case 3:
			pilots="1 1 0 0;1 0 1 0;0 1 1 0";
			pilots*=2;pilots-=1;
			break;
		case 4:
			pilots="1 1 1 0;1 1 0 1;1 0 1 1;0 1 1 1";
			pilots*=2;pilots-=1;
			break;	
	}
	Nsym=0;
}

void Pilot_Formatting_11n::add_pilots(cmat& data_symbols)
{ 
	
	// calculate the number of OFDM symbols 
	if ((data_symbols.cols()%Nsd)!=0)
	{
		cout << "ADMIN: Transmitter" << endl;
		cout << "------> ERROR: Cannot add pilots; cannot evenly format OFDM symbol!" << endl;
	}
	else
		Nsym=data_symbols.cols()/Nsd;
	
	data_symbols.set_size(Nsts,Nsym*Nst,true);
	// add pilots to the symbol
	unsigned int row_holder=0;
	unsigned int col_holder=0;

	for (int k=Nsym-1; k>=0; k--)
	{	
		row_holder=k*Nst;
		col_holder=k*Nsd;
		{
			for (int l=Nsd-1; l>=0; l--)
			{
				data_symbols.swap_cols(row_holder+data_index(l),col_holder+l);
			}
		}
		{
			for (int l=Nsp-1; l>=0; l--)
			{
				// add pilot with per symbol polarity (20.3.10.10.2/3)		
				data_symbols.set_col(row_holder+pilot_index(l),p(mod(k+z,127))*ones_c(Nsts)); 
				for (unsigned int m=0; m<Nsts; m++)
				{
					// add pilot with per symbol/space-time-stream polarity (20.3.10.9)
					data_symbols(m,row_holder+pilot_index(l))*=pilots(m,mod(k+l,Nsp));
				}
			}
		}
	}
}
