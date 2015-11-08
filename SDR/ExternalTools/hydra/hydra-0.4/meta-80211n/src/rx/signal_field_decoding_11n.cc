/*  	
 * Title:  Signal field decoding (SIG)
 * Created By: Robert Daniels
 * Creation Date: 
 * 
 * Description: Signal field decoding for 802.11n preambles
 *
 * Revision: v0.00 - - Initial Release (only supports Greenfield operation under 20 MHz channels)
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

#include "signal_field_decoding_11n.h"
#include "soft_bits_decoding_11n.h"
#include "itpp/itcomm.h"
#include "deinterleaving_11n.h"

using namespace itpp;
using std::cout;
using std::endl;

Signal_Field_Decoding_11n::Signal_Field_Decoding_11n()
{
	htsig1="0";htsig2="0";
	subcarrier_mapping="1 2 3 4 5 6 8 9 10 11 12 13 14 15 16 17 18 19 20 22 23 24 25 26 38 39 40 41 42 44 45 46 47 48 49 50 51 52 53 54 55 56 58 59 60 61 62 63";
	p="1 1 0";p=p*2-1;
}

bvec Signal_Field_Decoding_11n::htsig_egc(cmat signal_block, cmat chan_dat, double N0)
{
// High-Throughput signal field decode for 1 receive antenna
// sig_block_tx? = 2*80(or 160) symbols containing HTSIG1 and HTSIG2
// chan_dat = 48() data carrying subcarrier channel gains
// N0 = noise power (one-sided)
// returns bits in HTSIG1 and HTSIG2 w/o CRC or TAIL
// if CRC check fails, "0" bit returned
	
	BPSK_c demap;
	Soft_Bits_Decoding_11n code(0);
	Deinterleaving_11n dlv(3,16,1);
	int Ntone=52;
	int Nrx=signal_block.rows();
	{
		if (signal_block.cols()==240&&chan_dat.cols()==64)
		{
		
			cmat htsig1mat=zeros_c(Nrx,64);cmat htsig2mat=htsig1mat;cmat htsig3mat=htsig2mat;
			htsig1mat=signal_block.get_cols(16,79);
			htsig2mat=signal_block.get_cols(96,159);
			htsig3mat=signal_block.get_cols(176,239);

			for (unsigned int row_index=0; row_index<Nrx; row_index++)
			{
				htsig1mat.set_row(row_index,(1/(sqrt(Ntone)))*fft(htsig1mat.get_row(row_index),64));
				htsig2mat.set_row(row_index,(1/(sqrt(Ntone)))*fft(htsig2mat.get_row(row_index),64));
				htsig3mat.set_row(row_index,(1/(sqrt(Ntone)))*fft(htsig3mat.get_row(row_index),64));
			}
			for (unsigned int sub_index=0; sub_index<48;sub_index++)
			{
				htsig1mat.set_submatrix(0,sub_index,(ls_solve_od(chan_dat.get(0,-1,subcarrier_mapping(sub_index),subcarrier_mapping(sub_index)),htsig1mat.get_col(subcarrier_mapping(sub_index)))));
				htsig2mat.set_submatrix(0,sub_index,(ls_solve_od(chan_dat.get(0,-1,subcarrier_mapping(sub_index),subcarrier_mapping(sub_index)),htsig2mat.get_col(subcarrier_mapping(sub_index)))));	
				htsig3mat.set_submatrix(0,sub_index,(ls_solve_od(chan_dat.get(0,-1,subcarrier_mapping(sub_index),subcarrier_mapping(sub_index)),htsig3mat.get_col(subcarrier_mapping(sub_index)))));	
			}
			htsig1mat.set_size(1,48,true);
			htsig2mat.set_size(1,48,true);
			htsig3mat.set_size(1,48,true);
			// rearrange in correct order
			for (unsigned int ttt=0; ttt<24; ttt++)
			{
				htsig1mat.swap_cols(ttt,ttt+24);
				htsig2mat.swap_cols(ttt,ttt+24);
				htsig3mat.swap_cols(ttt,ttt+24);
			}
			cvec htsig1=htsig1mat.get_row(0);
			cvec htsig2=htsig2mat.get_row(0);
			cvec htsig3=htsig3mat.get_row(0);
			
			// soft demodulate
			vec soft_bits_1, soft_bits_2, soft_bits_3;
			std::complex<double>  iorj(0,1); // for phase de-rotation of constellation
			demap.demodulate_soft_bits(-1*iorj*htsig1,N0,soft_bits_1);
			demap.demodulate_soft_bits(-1*iorj*htsig2,N0,soft_bits_2);
			demap.demodulate_soft_bits(-1*iorj*htsig3,N0,soft_bits_3);
			mat soft_mat_1, soft_mat_2, soft_mat_3;
			soft_mat_1.set_size(1,length(soft_bits_1));
			soft_mat_1.set_row(0,soft_bits_1);
			soft_mat_2.set_size(1,length(soft_bits_2));
			soft_mat_2.set_row(0,soft_bits_2);
			soft_mat_3.set_size(1,length(soft_bits_3));
			soft_mat_3.set_row(0,soft_bits_3);

			// de-interleave
			dlv.deinterleave(soft_mat_1);
			dlv.deinterleave(soft_mat_2);
			dlv.deinterleave(soft_mat_3);
			
			// Viterbi decoding
			bvec sigbits=code.decode(0,concat(soft_mat_1.get_row(0),soft_mat_2.get_row(0),soft_mat_3.get_row(0)));
			
			// if CRC check passes, return bits, else return "0"
			CRC_Code crc_code("CCITT-16");
			if (crc_code.check_parity(sigbits))
			{
				return sigbits(0,49);
			}
			else
			{
				return "0";
			}
		}
		else 
		{
			cout << "Lengths: " << signal_block.cols() << "/" << chan_dat.cols() << endl;

			cout << "Error: Signal decode input vectors have improper size!" << endl;
			return "0";
		}
	}
}
 
