/*  	
 * Title:  Demodulation (DEM)
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Class definition for qam-symbol demodulation into soft bits in 802.11n
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

#include "soft_demapping_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Soft_Demapping_11n::Soft_Demapping_11n(unsigned int Nbpsc, unsigned int Ncbps, bool is_ML, Array<cmat> H) : ND_UQAM(1,4)
{
	// variables from call
	channels=H;
	Nst=H.size();Nss=H(0).cols();Nrx=H(0).rows();
	isML=is_ML;
	bitcols=Ncbps/Nss;
	M=pow2i(Nbpsc)*ones_i(Nss);
	logM=int(Nbpsc)*ones_i(Nss);
	// for QAM set order
	if (Nbpsc>1)
		ND_UQAM::set_M(Nss,M);
	// define subcarrier mapping
	switch (Nst)
	{
		case 52:
			submap="26 27 28 29 30 32 33 34 35 36 37 38 39 40 41 42 43 44 46 47 48 49 50 51 0 1 2 3 4 5 7 8 9 10 11 12 13 14 15 16 17 18 19 21 22 23 24 25";
			break;
		case 56:
			submap="28 29 30 31 32 33 34 36 37 38 39 40 41 42 43 44 45 46 47 48 50 51 52 53 54 55 0 1 2 3 4 5 7 8 9 10 11 12 13 14 15 16 17 18 19 21 22 23 24 25 26 27";
			break;
		case 114:
			//submap="59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 104 105 106 107 108 109 110 111 112 113 114 2 3 4 5 6 7 8 9 10 12 13 14 15 16 17 18 19 20 21 22 23 24 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 54 55 56 57 58";
			break;
	}
	// initialize variables
	return_mat.set_size(Nss,Ncbps/Nss);
	soft_bits_row.set_size(Ncbps/Nss);
	
}


mat Soft_Demapping_11n::demodulate_soft_bits(unsigned int Nbpsc, cmat const& symbol, cmat const& ref_symbol, mat& post_process_snr, double N0)
{
	QLLRvec apriori_llr(sum(logM));
	QLLRvec apost_llr(sum(logM));
	LLR_calc_unit llrcalc;
	int Nsd=symbol.cols();
	mat llrmat(sum(logM),Nsd);
	if (isML&&(Nss>1)) // maximum likelihood MIMO
	{	
		cmat chanmat(Nrx,Nss);cvec datacol(Nss);
		apriori_llr.clear();
		for (unsigned int a=0; a<Nsd; a++)
		{
			datacol=ref_symbol.get_col(a);
			chanmat=channels(submap(a));
			ND_UQAM::demodulate_soft_bits(datacol,chanmat,N0,apriori_llr,apost_llr,FULL_ENUM_LOGMAP);
			llrmat.set_col(a,llrcalc.to_double(apost_llr));
		}
	}
	else // post-processing SNR per stream ML
	{
		cvec chancol(Nss);cvec datacol(Nss);
		apriori_llr.clear();
		if (Nbpsc==1) // BPSK
		{	
			ND_UPSK bcon(Nss,M(0));
			for (unsigned int a=0; a<Nsd; a++)
			{
				datacol=symbol.get_col(a);
				chancol=to_cvec(sqrt(post_process_snr.get_col(a)));
				datacol=elem_mult(datacol,chancol);
				bcon.demodulate_soft_bits(datacol,chancol,double(1),apriori_llr,apost_llr);
				llrmat.set_col(a,llrcalc.to_double(apost_llr));
			}	
		}
		else // square QAM
		{
			for (unsigned int b=0; b<Nsd; b++)
			{
				datacol=symbol.get_col(b);
				chancol=to_cvec(sqrt(post_process_snr.get_col(b)));
				datacol=elem_mult(datacol,chancol);
				ND_UQAM::demodulate_soft_bits(datacol,chancol,double(1),apriori_llr,apost_llr);
				llrmat.set_col(b,llrcalc.to_double(apost_llr));
			}	
		}
	}	
	// write LLR in proper matrix fcrmat for viterbi
	return_mat.set_size(Nss,bitcols);
	for (int idxa=0; idxa<Nss; idxa++)
	{
		for (int idxb=0; idxb<Nsd; idxb++)
		{
			for (int idxc=0; idxc<logM(idxa); idxc++)
			{
				if (idxa==0)
					return_mat(idxa,idxb*logM(idxa)+idxc)=llrmat(idxc,idxb);
				else
					return_mat(idxa,idxb*logM(idxa)+idxc)=llrmat(sum(logM(0,idxa-1))+idxc,idxb);
			}
		}
	}
	
	//cout << "return_mat_new=[" << return_mat << "];" << endl;
	return return_mat;
}
	
