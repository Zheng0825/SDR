/*   	
 * Title:  Spatial mapping matrices for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Spatial mapping matrices for 802.11n
 * 
 * Revision: v0.00 - 1/02/2007 - Initial Release
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

#include "precoding_matrices_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Precoding_Matrices_11n::Precoding_Matrices_11n(unsigned int Ntx, unsigned int Nsts, unsigned Nsts_meta)
{

	subcarrier_mapping="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
	Tcs="0 8 4 12"; // negative ns/50
	switch (Ntx*10+Nsts)
	{
		case 11:
			break;
		case 21: // Ntx=2, Nsts=1
			D="1;1";D*=sqrt(double(1)/2);
			break;
		case 22: // Ntx=2, Nsts=2
			break;
		case 31: // Ntx=3, Nsts=1
			D="1;1;1";D*=sqrt(double(1)/3);
			break;
		case 32: // Ntx=3, Nsts=2
			D="1 0;0 1;0.7071 0.7071";D*=sqrt(double(2)/3);
			break;
		case 33: // Ntx=3, Nsts=3
			break;
		case 41: // Ntx=4, Nsts=1
			D="1;1;1;1";D*=sqrt(double(1)/4);
			break;
		case 42: // Ntx=4, Nsts=2

			D="1 0;0 1;1 0;0 1";D*=sqrt(double(1)/2);
			break;
		case 43: // Ntx=4, Nsts=3
			D="1 0 0;0 1 0;0 0 1;0.57735 0.57735 0.57735";D*=sqrt(double(3)/2);
			break;
		case 44: // Ntx=4, Nsts=4
			break;
		default:
			cout << "ERROR: Antenna dimensions not supported for creating beamforming matrices!" << endl;
	}
	switch (Ntx*10+Nsts_meta)
	{
		case 11:
			break;
		case 21: // Ntx=2, Nsts=1
			D_meta="1;1";D_meta*=sqrt(double(1)/2);
			break;
		case 22: // Ntx=2, Nsts=2
			break;
		case 31: // Ntx=3, Nsts=1
			D_meta="1;1;1";D_meta*=sqrt(double(1)/3);
			break;
		case 32: // Ntx=3, Nsts=2
			D_meta="1 0;0 1;0.7071 0.7071";D_meta*=sqrt(double(2)/3);
			break;
		case 33: // Ntx=3, Nsts=3
			break;
		case 41: // Ntx=4, Nsts=1
			D_meta="1;1;1;1";D_meta*=sqrt(double(1)/4);
			break;
		case 42: // Ntx=4, Nsts=2
			D_meta="1 0;0 1;1 0;0 1";D_meta*=sqrt(double(1)/2);
			break;
		case 43: // Ntx=4, Nsts=3
			D_meta="1 0 0;0 1 0;0 0 1;0.57735 0.57735 0.57735";D_meta*=sqrt(double(3)/2);
			break;
		case 44: // Ntx=4, Nsts=4
			break;
		default:
			cout << "ERROR: Antenna dimensions not supported for creating beamforming matrices!" << endl;
	}
#if USRP_COMPENSATION_FILTER
  // Initialize compensation filter parameters
  txvector t;
  int Nst = subcarrier_mapping.size();
  int Nfft = t.Nfft;
  int midpoint = Nst/2;
  compensation_gain.set_size(Nst);
  max_compensation_gain = USRP_COMPENSATION_GAIN;
  // create filter using above parameters
  int k; double x;
  double g = max_compensation_gain;
  g = (g>1.0)?g:1.0;
  for (int a=0; a<Nst; a++) {
    k = subcarrier_mapping(a);
    if (k>midpoint) k = k - Nfft;
    x = double(k)/(Nst/2);
    compensation_gain(a) = (g-1.0)*(fabs(x*x*x)) + 1.0;
  }
#endif
}



void Precoding_Matrices_11n::format_data_mapping(Array<cmat>& Q, txvector const& tx_parameters)
{ 
	std::complex<double> iorj(0,1);
	Q.set_size(tx_parameters.Nst);
	
	// create cyclic shift diversity matrix, if necessary
	Array<cmat> CSD(tx_parameters.Nst);
	if (tx_parameters.N_TX>tx_parameters.Nsts)
	{
		for (unsigned int a=0; a<tx_parameters.Nst; a++)
		{
			CSD(a)=eye_c(tx_parameters.N_TX);
			for (unsigned int b=tx_parameters.Nsts; b<tx_parameters.Ntx; b++)
			{
				CSD(a)(b,b)=exp(-iorj*2*pi*subcarrier_mapping(a)*Tcs(b)/double(tx_parameters.Nfft));
			}
		}
		switch (tx_parameters.EXPANSION_MAT_TYPE)
		{	
			case NO_EXPANSION_MAT:
				{
					for (unsigned int a=0; a<tx_parameters.Nst; a++)
						Q(a)=CSD(a)*D;
				}	
				break;

			case NON_COMPRESSED_SV:
				{
					for (unsigned int a=0; a<tx_parameters.Nst; a++)
						Q(a)=CSD(a)*tx_parameters.EXPANSION_MAT(a);
				}	
				break;
			case CSI_MATRICES:
				cout << "ERROR: CSI Matrix feedback not currently supported!" << endl;
				break;
			case COMPRESSED_SV:
				cout << "ERROR: Compresses BF feedback not currently supported!" << endl;
				break;
		}
	}	
	else // no CSD matrix operation
	{
		switch (tx_parameters.EXPANSION_MAT_TYPE)
		{	
			case NO_EXPANSION_MAT:
				{
					for (unsigned int a=0; a<tx_parameters.Nst; a++)
						Q(a)=eye_c(tx_parameters.N_TX);
				}
				break;
			case NON_COMPRESSED_SV:
				{
					for (unsigned int a=0; a<tx_parameters.Nst; a++)
						Q(a)=tx_parameters.EXPANSION_MAT(a);
				}
				break;
			case CSI_MATRICES:
				cout << "ERROR: CSI Matrix feedback not currently supported!" << endl;
				break;
			case COMPRESSED_SV:
				cout << "ERROR: Compresses BF feedback not currently supported!" << endl;
				break;
		}
	}
#if USRP_COMPENSATION_FILTER
  for (unsigned int a=0; a<tx_parameters.Nst; a++)
    Q(a)*=compensation_gain(a);
#endif
}


void Precoding_Matrices_11n::format_data_mapping(Array<cmat>& Q, txvector const& tx_parameters, ivec const& cyclic_shifts)
{ 

	Q.set_size(tx_parameters.Nst);
	std::complex<double> iorj(0,1);
	// create cyclic shift diversity matrix
	Array<cmat> CSD(tx_parameters.Nst);
	{
		for (unsigned int a=0; a<tx_parameters.Nst; a++)
		{
			CSD(a)=eye_c(tx_parameters.N_TX);
			for (unsigned int b=0; b<tx_parameters.Ntx; b++)
			{
				CSD(a)(b,b)=exp(-iorj*2*pi*subcarrier_mapping(a)*cyclic_shifts(b)/double(tx_parameters.Nfft));
			}
		}
	}
	if (tx_parameters.Ntx>tx_parameters.Nsts)
	{
		switch (tx_parameters.EXPANSION_MAT_TYPE)
		{	
			case NO_EXPANSION_MAT:
				{
					for (unsigned int a=0; a<tx_parameters.Nst; a++)
						Q(a)=CSD(a)*D;
				}		
				break;
			case NON_COMPRESSED_SV:
				{
					for (unsigned int a=0; a<tx_parameters.Nst; a++)
						Q(a)=CSD(a)*tx_parameters.EXPANSION_MAT(a);
				}	
				break;
			case CSI_MATRICES:
				cout << "ERROR: CSI Matrix feedback not currently supported!" << endl;
				break;
			case COMPRESSED_SV:
				cout << "ERROR: Compresses BF feedback not currently supported!" << endl;
				break;
		}
	}
	else //  full rank matrix
	{

		switch (tx_parameters.EXPANSION_MAT_TYPE)
		{	
			case NO_EXPANSION_MAT:
				{
					for (unsigned int a=0; a<tx_parameters.Nst; a++)
						Q(a)=CSD(a);
				}
				break;
			case NON_COMPRESSED_SV:
				{
					for (unsigned int a=0; a<tx_parameters.Nst; a++)
						Q(a)=CSD(a)*tx_parameters.EXPANSION_MAT(a);
				}
				break;
			case CSI_MATRICES:
				cout << "ERROR: CSI Matrix feedback not currently supported!" << endl;
				break;
			case COMPRESSED_SV:
				cout << "ERROR: Compresses BF feedback not currently supported!" << endl;
				break;
		}
	}

#if USRP_COMPENSATION_FILTER
  for (unsigned int a=0; a<tx_parameters.Nst; a++)
    Q(a)*=compensation_gain(a);
#endif
}

void Precoding_Matrices_11n::format_meta_mapping(Array<cmat> const& Q, Array<cmat>& Qmeta, txvector const& tx_parameters)
{ 
	std::complex<double> iorj(0,1);
	Qmeta.set_size(tx_parameters.Nst);
	cmat cmat_template=zeros_c(tx_parameters.N_TX,tx_parameters.Nsts_meta);
	unsigned int index=0;
	unsigned int index2=0;
	// meta mapping depends on data mapping for proper channel estimation
	switch(10*tx_parameters.Nsts+tx_parameters.Nsts_meta)
	{
		case 11:
		case 22:
		case 33:
		case 44:
			Qmeta=Q;
			break;
		case 21:
			for (;index<tx_parameters.Nst;index++)
			{
				index2=0;
				for (; index2<tx_parameters.N_TX; index2++)
				{
					cmat_template(index2,0)=Q(index)(index2,0)+Q(index)(index2,1);
				}
				Qmeta(index)=cmat_template;
			}
			break;
		case 31:
			for (;index<tx_parameters.Nst;index++)
			{
				index2=0;
				for (; index2<tx_parameters.N_TX; index2++)
				{
					cmat_template(index2,0)=Q(index)(index2,0)+Q(index)(index2,1)+Q(index)(index2,2);
				}
				Qmeta(index)=cmat_template;
			}
			break;
		case 32:
			for (;index<tx_parameters.Nst;index++)
			{
				cmat_template.set_col(0,Q(index).get_col(0));
				index2=1;
				for (; index2<tx_parameters.N_TX; index2++)
				{
					cmat_template(index2,1)=Q(index)(index2,1)+Q(index)(index2,2);
				}
				Qmeta(index)=cmat_template;
			}
			break;
		case 41:
			for (;index<tx_parameters.Nst;index++)
			{
				index2=0;
				for (; index2<tx_parameters.N_TX; index2++)
				{
					cmat_template(index2,0)=Q(index)(index2,0)+Q(index)(index2,1)+Q(index)(index2,2)+Q(index)(index2,3);
				}
				Qmeta(index)=cmat_template;
			}
			break;
		case 42:
			for (;index<tx_parameters.Nst;index++)
			{
				index2=0;
				for (; index2<tx_parameters.N_TX; index2++)
				{
					cmat_template(index2,0)=Q(index)(index2,0)+Q(index)(index2,1);
					cmat_template(index2,1)=Q(index)(index2,2)+Q(index)(index2,3);
				}
				Qmeta(index)=cmat_template;
			}
			break;
		case 43:
			for (;index<tx_parameters.Nst;index++)
			{
				cmat_template.set_col(0,Q(index).get_col(0));
				index2=1;
				for (; index2<tx_parameters.N_TX; index2++)
				{
					cmat_template(index2,1)=Q(index)(index2,1);
					cmat_template(index2,2)=Q(index)(index2,2)+Q(index)(index2,3);
				}
				Qmeta(index)=cmat_template;
			}
			break;
	}
}

void Precoding_Matrices_11n::format_extension_mapping(Array<cmat>& Qext, txvector const& tx_parameters)
{
	cmat E(tx_parameters.N_TX,tx_parameters.Ness+1);
	
	switch(tx_parameters.N_TX*100+tx_parameters.Ness+1)
	{
		case 202: // Ntx=2, Ness=2
			E="1 0;0 1";
			break;
		case 302: // Ntx=3, Ness=2
			E="1 0;0 1;0 0";
			break;
		case 303: // Ntx=3, Ness=3
			E="1 0 0;0 1 0;0 0 1";
			break;
		case 402: // Ntx=4, Ness=2
			E="1 0;0 1;0 0;0 0";
			break;
		case 403: // Ntx=4, Ness=3
			E="1 0 0;0 1 0;0 0 1;0 0 0";
			break;
		case 404: // Ntx=4, Ness=4
			E="1 0 0 0;0 1 0 0;0 0 1 0;0 0 0 1";
			break;
		default:
			cout << "ERROR: Antenna dimensions, with specified spatial streams do not support number of extension spatial streams!" << endl;
	}

	Qext.set_size(tx_parameters.Nst);
	std::complex<double> iorj(0,1);
	{
		for (unsigned int a=0; a<tx_parameters.Nst; a++)
		{
			Qext(a)=E;
		}
	}
#if USRP_COMPENSATION_FILTER
  for (unsigned int a=0; a<tx_parameters.Nst; a++)
    Qext(a)*=compensation_gain(a);
#endif
}



