/*   	
 * Title:  Long training sequences for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 1/03/2007
 * 
 * Description: Creation of long training sequences (HT and legacy)
 * Revision: v0.00 - 1/03/2007 - Initial Release only contains 20MHz 2 antenna HT/GF data LTFs 
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

#include "ltf_formatting_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

LTF_Formatting_11n::LTF_Formatting_11n(txvector const& tx_parameters)
{
	Ntx=tx_parameters.Ntx;
	ltf="0 1 0 0 1 1 0 1 0 1 0 0 0 0 0 1 1 0 0 1 0 1 0 1 1 1 1 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 1 1 0 1 0 1 1 1 1 1 1 0 0 1 1 0 1 0 1 1 1 1";
	ltf-="0 0 1 1 0 0 1 0 1 0 1 1 1 1 1 0 0 1 1 0 1 0 1 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 1 0 1 0 0 0 0 0 0 1 1 0 0 1 0 1 0 0 0 0"; // ht ltf 20 MHz sequence
	Ntone=56; // number of active tones in ht ltf sequence
	Nprefix=16;
	Nfft=64;
	normalization_factor=(double(Nfft)/sqrt(Ntone))/sqrt(tx_parameters.Nsts); 
	Tcs="0 0 0 0"; // cyclic shift time in negative ns/50
	PHTLTF="1 0 1 1;1 1 0 1;1 1 1 0;0 1 1 1";
	PHTLTF-="0 1 0 0;0 0 1 0;0 0 0 1;1 0 0 0";
}

void LTF_Formatting_11n::greenfield(cmat& tx_preamble, Array<cmat> const& Q, unsigned int& column_index) // double length greenfield LTF1
{
	cmat Q_tmp=Q(0);
	unsigned int Nsts=Q_tmp.cols();
	ltf_matrix=zeros_c(Ntx,Nfft);
	std::complex<double>  iorj(0,1);

	// subcarrier placement
	sub_index="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";

	{
		for (unsigned int i=0;i<Ntx; i++)
		{	
			for (unsigned int j=0; j<Ntone; j++)
			{
				Q_tmp=Q(j);
				for (unsigned int index=0; index<Nsts; index++)
				{
					ltf_matrix(i,sub_index(j))+=ltf(sub_index(j))*Q_tmp(i,index)*PHTLTF(index,0)*exp(iorj*sub_index(j)*2*pi*(Tcs(index)/double(Nfft)));
				}
			}
		}
	}
	{
		for (unsigned int i=0;i<Ntx;i++)
		{
			ltf_matrix.set_row(i,normalization_factor*ifft(ltf_matrix.get_row(i),Nfft));
		}
	}
	

	// double size with double prefix
	tx_preamble.set_submatrix(0,column_index,ltf_matrix.get_cols(Nfft-2*Nprefix,Nfft-1));
	tx_preamble.set_submatrix(0,column_index+2*Nprefix,ltf_matrix);
	tx_preamble.set_submatrix(0,column_index+2*Nprefix+Nfft,ltf_matrix);
	
	// update column index
	column_index+=2*(Nfft+Nprefix);
}

void LTF_Formatting_11n::spatial(cmat& tx_preamble, Array<cmat> const& Q, Array<cmat> const& Qext, unsigned int& column_index)
{
	unsigned int Nsts=Q(0).cols();
	unsigned int Ndltf;
	switch (Nsts)
	{
		case 1:
			Ndltf=1;
			break;
		case 2:
			Ndltf=2;
			break;
		case 3:
			Ndltf=4;
			break;
		case 4:
			Ndltf=4;
			break;
	}
	// intialize return and ltf matrices
	ltf_matrix.set_size(Ntx,Nfft);
	std::complex<double>  iorj(0,1);
	
	// create data ltfs
	unsigned int data_index=0;
	data_index=1;

	// subcarrier placement
	sub_index="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
	
	if (data_index<Ndltf)
	{
		cmat Q_tmp;
		Q_tmp.set_size(Ntx,Nsts);
		for (; data_index<Ndltf; data_index++)
		{
			ltf_matrix=zeros_c(Ntx,Nfft);		
			for (unsigned int i=0; i<Ntx; i++)
			{		
				for (unsigned int j=0; j<Ntone; j++)
				{
					Q_tmp=Q(j);
					for (unsigned int index=0; index<Nsts; index++)
					{	
						ltf_matrix(i,sub_index(j))+=ltf(sub_index(j))*Q_tmp(i,index)*PHTLTF(index,data_index);
					}
				}
			}
			for (unsigned int i=0;i<Ntx;i++)
			{
				ltf_matrix.set_row(i,normalization_factor*ifft(ltf_matrix.get_row(i),Nfft));
			}

		// add cyclic prefix of single-length symbol
		tx_preamble.set_submatrix(0,column_index,ltf_matrix.get_cols(Nfft-Nprefix,Nfft-1));
		tx_preamble.set_submatrix(0,column_index+Nprefix,ltf_matrix);
		
		// update column index
		column_index+=Nfft+Nprefix;
		}
	}
	
	unsigned int Neltf;
	if (Qext.size()==0)
		Neltf=0;
	else
	{
		// create extension ltfs
		Neltf=Qext(0).cols();
		
		// extended training
		cmat Qext_tmp;
		Qext_tmp.set_size(Qext(0).rows(),Qext(0).cols());	
		for (unsigned int ext_index=0; ext_index<Neltf; ext_index++)
		{	
			ltf_matrix=zeros_c(Ntx,Nfft);
			for (unsigned int k=0; k<Ntx; k++)
			{		
				for (unsigned int l=0;l<Ntone; l++)
				{
					Qext_tmp=Qext(l);
					ltf_matrix(k,sub_index(l))+=ltf(sub_index(l))*Qext_tmp(k,ext_index); // no cyclic shifting or PHTLTF for extension streams in beamforming experiment
				}
			}
			for (unsigned int k=0;k<Ntx;k++)
			{
				ltf_matrix.set_row(k,normalization_factor*sqrt(Nsts)*ifft(ltf_matrix.get_row(k),Nfft));
			}
				
		// add cyclic prefix of single-length symbol
		tx_preamble.set_submatrix(0,column_index,ltf_matrix.get_cols(Nfft-Nprefix,Nfft-1));
		tx_preamble.set_submatrix(0,column_index+Nprefix,ltf_matrix);

		// update column index
		column_index+=Nfft+Nprefix;
		}
	}
}	

