/*!  	
 *	\brief Space-time block decoder class for 802.11n
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

#include "space_time_decoding_11n.h"
 
using namespace itpp;
using std::cout;
using std::endl;

Space_Time_Decoding_11n::Space_Time_Decoding_11n(unsigned int num_space_time_streams, unsigned int num_spatial_streams, unsigned int num_subcarriers)
{
	Nsts=num_space_time_streams;
	Nss=num_spatial_streams;
	Nsd=num_subcarriers;
	switch (Nsd)
	{
		case 48:
			subcarrier_index="26 27 28 29 30 32 33 34 35 36 37 38 39 40 41 42 43 44 46 47 48 49 50 51 0 1 2 3 4 5 7 8 9 10 11 12 13 14 15 16 17 18 19 21 22 23 24 25";
			break;
		case 52:
			subcarrier_index="28 29 30 31 32 33 34 36 37 38 39 40 41 42 43 44 45 46 47 48 50 51 52 53 54 55 0 1 2 3 4 5 7 8 9 10 11 12 13 14 15 16 17 18 19 21 22 23 24 25 26 27";
			break;
		case 98:
			subcarrier_index="";
			break;
		case 108:
			subcarrier_index="58 59 60 61 62 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 92 93 94 95 96 97 98 99 100 101 102 103 104 106 107 108 109 110 111 112 113 114 0 1 2 3 4 5 6 7 8 10 11 12 13 14 15 16 17 18 19 20 21 22 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 50 51 52 53 52 55 56 57";
			break;
	}
}

void Space_Time_Decoding_11n::decode(cmat& output_symbol_1, cmat& output_symbol_2, cmat& equalized_symbol_1, cmat& equalized_symbol_2, itpp::Array<cmat> const& channel_matrix)
{
	
	unsigned int Nrx=output_symbol_1.rows();
	if (Nrx<Nsts)
		cout << "ERROR: STBC in 802.11n requires number of receiver antennas >= Nsts!" << endl;
	else
	{
		// decode the STBC encoded symbols
		cvec slot_1, slot_2;
		slot_1.set_size(Nrx);
		slot_2.set_size(Nrx);
		switch (10*Nsts+Nss)
		{
			case 21: // Nsts=2, Nss=1
				{
	
					for (unsigned int i=0; i<Nsd; i++)
					{
						
						for (unsigned int j=0; j<Nrx; j++)
						{
							// no interference cancellation needed
							slot_1(j)=output_symbol_1(j,i);
							slot_2(j)=output_symbol_2(j,i);
						}
						
						// perform alamouti decoding on both STBC streams
						alamouti_decode(slot_1,slot_2,channel_matrix(subcarrier_index(i)));
						
						// set first row to decoded STBC symbols
						equalized_symbol_1(0,i)=slot_1(0);
						equalized_symbol_2(0,i)=slot_2(0);

					}
					
					// resize equalized symbols
					equalized_symbol_1.set_size(1,Nsd,true);
					equalized_symbol_2.set_size(1,Nsd,true);
				
				}
				break;
			case 32: // Nsts=3, Nss=2
				{
					for (unsigned int i=0; i<Nsd; i++)
					{
					
						for (unsigned int j=0; j<Nrx; j++)
						{
							// cancel second spatial stream (third space-time stream)
							slot_1(j)=output_symbol_1(j,i)-equalized_symbol_1(2,i)*channel_matrix(subcarrier_index(i))(j,2);
							slot_2(j)=output_symbol_2(j,i)-equalized_symbol_2(2,i)*channel_matrix(subcarrier_index(i))(j,2);
						}

						// perform alamouti decoding algorithm on STBC stream
						alamouti_decode(slot_1,slot_2,channel_matrix(subcarrier_index(i)).get_cols(0,1));
	
						// set second row to decoded STBC symbols
						equalized_symbol_1(1,i)=slot_1(0);
						equalized_symbol_2(1,i)=slot_2(0);

					}
					
					// resize equalized symbols
					equalized_symbol_1.set_size(2,Nsd,true);
					equalized_symbol_2.set_size(2,Nsd,true);

				}
				break;
			case 42: // Nsts=4, Nss=2
				{
					for (unsigned int i=0; i<Nsd; i++)
					{
				
						for (unsigned int j=0; j<Nrx; j++)
						{
							// cancel third and fourth spatial stream
							slot_1(j)=output_symbol_1(j,i)-equalized_symbol_1(2,i)*channel_matrix(subcarrier_index(i))(j,2)-equalized_symbol_1(3,i)*channel_matrix(subcarrier_index(i))(j,3);
							slot_2(j)=output_symbol_2(j,i)-equalized_symbol_2(2,i)*channel_matrix(subcarrier_index(i))(j,2)-equalized_symbol_2(3,i)*channel_matrix(subcarrier_index(i))(j,3);
						}

						// perform alamouti decoding algorithm on first STBC stream
						alamouti_decode(slot_1,slot_2,channel_matrix(subcarrier_index(i)).get_cols(0,1));
									
						// set first row to decoded STBC symbols
						equalized_symbol_1(0,i)=slot_1(0);
						equalized_symbol_2(0,i)=slot_2(0);

						for (unsigned int j=0; j<Nrx; j++)
						{
							// cancel first and second spatial stream
							slot_1(j)=output_symbol_1(j,i)-equalized_symbol_1(0,i)*channel_matrix(subcarrier_index(i))(j,0)-equalized_symbol_1(1,i)*channel_matrix(subcarrier_index(i))(j,1);
							slot_2(j)=output_symbol_2(j,i)-equalized_symbol_2(0,i)*channel_matrix(subcarrier_index(i))(j,0)-equalized_symbol_2(1,i)*channel_matrix(subcarrier_index(i))(j,1);
						}

						// perform alamouti decoding algorithm on second STBC stream
						alamouti_decode(slot_1,slot_2,channel_matrix(subcarrier_index(i)).get_cols(2,3));

						// set second row to decoded STBC symbols
						equalized_symbol_1(1,i)=slot_1(0);
						equalized_symbol_2(1,i)=slot_2(0);
			
					}
					
					// resize equalized symbols
					equalized_symbol_1.set_size(2,Nsd,true);
					equalized_symbol_2.set_size(2,Nsd,true);

				}
				break;
			case 43: // Nsts=4, Nss=3
				{
					for (unsigned int i=0; i<Nsd; i++)
					{
					
						for (unsigned int j=0; j<Nrx; j++)
						{
							// cancel third and fourth spatial stream
							slot_1(j)=output_symbol_1(j,i)-equalized_symbol_1(2,i)*channel_matrix(subcarrier_index(i))(j,2)-equalized_symbol_1(3,i)*channel_matrix(subcarrier_index(i))(j,3);
							slot_2(j)=output_symbol_2(j,i)-equalized_symbol_2(2,i)*channel_matrix(subcarrier_index(i))(j,2)-equalized_symbol_2(3,i)*channel_matrix(subcarrier_index(i))(j,3);
						}

						// perform alamouti decoding algorithm on STBC stream
						alamouti_decode(slot_1,slot_2,channel_matrix(subcarrier_index(i)).get_cols(0,1));
	
						// set second row to decoded STBC symbols
						equalized_symbol_1(1,i)=slot_1(0);
						equalized_symbol_2(1,i)=slot_2(0);

					}
					
					// resize equalized symbols
					equalized_symbol_1=equalized_symbol_1.get_rows(1,3);//,0,equalized_symbol_1.cols()-1);
					equalized_symbol_2=equalized_symbol_2.get_rows(1,3);//,0,equalized_symbol_2.cols()-1);

				}
				break;
		}
	}
}

void Space_Time_Decoding_11n::alamouti_decode(cvec& sl_1, cvec& sl_2, cmat chan)
{
	unsigned int Nrx=length(sl_1);
	cvec out_1(Nrx);
	cvec out_2(Nrx);
	
	// perform Alamouti decoding algorithm for each rx antenna
	for (unsigned int k=0; k<Nrx; k++)
	{
		out_1(k)=conj(chan(k,0))*sl_1(k)+conj(conj(chan(k,1))*sl_2(k));
		out_2(k)=conj(chan(k,0))*sl_2(k)-conj(conj(chan(k,1))*sl_1(k));
	} 	
	
	// normalize energy for proper detection
	double normalization_constant=sqr(1/norm(chan,"fro"));
	
	// sum the contributions for each RX antenna
	sl_1(0)=sum(out_1)*normalization_constant;
	sl_2(0)=sum(out_2)*normalization_constant;
}

