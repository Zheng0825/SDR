/*!     \file Beamforming_MIMO_OFDM.cc
 * 	\brief Beamforming Vector Class for MIMO-OFDM Systems
 * 	\date Saturday, February 17th, 2007
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
 *
 *	\author{Robert C. Daniels (rdaniels@ece.utexas.edu), Kien T. Truong
 *	(kientruong@ece.utexas.edu)}
 *
 * 	- \date{02/17/07} \note{Robert Daniels: Created BF_MIMO_OFDM.h and
 * 	BF_MIMO_OFDM.cc template}
 *   	- \date{03/11/07} \note{Kien Truong: Created BF_MIMO_OFDM::matrix_SVD()
 *   	and BF_MIMO_OFDM::optimal_bf()}
 *      - \date{03/12/07} \note{Kien Truong: Created BF_MIMO_OFDM::codebooks_gen() and
 *      BF_MIMO_OFDM::limited_bf()}
 *	- \date{03/13/07} \note{Kien Truong: Created
 *	BF_MIMO_OFDM::limited_clustering_bf()}
 *	- \date{05/31/07} \note{Kien Truong: Debugged: correct checking
 *	procedure for codebook index and add suggestions}
 *	- \date{06/25/07} \note{Robert Daniels: Changed the entire structure of the
 *	class to represent the update class header for better organization and
 *	style}	
 */

#include "Beamforming_MIMO_OFDM.h"

using namespace itpp;
using std::cout;
using std::endl;

Beamforming_MIMO_OFDM::Beamforming_MIMO_OFDM()
{
	// error checking false until successful vector creation
	is_vector_valid=false; 
	
	//! \brief Set default parameters as 802.11n 2x2 system
	Ntx=2;Nrx=2;Nsc=56;Nmin=2; // the number of eigenpaths

	transmit_vectors.set_size(Nsc,false);
	receive_vectors.set_size(Nsc,false);
	transmit_clustered_vectors.set_size(Nsc,false);
	receive_clustered_vectors.set_size(Nsc,false);
	transmit_interpolated_vectors.set_size(Nsc,false);
	receive_interpolated_vectors.set_size(Nsc,false);
	cvec init_value=ones_c(Ntx)/sqrt(Ntx);
	for (unsigned int a=0; a<Nsc; a++)
	{
		transmit_vectors(a)=init_value;	
		receive_vectors(a)=init_value;	
		transmit_clustered_vectors(a)=init_value;	
		receive_clustered_vectors(a)=init_value;	
		transmit_interpolated_vectors(a)=init_value;	
		receive_interpolated_vectors(a)=init_value;
	}	

}


void Beamforming_MIMO_OFDM::compute_optimal_vectors(Array<cmat> const& in_chan)
{
	Nrx=in_chan(0).rows();
	Ntx=in_chan(0).cols();
	Nsc=in_chan.size();
	Nmin=std::min(Ntx,Nrx);

	transmit_vectors.set_size(Nsc,false);	
	receive_vectors.set_size(Nsc,false);	

	// check the system dimensions
	switch (Ntx*10+Nrx) 
	{
		case 21: // 2x1
		case 31: // 3x1
		case 41: // 4x1
			// MISO system vector creation below based on
			// maximal ratio transmission
			{
				double H_norm;
				cvec receive_vector_trivial=ones_c(Nrx);
				for (unsigned int a=0; a<Nsc; a++)
				{
					H_norm = norm(in_chan(a));
					transmit_vectors(a) = in_chan(a)/H_norm;
					// Trivial combining vector
					receive_vectors(a) = receive_vector_trivial;
				}
			} 
			break;
		case 22: // 2x2
		case 23: // 2x3
		case 24: // 2x4
		case 32: // 3x2
		case 33: // 3x3
		case 34: // 3x4
		case 42: // 4x2
		case 43: // 4x3
		case 44: // 4x4
			{
				// MIMO vector creation based on the largest eigenpath			
				matrix_SVD(in_chan);
				// indices of maximum eigenvalues
				ivec eg_value_max_index(Nsc); 
				for (unsigned int b=0; b<Nsc; b++) 	
				{	
					eg_value_max_index(b) = max_index(eigenvalues(b));
					transmit_vectors(b).set_size(Nmin,false);
					receive_vectors(b).set_size(Nmin,false);
					for (int c=0; c < Nmin; c++) 
					{ 
						// only Nmin nonzero elements possible
						transmit_vectors(b)(c) = tx_eigenvectors(b)(c,eg_value_max_index(b)); 
						receive_vectors(b)(c) = rx_eigenvectors(b)(c,eg_value_max_index(b));
					}
				}
			} 
			break;
		default:
		{
			cout << "ERROR: System dimensions not supported for optimal beamforming vector creation!" << endl;
			is_vector_valid = false;
			break;
		} 
	}
}

ivec Beamforming_MIMO_OFDM::compute_code_indices(Array<cmat> const& in_chan, unsigned int codebook_index, unsigned int codebook_bits)
{	
	Ntx=in_chan(0).cols();
	Nrx=in_chan(0).rows();
	Nsc=in_chan.size();
	Nmin=std::min(Ntx,Nrx);
	generate_codebook(codebook_index, codebook_bits);

	cvec code_word;
	ivec code_indices=zeros_i(in_chan.size());
	
	vec H_w_norm; // holds ||H(k)w|| for each subcarrier
	unsigned int num_cw=codebook.rows(); // number of code words
	H_w_norm.set_size(num_cw); 
		
	// loop finds the beamforming vector for each subcarrier
	for (unsigned int a=0; a<Nsc; a++) 	
	{
		// loop calculates ||H(k)w|| for each codeword
		for (unsigned int b=0; b<num_cw; b++) 
		{
			code_word = codebook.get_row(b); 
			H_w_norm(b) = norm(in_chan(a)*code_word);
		} 
			
		// find the maximum ||H(k)w|| for each subcarrier
		code_indices(a) = max_index(H_w_norm);					
	} 
	return code_indices;
} 

itpp::ivec Beamforming_MIMO_OFDM::compute_clustered_code_indices(Array<cmat> in_chan, unsigned int codebook_index, unsigned int codebook_bits, unsigned int cluster_size, unsigned int clustering_metric)
{
	Ntx=in_chan(0).cols();
	Nrx=in_chan(0).rows();
	Nsc=in_chan.size();
	Nmin=std::min(Ntx,Nrx);
	generate_codebook(codebook_index, codebook_bits);

	cvec code_word;
	
	vec H_w_norm; // holds ||H(k)w|| for each subcarrier
	unsigned int num_cw=codebook.rows(); // number of code words
	H_w_norm.set_size(num_cw); 
	unsigned int num_clusters=ceil(double(Nsc)/double(cluster_size));
	ivec code_indices=zeros_i(num_clusters);
	switch (clustering_metric)
	{
		case 0: // maximizes total summed SNR in cluster (eq: 3 in Mondal et al)
			
			// loop finds the beamforming vector for each cluster
			for (unsigned int a=0; a<num_clusters; a++) 	
			{
				H_w_norm.clear();
				// loop calculates ||H(k)w|| for each codeword
				for (unsigned int b=0; b<num_cw; b++) 
				{
					code_word = codebook.get_row(b); 
					for (unsigned int c=0; ((c<cluster_size)&&((a*cluster_size+a)<Nsc)); c++)
					{
						H_w_norm(b) += norm(in_chan(a*cluster_size+c)*code_word);
					}
				}
				// find the maximum ||H(k)w|| for each cluster
				code_indices(a) = max_index(H_w_norm);					
			} 

		case 1: // maximizes minimum SNR per subcarrier in cluster (eq: 4 in Mondal et al)
			break;
		default: 
			cout << "ERROR: Clustering metric not yet supported!" << endl;
			break;
	}
	return code_indices;
}

void Beamforming_MIMO_OFDM::compute_limited_feedback_vectors(unsigned int codebook_index, unsigned int codebook_bits, ivec code_indices, unsigned int num_tx, unsigned int num_rx)
{
	Ntx=num_tx; Nrx=num_rx;
	Nmin=std::min(Ntx,Nrx);
	Nsc=length(code_indices);
	generate_codebook(codebook_index, codebook_bits);
	transmit_vectors.set_size(Nsc);

	for (unsigned int a=0; a<Nsc; a++)
	{
		// assign the beamforming vector for the subcarrier
		transmit_vectors(a) = codebook.get_row(code_indices(a));
	}
}

void Beamforming_MIMO_OFDM::compute_limited_feedback_vectors(unsigned int codebook_index, unsigned int codebook_bits, ivec code_indices, Array<cmat> const& H)
{
	Ntx=H(0).cols(); Nrx=H(0).rows();
	Nmin=std::min(Ntx,Nrx);
	Nsc=H.size();
	generate_codebook(codebook_index, codebook_bits);
	transmit_vectors.set_size(Nsc);
	receive_vectors.set_size(Nsc);
	
	for (unsigned int a=0; a<Nsc; a++)
	{
		// assign the beamforming vector for the subcarrier
		transmit_vectors(a) = codebook.get_row(code_indices(a));
		// determine the corresponding vector for the subcarrier
		receive_vectors(a) = (H(a)*transmit_vectors(a))/norm(H(a)*transmit_vectors(a));
	}
}


void Beamforming_MIMO_OFDM::compute_clustered_vectors(unsigned int codebook_index, unsigned int codebook_bits, itpp::ivec code_indices, unsigned int num_tx, unsigned int num_rx, unsigned int num_sc)
{
	Ntx=num_tx; Nrx=num_rx; Nsc=num_sc;
	Nmin=std::min(Ntx,Nrx);
	unsigned int num_clusters=length(code_indices);
	generate_codebook(codebook_index, codebook_bits);
	unsigned int cluster_size=ceil_i(double(Nsc)/double(num_clusters));
	transmit_clustered_vectors.set_size(Nsc,false);

	for (unsigned int a=0; a<num_clusters; a++)
	{
		transmit_vectors(a*cluster_size) = codebook.get_row(code_indices(a));
		for (unsigned int b=1; ((b<cluster_size)&&((a*cluster_size+b)<Nsc)); b++)
		{
			transmit_vectors(a*cluster_size+b)=transmit_vectors(a*cluster_size+b-1);
		}
	}

}

void Beamforming_MIMO_OFDM::compute_clustered_vectors(unsigned int codebook_index, unsigned int codebook_bits, itpp::ivec code_indices, itpp::Array<itpp::cmat> const& in_chan)
{
	Ntx=in_chan(0).cols();
	Nrx=in_chan(0).rows(); 
	Nmin=std::min(Ntx,Nrx);
	Nsc=in_chan.size();
	unsigned int num_clusters=length(code_indices);
	generate_codebook(codebook_index, codebook_bits);
	unsigned int cluster_size=ceil_i(double(Nsc)/double(num_clusters));
	transmit_clustered_vectors.set_size(Nsc,false);
	receive_clustered_vectors.set_size(Nsc,false);

	for (unsigned int a=0; a<num_clusters; a++)
	{
		transmit_vectors(a*cluster_size) = codebook.get_row(code_indices(a));
		receive_vectors(a*cluster_size) = (in_chan(a*cluster_size)*transmit_vectors(a*cluster_size));
		receive_vectors(a*cluster_size) /= norm(receive_vectors(a*cluster_size));
		for (unsigned int b=1; ((b<cluster_size)&&((a*cluster_size+b)<Nsc)); b++)
		{
			transmit_vectors(a*cluster_size+b)=transmit_vectors(a*cluster_size+b-1);
			receive_vectors(a*cluster_size+b)=receive_vectors(a*cluster_size+b-1);
		}
	}

}



void Beamforming_MIMO_OFDM::compute_interpolated_vectors(unsigned int interpolation_size)
{
	cout << "ERROR: Interpolated vectors not yet supported!" << endl;
	is_vector_valid=false;
}


void Beamforming_MIMO_OFDM::generate_codebook(unsigned int codebook_index, unsigned int codebook_bits)
{
	// Grassmanian codebooks
	if (codebook_index==0)
	{
		// Codebook construction depends on the system dimensions
		switch (Ntx*10+codebook_bits)
		{ 
			// 2 transmit antennas and 2 bit codebook
			case 22:
				codebook.set("-0.1612-0.7348i -0.5135-0.4128i; -0.0787-0.3192i -0.2506+0.9106i; -0.2399+0.5985i -0.7641-0.0212i; -0.9541 0.2996");
				break;
			// 2 transmit antennas and 3 bit codebook
			case 23: 
				codebook.set("0.8393-0.2939i -0.1677+0.4256i; -0.3427+0.9161i 0.0498+0.2019i; -0.2065+0.3371i 0.9166+0.0600i; 0.3478-0.3351i 0.2584+0.8366i; 0.1049+0.6820i 0.6537+0.3106i; 0.0347-0.2716i 0.0935-0.9572i;-0.7457+0.1181i -0.4553-0.4719i; -0.7983+0.3232i 0.5000+0.0906i");
				break;
			// 2 transmit antennas and 4 bit codebook
			case 24:
				codebook.set("-0.6865 0.4857+0.5412i;-0.8034 -0.0913+0.5885i;-0.5655 -0.1019-0.8184i;-0.7125 -0.6074-0.3514i;-0.8255 -0.1831-0.5338i;-0.4803 -0.0391+0.8762i;-0.5957 0.5508-0.5846i;-0.7995 -0.5835+0.1426i;-0.8478 0.2917-0.4429i;-0.8173 0.5748-0.0404i;-0.9794 -0.1997-0.0288i;-0.6328 -0.5940+0.4968i;0.1918 -0.9757-0.1055i;-0.3683 -0.9059-0.2090i;-0.6089 0.7893+0.0789i;-0.9545 0.2115+0.2101i");
				break;
			// 2 transmit antennas and 5 bit codebook
			case 25:
				codebook.set("-0.7846 -0.4317-0.4450i;-0.7583 0.6461-0.0871i;-0.6386 -0.2088-0.7407i;-0.7104 0.1794-0.6806i;-0.4655 0.3752-0.8016i;-0.8872 -0.0554-0.4580i;-0.4411 -0.8910+0.1075i;-0.5470 0.7958+0.2597i;-0.8714 0.3255-0.3671i;-0.8849 -0.4046+0.2307i;-0.6897 -0.6735+0.2659i;-0.5742 0.7735-0.2684i;-0.3000 -0.4672-0.8317i;-0.7090 0.4990-0.4983i;-0.8691 0.2619+0.4195i;-0.7627 0.5739+0.2982i;-0.8360 -0.1028+0.5390i;-0.4772 -0.4081+0.7783i;-0.6968 -0.4915+0.5224i;-0.9209 0.3863+0.0520i;-0.5777 -0.6267-0.5229i;-0.6617 0.1089+0.7419i;-0.6876 0.4117+0.5981i;-0.9682 -0.0324+0.2480i;-0.1479 -0.4838+0.8626i;-0.9923 0.0517-0.1122i;-0.2295 0.9363-0.2657i;-0.4283 0.4065+0.8071i;-0.9387 -0.3059-0.1592i;-0.6696 -0.7343-0.1119i;-0.8193 -0.5652-0.0968i;-0.6779 -0.1948+0.7088i");
				break;
			// 3 transmit antennas and 2 bit codebook
			case 32:
				codebook.set("0.7939+0.0590i -0.4126-0.0807i -0.0853-0.4269i; 0.2189+0.0654i 0.1844-0.3191i -0.8804-0.1921i; 0.3087-0.4341i -0.2454-0.6507i 0.4817+0.0258i; 0.5915-0.1175i 0.3113+0.6635i 0.2941+0.1128i");
				break;
			// 3 transmit antennas and 3 bit codebook
			case 33:
				codebook.set("0.4170+0.0350i 0.2583+0.5635i 0.4826+0.4558i; -0.5521+0.2276i 0.2496+0.4156i -0.1060-0.6301i; 0.2148+0.0059i -0.4115-0.8788i -0.0625-0.0911i; 0.1814+0.3412i -0.7407+0.0188i 0.5024+0.2221i; 0.9126+0.1634i -0.2865+0.0983i -0.1510+0.1608i; 0.4883-0.5125i 0.4955-0.1385i -0.4266+0.2284i; 0.2020-0.4667i -0.1941+0.2372i -0.7891-0.1576i; 0.4294-0.1103i 0.1998-0.5339i 0.6693+0.1747i");
				break;
	
			// 3 transmit antennas and 4 bit codebook
			case 34:
				codebook.set("0.7274+0.3026i -0.1659+0.4141i -0.4227+0.0405i; 0.4118-0.2298i -0.4077+0.2244i 0.7207+0.2040i; 0.1600-0.0152i -0.0562-0.1872i 0.9478-0.1939i; 0.5793-0.3144i -0.2044+0.1672i 0.0451-0.7027i; 0.0562+0.2690i -0.6267+0.1016i -0.6783+0.2478i; 0.3820-0.1753i -0.5300-0.0226i -0.5920-0.4375i; -0.3962-0.1652i  -0.7409-0.1551i  -0.3035+0.3882i; -0.0021+0.5972i  0.6062-0.4902i  0.1047-0.1568i;  0.2134-0.8091i  0.1887-0.2612i -0.3483-0.2734i;  0.2383+0.4676i 0.0177+0.2231i -0.8036-0.1698i;  -0.7293-0.0393i -0.2017+0.1928i  0.4653+0.4150i; 0.1396+0.3488i 0.5725+0.2910i 0.0246+0.6676i; 0.0857+0.2018i  0.4897+0.2938i -0.6664-0.4264i;  0.8821+0.2361i  0.3456-0.0126i -0.1816+0.1166i; 0.3873-0.1979i -0.2215+0.8061i  0.3050+0.1375i; 0.5116+0.1282i 0.5332+0.6481i 0.0425+0.1255i");
				break;
			// 4 transmit antennas and 4 bit codebook 
			case 44:
				codebook.set("-0.2832-0.3026i -0.4318+0.2306i -0.4143-0.0092i 0.6457; 0.7715 -0.1213+0.1684i 0.5813+0.0449i -0.1475-0.0049i; 0.0490+0.2645i 0.3039+0.1429i 0.1790+0.5045i 0.7268; 0.6851 0.1861+0.2946i -0.3828-0.2366i -0.1392+0.4328i; 0.5480-0.0557i  0.3246-0.1762i 0.0337-0.4986i 0.5572; 0.7318 -0.4010-0.3151i -0.3431-0.0759i -0.2387-0.1545i; -0.2314-0.2726i -0.0087-0.0200i 0.4820-0.3299i 0.7283; -0.2075-0.2030i -0.0327+0.0432i 0.8885 -0.3461+0.0595i; 0.0573+0.3799i -0.1914-0.5340i 0.6762 0.2247+0.1510i; -0.0681+0.1830i -0.0628-0.6066i -0.1660+0.0868i 0.7449; 0.6596 -0.3825+0.1394i -0.0604+0.1930i 0.5696+0.1838i; 0.6576 0.3519+0.2595i -0.2176+0.0574i -0.1051-0.5610i; 0.3039+0.5663i 0.6450 -0.3047+0.2050i 0.0075+0.1896i; 0.1730+0.0383i 0.6533 -0.1297-0.6396i -0.2886-0.1806i; -0.1293-0.0810i 0.7675 -0.3124-0.0060i 0.4108-0.3483i; 0.1200-0.3160i 0.7791 0.5003+0.1412i 0.0638+0.0668i");
				break;
			// 4 transmit antennas and 6 bit codebook
			case 46:
				codebook.set("0.5000 0.5000 0.5000 0.5000; -0.4619+0.1913i  0.0000+0.5000i -0.5000+0.0000i 0.4976+0.0490i; 0.3536-0.3536i -0.5000+0.0000i  0.5000-0.0000i 0.4904+0.0975i; -0.1913+0.4619i -0.0000-0.5000i -0.5000+0.0000i 0.4785+0.1451i; -0.0000-0.5000i 0.5000-0.0000i  0.5000-0.0000i 0.4619+0.1913i; 0.1913+0.4619i 0.0000+0.5000i -0.5000+0.0000i 0.4410+0.2357i; -0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.4157+0.2778i; 0.4619+0.1913i -0.0000-0.5000i -0.5000+0.0000i 0.3865+0.3172i; -0.5000+0.0000i 0.5000-0.0000i  0.5000-0.0000i 0.3536+0.3536i; 0.4619-0.1913i 0.0000+0.5000i -0.5000+0.0000i 0.3172+0.3865i; -0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.2778+0.4157i; 0.1913-0.4619i -0.0000-0.5000i -0.5000+0.0000i 0.2357+0.4410i;-0.0000+0.5000i 0.5000-0.0000i 0.5000-0.0000i 0.1913+0.4619i; -0.1913-0.4619i -0.0000+0.5000i -0.5000-0.0000i 0.1451+0.4785i; 0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.0975+0.4904i; -0.4619-0.1913i -0.0000-0.5000i -0.5000+0.0000i 0.0490+0.4976i; 0.5000-0.0000i 0.5000-0.0000i 0.5000-0.0000i 0.0000+0.5000i; -0.4619+0.1913i -0.0000+0.5000i -0.5000-0.0000i -0.0490+0.4976i; 0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.0975+0.4904i; -0.1913+0.4619i -0.0000-0.5000i -0.5000+0.0000i -0.1451+0.4785i; 0.0000-0.5000i 0.5000-0.0000i 0.5000-0.0000i -0.1913+0.4619i; 0.1913+0.4619i -0.0000+0.5000i -0.5000-0.0000i -0.2357+0.4410i; -0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.2778+0.4157i; 0.4619+0.1913i -0.0000-0.5000i -0.5000+0.0000i -0.3172+0.3865i; -0.5000-0.0000i 0.5000-0.0000i 0.5000-0.0000i -0.3536+0.3536i; 0.4619-0.1913i -0.0000+0.5000i -0.5000-0.0000i -0.3865+0.3172i; -0.3536+0.3536i -0.5000-0.0000i 0.5000+0.0000i -0.4157+0.2778i; 0.1913-0.4619i -0.0000-0.5000i -0.5000+0.0000i -0.4410+0.2357i; 0.0000+0.5000i 0.5000-0.0000i 0.5000-0.0000i -0.4619+0.1913i; -0.1913-0.4619i -0.0000+0.5000i -0.5000-0.0000i -0.4785+0.1451i; 0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.4904+0.0975i; -0.4619-0.1913i -0.0000-0.5000i -0.5000+0.0000i -0.4976+0.0490i; 0.5000-0.0000i 0.5000-0.0000i 0.5000-0.0000i -0.5000+0.0000i; -0.4619+0.1913i 0.0000+0.5000i -0.5000+0.0000i -0.4976-0.0490i; 0.3536-0.3536i -0.5000-0.0000i 0.5000+0.0000i -0.4904-0.0975i; -0.1913+0.4619i -0.0000-0.5000i -0.5000+0.0000i -0.4785-0.1451i; 0.0000-0.5000i 0.5000-0.0000i 0.5000-0.0000i -0.4619-0.1913i; 0.1913+0.4619i 0.0000+0.5000i -0.5000+0.0000i -0.4410-0.2357i;-0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.4157-0.2778i; 0.4619+0.1913i -0.0000-0.5000i -0.5000+0.0000i -0.3865-0.3172i; -0.5000+0.0000i 0.5000-0.0000i 0.5000-0.0000i -0.3536-0.3536i; 0.4619-0.1913i 0.0000+0.5000i -0.5000+0.0000i -0.3172-0.3865i; -0.3536+0.3536i -0.5000-0.0000i 0.5000+0.0000i -0.2778-0.4157i; 0.1913-0.4619i -0.0000-0.5000i -0.5000+0.0000i -0.2357-0.4410i; 0.0000+0.5000i 0.5000-0.0000i 0.5000-0.0000i -0.1913-0.4619i; 0.1913-0.4619i 0.0000+0.5000i -0.5000+0.0000i -0.1451-0.4785i; 0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.0975-0.4904i; -0.4619-0.1913i 0.0000-0.5000i -0.5000-0.0000i -0.0490-0.4976i; 0.5000+0.0000i 0.5000-0.0000i 0.5000-0.0000i -0.0000-0.5000i; -0.4619+0.1913i 0.0000+0.5000i -0.5000+0.0000i 0.0490-0.4976i; 0.3536-0.3536i -0.5000-0.0000i 0.5000+0.0000i 0.0975-0.4904i; -0.1913+0.4619i -0.0000-0.5000i -0.5000+0.0000i 0.1451-0.4785i; -0.0000-0.5000i 0.5000+0.0000i 0.5000+0.0000i 0.1913-0.4619i; 0.1913+0.4619i 0.0000+0.5000i -0.5000+0.0000i 0.2357-0.4410i; -0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.2778-0.4157i; 0.4619+0.1913i 0.0000-0.5000i -0.5000-0.0000i 0.3172-0.3865i; -0.5000+0.0000i 0.5000-0.0000i 0.5000-0.0000i 0.3536-0.3536i; 0.4619-0.1913i 0.0000+0.5000i -0.5000+0.0000i 0.3865-0.3172i;-0.3536+0.3536i -0.5000-0.0000i 0.5000+0.0000i 0.4157-0.2778i; 0.1913-0.4619i -0.0000-0.5000i -0.5000+0.0000i 0.4410-0.2357i; -0.0000+0.5000i 0.5000-0.0000i 0.5000-0.0000i 0.4619-0.1913i; -0.1913-0.4619i 0.0000+0.5000i -0.5000+0.0000i 0.4785-0.1451i; 0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.4904-0.0975i; -0.4619-0.1913i 0.0000-0.5000i -0.5000-0.0000i 0.4976-0.0490i");
				break;
			default:
				cout << "ERROR: Codebook dimensions not currently supported!" << endl;
		}
	}
}



void Beamforming_MIMO_OFDM::matrix_SVD(Array<cmat> const& H)
{
	
	// Reserve memory for eigenvalues, eigenvectors
	eigenvalues.set_size(Nsc); 		
	tx_eigenvectors.set_size(Nsc);	
	rx_eigenvectors.set_size(Nsc);		
	
	// initialize eigenvectors
	for (unsigned int a=0; a<Nsc; a++)
	{	 
		eigenvalues(a) = ones(Nmin);  
		tx_eigenvectors(a) = ones_c(Ntx,Nmin); 
		rx_eigenvectors(a) = ones_c(Nrx,Nmin); 
	}
	
	bool svd_test; is_vector_valid=true;
	// compute singular value decompostition for each subcarrier 
	for (unsigned int b=0; b<Nsc; b++)
	{
		svd_test = svd(H(b),rx_eigenvectors(b),eigenvalues(b),tx_eigenvectors(b));
		if (!svd_test)
			is_vector_valid=false;
	}
}

