/*   	
 * Title:  Signal field creation for 802.11n
 * Created By: Robert Daniels
 * Creation Date: 1/02/2007
 * 
 * Description: Creation of creation (HT and legacy)
 * Revision: v0.00 - 1/04/2007 - Initial Release only supports 20MHz HT-GF-STF 
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

#include "signal_field_formatting_11n.h"
#include "channel_encoding_11n.h"
#include "interleaving_11n.h"
#include <itpp/comm/modulator.h>
#include <itpp/comm/crc.h>

using namespace itpp;
using std::cout;
using std::endl;

Signal_Field_Formatting_11n::Signal_Field_Formatting_11n(txvector const& tx_parameters)
{
	Ntx=tx_parameters.N_TX;
	Tcs="0 0 0 0"; // cyclic shift time in negative ns/50
	switch (Ntx)
	{
		case 1:
			Tcs="0"; // cyclic shift time in negative ns/50
			break;
		case 2:
			Tcs="0 4"; // cyclic shift time in negative ns/50
			break;
		case 3:
			Tcs="0 2 4"; // cyclic shift time in negative ns/50
			break;
		case 4:
			Tcs="0 1 2 3"; // cyclic shift time in negative ns/50
			break;
	}
	
	sig_matrix.set_size(Ntx,240);
}

void Signal_Field_Formatting_11n::ht(txvector const& tx_parameters, cmat& tx_preamble, Array<cmat> const& Q, unsigned int& column_index)
{ 

	// construct HT signal fields from components (21.3.9.4.3 and fig n55)
	
	// construct HT-SIG1
	bvec MCS_meta=reverse(dec2bin(7,int(tx_parameters.MCS_meta))); // MCS field (LSB first)
	bvec LENGTH_meta=reverse(dec2bin(12,int(tx_parameters.LENGTH_meta))); // LENGTH field (LSB first)
	HTSIG1=concat(MCS_meta,LENGTH_meta);

	// construct HT-SIG2
	bvec MCS=reverse(dec2bin(7,int(tx_parameters.MCS))); // MCS field (LSB first)
	bvec LENGTH=reverse(dec2bin(16,int(tx_parameters.LENGTH))); // LENGTH field (LSB first)
	HTSIG2=concat(MCS,LENGTH);
	
	// construct HT-SIG3
	bvec SMOOTH="0";
	if (tx_parameters.SMOOTHING==0) // smoothing recommended
		SMOOTH="1";
	bvec SOUND="1";
	if (tx_parameters.NOT_SOUNDING==0) // sounding packet
		SOUND="0";
	bvec AGG="0";
	if (tx_parameters.AGGREGATION==0) // A-MPDU aggregation
		AGG="1";
	bvec STBC=reverse(dec2bin(2,int(tx_parameters.STBC))); // STBC field (LSB first)
	bvec LDPC="0";
	if (tx_parameters.LDPC_CODING==1)
		LDPC="1";
	bvec NESS=reverse(dec2bin(2,int(tx_parameters.NUM_EXTEN_SS)));
	HTSIG3=concat(SMOOTH,SOUND,AGG,STBC);
	HTSIG3=concat(HTSIG3,LDPC,NESS);
	// calculate CRC
	CRC_Code crc_code("CCITT-16");
	bvec CRC=crc_code.encode(concat(HTSIG1,HTSIG2,HTSIG3));
	CRC.set_size(66,true);
	CRC=concat(CRC,zeros_b(6));

	// code, interleave, modulate signal fields
	Channel_Encoding_11n code(0); // initialize error coding
	Interleaving_11n ilv(3,16,1,0); // initialize interleaving
	BPSK_c map; // BPSK used for header
	code.encode(0,CRC,0); // code HT signal field with rate 1/2
	CRC.set_size(144,true);
	ilv.interleave(CRC,0); // interleave HT signal field
	cvec CTEMP=map.modulate_bits(CRC);
	for (unsigned int bit_temp=0; bit_temp<144; bit_temp++)
		tx_preamble(0,column_index+bit_temp)=CTEMP(bit_temp);
	
	generate_20mhz_greenfield(tx_preamble,Q,column_index);	
}	

void Signal_Field_Formatting_11n::generate_20mhz_greenfield(cmat& tx_preamble, Array<cmat> const& Q, unsigned int& column_index)
{ 
Ntone=52;
	Nfft=64;
	Q_tmp=Q(0);
	unsigned int Ntx=Q_tmp.rows();
	unsigned int Nsts=Q_tmp.cols();
	sig_matrix=zeros_c(Ntx,240);
	std::complex<double>  iorj(0,1);
	ivec P="1 0 1 1";P-="0 1 0 0"; // pilot sequence (17.3.5.9)
	ivec p="1 1 0"; // polarity of pilots for SIGNAL field (17.3.5.9) with n=2,3,4
	p=p*2-1; 
	ivec PHTLTF="1 1 1 0";PHTLTF=PHTLTF*2-1; // space-time stream polarity vector

	ivec data_index="0 1 2 3 4 5 7 8 9 10 11 12 13 14 15 16 17 18 19 21 22 23 24 25 30 31 32 33 34 36 37 38 39 40 41 42 43 44 45 46 47 48 50 51 52 53 54 55"; // spatial mapping index
	ivec data_domain="24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23"; // index of data in tx_preamble
	ivec data_range="1 2 3 4 5 6 8 9 10 11 12 13 14 15 16 17 18 19 20 22 23 24 25 26 38 39 40 41 42 44 45 46 47 48 49 50 51 52 53 54 55 56 58 59 60 61 62 63"; // mapping of data into fft
	ivec pilot_index="6 20 35 49"; // spatial mapping index of pilots
	ivec pilot_range="7 21 43 57"; // mapping of pilots into fft

	// add cyclic delay diversity and spatial mapping
	{
		for (unsigned int sig_index=0; sig_index<3; sig_index++)
		{
			for (unsigned int i=0; i<Ntx; i++)
			{	
				
				{
					for (unsigned int j=0; j<48; j++)
					{	
						Q_tmp=Q(data_index(j));
						for (unsigned int m=0; m<Nsts; m++)
						{
							sig_matrix(i,sig_index*80+16+data_range(j))+=iorj*tx_preamble(0,column_index+sig_index*48+data_domain(j))*PHTLTF(m)*Q_tmp(i,m); // 				
						}
					}
				}
				{
					for (unsigned int j=0; j<4; j++)
					{	
						Q_tmp=Q(pilot_index(j));
						for (unsigned int m=0; m<Nsts; m++)
						{

							sig_matrix(i,sig_index*80+16+pilot_range(j))+=P(j)*p(sig_index)*PHTLTF(m)*Q_tmp(i,m); // 				
						}
					}
				}
			}
		}	
	}

	// take IFFT1
	cvec ifft_vector;
	{
		for (unsigned int i=0;i<Ntx;i++)
		{
			ifft_vector=sig_matrix.get_row(i);
			ifft_vector.replace_mid(16,(Nfft/sqrt(Ntone*Nsts))*ifft(ifft_vector(16,79),Nfft));
			ifft_vector.replace_mid(96,(Nfft/sqrt(Ntone*Nsts))*ifft(ifft_vector(96,159),Nfft));
			ifft_vector.replace_mid(176,(Nfft/sqrt(Ntone*Nsts))*ifft(ifft_vector(176,239),Nfft));
			sig_matrix.set_row(i,ifft_vector);
		}
	}
			
	// add cyclic prefix
	{
		for (unsigned int i=0; i<16; i++)
		{
			sig_matrix.copy_col(i,i+64);
			sig_matrix.copy_col(i+80,i+144);
			sig_matrix.copy_col(i+160,i+224);
		}
	}

	// assign to the preamble
	tx_preamble.set_submatrix(0,column_index,sig_matrix);

	// update column index
	column_index+=(3*(Nfft+Nfft/4));
}
