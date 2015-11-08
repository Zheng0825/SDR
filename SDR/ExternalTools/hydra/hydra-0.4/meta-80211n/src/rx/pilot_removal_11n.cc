/*  	
 * Title:  Remove Pilots (RMP)
 * Created By: Robert Daniels
 * Creation Date: 
 * 
 * Description: Remove the pilot tones from an ofdm symbol
 * Revision: v0.00 -
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

#include "pilot_removal_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Pilot_Removal_11n::Pilot_Removal_11n(unsigned int num_total_subcarriers)
{
	Nst=num_total_subcarriers;
	switch (Nst)
	{
		case 52:
			data_index="1 2 3 4 5 6 8 9 10 11 12 13 14 15 16 17 18 19 20 22 23 24 25 26 38 39 40 41 42 44 45 46 47 48 49 50 51 52 53 54 55 56 58 59 60 61 62 63";
			pilot_offset="0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4";
			Nsd=48;
			break;
		case 56:	
			data_index="1 2 3 4 5 6 8 9 10 11 12 13 14 15 16 17 18 19 20 22 23 24 25 26 27 28 36 37 38 39 40 41 42 44 45 46 47 48 49 50 51 52 53 54 55 56 58 59 60 61 62 63";
			pilot_offset="0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4";
			Nsd=52;
			break;
		case 104:
			break;
		case 114:
			data_index="2 3 4 5 6 7 8 9 10 12 13 14 15 16 17 18 19 20 21 22 23 24 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 54 55 56 57 58 70 71 72 73 74 76 77 78 79 80 81 82 83 84 85 86 87 88 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 118 119 120 121 122 123 124 125 126";
			pilot_offset="0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 6 6 6 6 6 6 6 6 6";
			Nsd=108;
			break;
	}
	shuffle_value=Nsd/2;
}

void Pilot_Removal_11n::remove(cmat& ofdm_sym)
{
	// move data subcarriers to beginning columns of matrix
	int k=0;
	for (k=0; k<Nsd; k++)
	{
		ofdm_sym.copy_col(k,data_index(k));
	}
	// shuffle back to negative subcarriers first
	for (k=0; k<(shuffle_value); k++)
	{
		ofdm_sym.swap_cols(k,k+shuffle_value);
	}
	// resize matrix
	int nrows=ofdm_sym.rows();
	ofdm_sym.set_size(nrows,Nsd,true);
}

void Pilot_Removal_11n::remove(mat& pp_snr)
{
	// move data subcarriers to beginning columns of matrix
	int k=0;
	for (k=0; k<Nsd; k++)
	{
		pp_snr.copy_col(k,k+pilot_offset(k));
	}
	// shuffle back to negative subcarriers first
	for (k=0; k<(shuffle_value); k++)
	{
		pp_snr.swap_cols(k,k+shuffle_value);
	}
	// resize matrix
	int nrows=pp_snr.rows();
	pp_snr.set_size(nrows,Nsd,true);
}
