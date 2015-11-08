/*   	
 * Title:  Short training sequences for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Creation of short training sequences (HT and legacy)
 * Revision: v0.00 - 1/02/2007 - Initial Release only contains 20MHz HT-GF-STF 
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

#include "stf_formatting_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

STF_Formatting_11n::STF_Formatting_11n(txvector const& tx_parameters)
{
	std::complex<double>  iorj(0,1);
	stf="0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0";
	stf-="0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0";
	stf=1/sqrt(2)*stf;
	stf=stf+iorj*stf;
	Nfft=64;
	Ntone=12;
	PHTLTF="1 0 1 1;1 1 0 1;1 1 1 0;0 1 1 1";
	PHTLTF-="0 1 0 0;0 0 1 0;0 0 0 1;1 0 0 0";
	Tcs="0 0 0 0"; // cyclic shift time in negative ns/50
	Ntx=tx_parameters.N_TX;
	Nsts=tx_parameters.Nsts;
}

void STF_Formatting_11n::greenfield(cmat& tx_preamble, Array<cmat> const& Q, unsigned int& column_index) // 
{
	cmat Q_tmp=Q(0);
	stf_matrix=zeros_c(Ntx,Nfft);
	std::complex<double>  iorj(0,1);

	sub_index="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
	
	{
		for (unsigned int i=0;i<Ntx; i++)
		{		
			for (unsigned int j=0;j<56;j++)
			{
				Q_tmp=Q(j);
				for (unsigned int index=0; index<Nsts; index++)
				{
					stf_matrix(i,sub_index(j))+=stf(sub_index(j))*Q_tmp(i,index)*PHTLTF(index,0);
				}
			}
		}
	}


	{
		for (unsigned int i=0;i<Ntx;i++)
		{
			stf_matrix.set_row(i,(Nfft/sqrt(Ntone*Nsts))*ifft(stf_matrix.get_row(i),Nfft));
		}
	}

	// cyclically extend to get double size short-training field
	tx_preamble.set_submatrix(0,Ntx-1,column_index,column_index+Nfft-1,stf_matrix);
	tx_preamble.set_submatrix(0,Ntx-1,column_index+Nfft,column_index+2*Nfft-1,stf_matrix);
	tx_preamble.set_submatrix(0,Ntx-1,column_index+2*Nfft,column_index+2*Nfft+Nfft/2-1,stf_matrix.get_cols(0,Nfft/2-1));

	// update column index
	column_index+=2*Nfft+Nfft/2;
}

