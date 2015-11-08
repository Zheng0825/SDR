/*   	
 * Title:  TXVector Initialization and Checking
 * Created By: Robert Daniels
 * Creation Date: 01/09/2007
 * 
 * Description: TXVector definition for MAC/PHY interface
 * 
 * Revision: v0.00 - 01/09/2007 - Initial Release (does not contain checking)
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

#include "txvector.h"

using namespace itpp;
using std::cout;
using std::endl;

txvector::txvector() // default values
{
	// intialize standard variables
	SERVICE="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
	TXPWR_LEVEL=8;
	MCS=0;
	LENGTH=0;
	LENGTH_meta=0;
	SMOOTHING=SMOOTHING_NOT_REC;
	NOT_SOUNDING=CONST_NOT_SOUNDING;
	AGGREGATION=NOT_AGGREGATED;
	STBC=0;
	STBC_meta=0;
	LDPC_CODING=BCC_CODING;
	NUM_EXTEN_SS=0;
	ANTENNA_SET_ON=NO_ANTENNA_SET_ON;
	ANTENNA_SET="1 1 0 0 0 0 0 0";
	N_TX=2;
	EXPANSION_MAT_TYPE=NO_EXPANSION_MAT;
	EXPANSION_MAT.set_size(56);

	// protected variable initialization
	is_valid=true;
	Nsts=1;Nsts_meta=1;Nss=1;Nss_meta=1;Ntx=1;mod_index=QAM_2;code_rate=R_1_2;
	Nbpsc=1;Nbpsc_meta=1;Nsd=52;Nsp=4;
	Ncbps=52;Ncbps_meta=52;Ndbps=26;Ndbps_meta=26;Nes=1;Nes_meta=1;
	Ndltf=0;Neltf=0;Nfft=64;Ngi=16;Nnull=8;Nst=56;
	Nofdm=80;Ness=0;Npresamples=480;Nmetasamples=0;Ndatasamples=0;
	Ndatacoded=0;Nmetacoded=0;
}

void txvector::calculate_parameters(bool check_txvector_integrity)
{
	is_valid=true;
	// ********* CHECK TXVECTOR INTEGRITY *********** //
	if (check_txvector_integrity)
	{
			if (SERVICE!=zeros_b(16))
			{
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> WARNING: SERVICE field invalid!" << endl;
				SERVICE=zeros_b(16);
			}
			if ((MCS<0)||(MCS>76)) 
			{ 		
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: MCS field invalid!" << endl;
				is_valid=false;
			}	
			if ((LENGTH<0)||(LENGTH>65535))
			{
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: LENGTH field invalid!" << endl;
				is_valid=false;	
			}
			if ((SMOOTHING!=SMOOTHING_REC)&&(SMOOTHING!=SMOOTHING_NOT_REC)) 
			{ 		
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: SMOOTHING field invalid!" << endl;
				is_valid=false;
			}
			if ((NOT_SOUNDING!=SOUNDING)&&(NOT_SOUNDING!=CONST_NOT_SOUNDING)) 
			{ 		
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: SOUNDING field invalid!" << endl;
				is_valid=false;
			}
			if ((AGGREGATION!=AGGREGATED)&&(AGGREGATION!=NOT_AGGREGATED)) 
			{ 		
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: AGGREGATION field invalid!" << endl;
				is_valid=false;
			}
			if ((STBC<0)||(STBC>3))
			{
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: STBC field invalid!" << endl;
				is_valid=false;
			}
			if ((LDPC_CODING!=BCC_CODING)&&(LDPC_CODING!=LDPC_CODING)) 
			{	 		
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: LDPC_CODING field invalid!" << endl;
				is_valid=false;	
			}
			if ((NUM_EXTEN_SS<0)||(NUM_EXTEN_SS>3))
			{
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: NUM_EXTEN_SS field invalid!" << endl;
				is_valid=false;
			}
			if ((EXPANSION_MAT_TYPE!=NO_EXPANSION_MAT)&&(EXPANSION_MAT_TYPE!=COMPRESSED_SV)&&(EXPANSION_MAT_TYPE!=NON_COMPRESSED_SV)&&(EXPANSION_MAT_TYPE!=CSI_MATRICES)) 
			{ 		
				cout << "ADMIN: TXVECTOR" << endl;
				cout << "------> ERROR: EXPANSION_MAT_TYPE field invalid!" << endl;
				is_valid=false;
			}
			switch (N_TX-STBC)
			{
				case 1:
					if (MCS>7)
					{
						cout << "ADMIN: RXVECTOR" << endl;
						cout << "------> ERROR: MCS " << MCS << " cannot be supported with " << N_TX-STBC << " spatial streams(STBC=" << STBC << ")!" << endl;					
						is_valid=false;
					}
					break;
				case 2:
					if (MCS>15)
					{
						cout << "ADMIN: RXVECTOR" << endl;
						cout << "------> ERROR: MCS " << MCS << " cannot be supported with " << N_TX-STBC << " spatial streams (STBC=" << STBC << ")!" << endl;
						is_valid=false;
					}
					break;

				case 3:
					if (MCS>23)
					{
						cout << "ADMIN: RXVECTOR" << endl;
						cout << "------> ERROR: MCS " << MCS << " cannot be supported with " << N_TX-STBC << " spatial streams (STBC=" << STBC << ")!" << endl;
						is_valid=false;
					}
					break;
				case 4:
					if (MCS>31)
					{
						cout << "ADMIN: RXVECTOR" << endl;
						cout << "------> ERROR: MCS " << MCS << " cannot be supported with " << N_TX-STBC << " spatial streams (STBC=" << STBC << ")!" << endl;
						is_valid=false;
					}
					break;
				default:
					cout << "ADMIN: RXVECTOR" << endl;
					cout << "------> ERROR: Incorrect number of spatial streams (Nss=" << N_TX-STBC << ", STBC=" << STBC << ")!" << endl;  
			} 
		}
		if ((ANTENNA_SET_ON!=ANTENNA_SET_ON)&&(ANTENNA_SET_ON!=NO_ANTENNA_SET_ON))
		{
			cout << "ADMIN: TXVECTOR" << endl;
			cout << "------> ERROR: ANTENNA_SET_ON field invalid!" << endl;
			is_valid=false;	
		}
		if (length(ANTENNA_SET)>8)
		{
			cout << "ADMIN: TXVECTOR" << endl;
			cout << "------> ERROR: ANTENNA_SET field invalid!" << endl;
			is_valid=false;
		}
		if ((TXPWR_LEVEL>8)||(TXPWR_LEVEL<1))
		{
			cout << "ADMIN: TXVECTOR" << endl;
			cout << "------> ERROR: TXPWR_LEVEL field invalid!" << endl;
			is_valid=false;
		}
		if ((N_TX>4)||(N_TX<1))
		{
			cout << "ADMIN: TXVECTOR" << endl;
			cout << "------> ERROR: N_TX field invalid!" << endl;
			is_valid=false;
		}
	
	// **** TXVECTOR INTEGRITY CHECKING COMPLETE **** //

	
	// ********* ASSIGN PRIVATE VARIABLES *********** //
	
	// set the 'number of transmitters' variable for PHY
	if (ANTENNA_SET_ON==0)
	{ // from TXVECTOR.ANTENNA_SET (21.2.2.17)
		Ntx=0;
		for (unsigned int a=0; a<7; a++)
		{
			if (ANTENNA_SET.mid(a,1)==ones_b(1))
				Ntx++;
		}
		if (Ntx!=N_TX)
		{	
			cout << "ADMIN: TXVECTOR" << endl;
			cout << "------> ERROR: N_TX and ANTENNA_SET field mismatch!" << endl;
			is_valid=false;
			
		}
	}
	Ntx=N_TX;
	Ngi=16;
	Nfft=64;
	switch (MCS) // see 21.2.2.6 and 21.7
	{
		case 0:
			mod_index=QAM_2; code_rate=R_1_2; Nbpsc=1; Nsd=52; Nsp=4; Ncbps=52; Ndbps=26; Nss=1; Nes=1;
			break;
		case 1:
			mod_index=QAM_4; code_rate=R_1_2; Nbpsc=2; Nsd=52; Nsp=4; Ncbps=104; Ndbps=52; Nss=1; Nes=1;
			break;
		case 2:
			mod_index=QAM_4; code_rate=R_3_4; Nbpsc=2; Nsd=52; Nsp=4; Ncbps=104; Ndbps=78; Nss=1; Nes=1;
			break;
		case 3:
			mod_index=QAM_16; code_rate=R_1_2; Nbpsc=4; Nsd=52; Nsp=4; Ncbps=208; Ndbps=104; Nss=1; Nes=1;
			break;
		case 4:
			mod_index=QAM_16; code_rate=R_3_4; Nbpsc=4; Nsd=52; Nsp=4; Ncbps=208; Ndbps=156; Nss=1; Nes=1;
			break;
		case 5:
			mod_index=QAM_64; code_rate=R_2_3; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=312; Ndbps=208; Nss=1; Nes=1;
			break;
		case 6:
			mod_index=QAM_64; code_rate=R_3_4; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=312; Ndbps=234; Nss=1; Nes=1;
			break;
		case 7:
			mod_index=QAM_64; code_rate=R_5_6; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=312; Ndbps=260; Nss=1; Nes=1;
			break;
		case 8:
			mod_index=QAM_2; code_rate=R_1_2; Nbpsc=1; Nsd=52; Nsp=4; Ncbps=104; Ndbps=52; Nss=2; Nes=1;
			break;
		case 9:
			mod_index=QAM_4; code_rate=R_1_2; Nbpsc=2; Nsd=52; Nsp=4; Ncbps=208; Ndbps=104; Nss=2; Nes=1;
			break;
		case 10:
			mod_index=QAM_4; code_rate=R_3_4; Nbpsc=2; Nsd=52; Nsp=4; Ncbps=208; Ndbps=156; Nss=2; Nes=1;
			break;
		case 11:
			mod_index=QAM_16; code_rate=R_1_2; Nbpsc=4; Nsd=52; Nsp=4; Ncbps=416; Ndbps=208; Nss=2; Nes=1;
			break;
		case 12:
			mod_index=QAM_16; code_rate=R_3_4; Nbpsc=4; Nsd=52; Nsp=4; Ncbps=416; Ndbps=312; Nss=2; Nes=1;
			break;
		case 13:
			mod_index=QAM_64; code_rate=R_2_3; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=624; Ndbps=416; Nss=2; Nes=1;
			break;
		case 14:
			mod_index=QAM_64; code_rate=R_3_4; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=624; Ndbps=468; Nss=2; Nes=1;
			break;
		case 15:
			mod_index=QAM_64; code_rate=R_5_6; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=624; Ndbps=520; Nss=2; Nes=1;
			break;
		case 16:
			mod_index=QAM_2; code_rate=R_1_2; Nbpsc=1; Nsd=52; Nsp=4; Ncbps=156; Ndbps=78; Nss=3; Nes=1;
			break;
		case 17:
			mod_index=QAM_4; code_rate=R_1_2; Nbpsc=2; Nsd=52; Nsp=4; Ncbps=312; Ndbps=156; Nss=3; Nes=1;
			break;
		case 18:	
			mod_index=QAM_4; code_rate=R_3_4; Nbpsc=2; Nsd=52; Nsp=4; Ncbps=312; Ndbps=234; Nss=3; Nes=1;
			break;
		case 19:
			mod_index=QAM_16; code_rate=R_1_2; Nbpsc=4; Nsd=52; Nsp=4; Ncbps=624; Ndbps=312; Nss=3; Nes=1;
			break;
		case 20:
			mod_index=QAM_16; code_rate=R_3_4; Nbpsc=4; Nsd=52; Nsp=4; Ncbps=624; Ndbps=468; Nss=3; Nes=1;
			break;
		case 21:
			mod_index=QAM_64; code_rate=R_2_3; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=936; Ndbps=624; Nss=3; Nes=1;
			break;
		case 22:
			mod_index=QAM_64; code_rate=R_3_4; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=936; Ndbps=702; Nss=3; Nes=1;
			break;
		case 23:
			mod_index=QAM_64; code_rate=R_5_6; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=936; Ndbps=780; Nss=3; Nes=1;
			break;
		case 24:
			mod_index=QAM_2; code_rate=R_1_2; Nbpsc=1; Nsd=52; Nsp=4; Ncbps=208; Ndbps=104; Nss=4; Nes=1;
			break;
		case 25:
			mod_index=QAM_4; code_rate=R_1_2; Nbpsc=2; Nsd=52; Nsp=4; Ncbps=416; Ndbps=208; Nss=4; Nes=1;
			break;
		case 26:
			mod_index=QAM_4; code_rate=R_3_4; Nbpsc=2; Nsd=52; Nsp=4; Ncbps=416; Ndbps=312; Nss=4; Nes=1;
			break;
		case 27:
			mod_index=QAM_16; code_rate=R_1_2; Nbpsc=4; Nsd=52; Nsp=4; Ncbps=832; Ndbps=416; Nss=4; Nes=1;
			break;
		case 28:
			mod_index=QAM_16; code_rate=R_3_4; Nbpsc=4; Nsd=52; Nsp=4; Ncbps=832; Ndbps=624; Nss=4; Nes=1;
			break;
		case 29:
			mod_index=QAM_64; code_rate=R_2_3; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=1248; Ndbps=832; Nss=4; Nes=1;
			break;
		case 30:
			mod_index=QAM_64; code_rate=R_3_4; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=1248; Ndbps=936; Nss=4; Nes=1;
			break;
		case 31:
			mod_index=QAM_64; code_rate=R_5_6; Nbpsc=6; Nsd=52; Nsp=4; Ncbps=1248; Ndbps=1040; Nss=4; Nes=1;
			break;
		case 33:
			break;
			// not yet supported	
		case 34:
			break;
			// not yet supported
		case 35:
			break;
			// not yet supported
		case 36:
			break;
			// not yet supported
		case 37:
			break;
			// not yet supported
		case 38:
			break;
			// not yet supported
		case 39:
			break;
			// not yet supported	
		case 40:
			break;
			// not yet supported
		case 41:
			break;
			// not yet supported
		case 42:
			break;
			// not yet supported
		case 43:
			break;
			// not yet supported
		case 44:
			break;
			// not yet supported
		case 45:
			break;
			// not yet supported	
		case 46:
			break;
			// not yet supported
		case 47:
			break;
			// not yet supported
		case 48:
			break;
			// not yet supported
		case 49:
			break;
			// not yet supported
		case 50:
			break;
			// not yet supported
		case 51:
			break;
			// not yet supported	
		case 52:
			break;
			// not yet supported
		case 53:
			break;
			// not yet supported
		case 54:
			break;
			// not yet supported
		case 55:
			break;
			// not yet supported
		case 56:
			break;
			// not yet supported
		case 57:
			break;
			// not yet supported	
		case 58:
			break;
			// not yet supported
		case 59:
			break;
			// not yet supported
		case 60:
			break;
			// not yet supported
		case 61:
			break;
			// not yet supported
		case 62:
			break;
			// not yet supported
		case 63:
			break;
			// not yet supported	
		case 64:
			break;
			// not yet supported
		case 65:
			break;
			// not yet supported
		case 66:
			break;
			// not yet supported
		case 67:
			break;
			// not yet supported
		case 68:
			break;
			// not yet supported
		case 69:
			break;
			// not yet supported	
		case 70:
			break;
			// not yet supported
		case 71:
			break;
			// not yet supported
		case 72:
			break;
			// not yet supported
		case 73:
			break;
			// not yet supported
		case 74:
			break;
			// not yet supported
		case 75:
			break;
			// not yet supported	
		case 76:
			break;
			// not yet supported
	}
	switch (MCS_meta) // see 21.2.2.6 and 21.7
	{
		case 0:
			mod_index_meta=QAM_2; code_rate_meta=R_1_2; Nbpsc_meta=1; Nsd=52; Nsp=4; Ncbps_meta=52; Ndbps_meta=26; Nss_meta=1; Nes_meta=1;
			break;
		case 1:
			mod_index_meta=QAM_4; code_rate_meta=R_1_2; Nbpsc_meta=2; Nsd=52; Nsp=4; Ncbps_meta=104; Ndbps_meta=52; Nss_meta=1; Nes_meta=1;
			break;
		case 2:
			mod_index_meta=QAM_4; code_rate_meta=R_3_4; Nbpsc_meta=2; Nsd=52; Nsp=4; Ncbps_meta=104; Ndbps_meta=78; Nss_meta=1; Nes_meta=1;
			break;
		case 3:
			mod_index_meta=QAM_16; code_rate_meta=R_1_2; Nbpsc_meta=4; Nsd=52; Nsp=4; Ncbps_meta=208; Ndbps_meta=104; Nss_meta=1; Nes_meta=1;
			break;
		case 4:
			mod_index_meta=QAM_16; code_rate_meta=R_3_4; Nbpsc_meta=4; Nsd=52; Nsp=4; Ncbps_meta=208; Ndbps_meta=156; Nss_meta=1; Nes_meta=1;
			break;
		case 5:
			mod_index_meta=QAM_64; code_rate_meta=R_2_3; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=312; Ndbps_meta=208; Nss_meta=1; Nes_meta=1;
			break;
		case 6:
			mod_index_meta=QAM_64; code_rate_meta=R_3_4; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=312; Ndbps_meta=234; Nss_meta=1; Nes_meta=1;
			break;
		case 7:
			mod_index_meta=QAM_64; code_rate_meta=R_5_6; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=312; Ndbps_meta=260; Nss_meta=1; Nes_meta=1;
			break;
		case 8:
			mod_index_meta=QAM_2; code_rate_meta=R_1_2; Nbpsc_meta=1; Nsd=52; Nsp=4; Ncbps_meta=104; Ndbps_meta=52; Nss_meta=2; Nes_meta=1;
			break;
		case 9:
			mod_index_meta=QAM_4; code_rate_meta=R_1_2; Nbpsc_meta=2; Nsd=52; Nsp=4; Ncbps_meta=208; Ndbps_meta=104; Nss_meta=2; Nes_meta=1;
			break;
		case 10:
			mod_index_meta=QAM_4; code_rate_meta=R_3_4; Nbpsc_meta=2; Nsd=52; Nsp=4; Ncbps_meta=208; Ndbps_meta=156; Nss_meta=2; Nes_meta=1;
			break;
		case 11:
			mod_index_meta=QAM_16; code_rate_meta=R_1_2; Nbpsc_meta=4; Nsd=52; Nsp=4; Ncbps_meta=416; Ndbps_meta=208; Nss_meta=2; Nes_meta=1;
			break;
		case 12:
			mod_index_meta=QAM_16; code_rate_meta=R_3_4; Nbpsc_meta=4; Nsd=52; Nsp=4; Ncbps_meta=416; Ndbps_meta=312; Nss_meta=2; Nes_meta=1;
			break;
		case 13:
			mod_index_meta=QAM_64; code_rate_meta=R_2_3; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=624; Ndbps_meta=416; Nss_meta=2; Nes_meta=1;
			break;
		case 14:
			mod_index_meta=QAM_64; code_rate_meta=R_3_4; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=624; Ndbps_meta=468; Nss_meta=2; Nes_meta=1;
			break;
		case 15:
			mod_index_meta=QAM_64; code_rate_meta=R_5_6; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=624; Ndbps_meta=520; Nss_meta=2; Nes_meta=1;
			break;
		case 16:
			mod_index_meta=QAM_2; code_rate_meta=R_1_2; Nbpsc_meta=1; Nsd=52; Nsp=4; Ncbps_meta=156; Ndbps_meta=78; Nss_meta=3; Nes_meta=1;
			break;
		case 17:
			mod_index_meta=QAM_4; code_rate_meta=R_1_2; Nbpsc_meta=2; Nsd=52; Nsp=4; Ncbps_meta=312; Ndbps_meta=156; Nss_meta=3; Nes_meta=1;
			break;
		case 18:	
			mod_index_meta=QAM_4; code_rate_meta=R_3_4; Nbpsc_meta=2; Nsd=52; Nsp=4; Ncbps_meta=312; Ndbps_meta=234; Nss_meta=3; Nes_meta=1;
			break;
		case 19:
			mod_index_meta=QAM_16; code_rate_meta=R_1_2; Nbpsc_meta=4; Nsd=52; Nsp=4; Ncbps_meta=624; Ndbps_meta=312; Nss_meta=3; Nes_meta=1;
			break;
		case 20:
			mod_index_meta=QAM_16; code_rate_meta=R_3_4; Nbpsc_meta=4; Nsd=52; Nsp=4; Ncbps_meta=624; Ndbps_meta=468; Nss_meta=3; Nes_meta=1;
			break;
		case 21:
			mod_index_meta=QAM_64; code_rate_meta=R_2_3; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=936; Ndbps_meta=624; Nss_meta=3; Nes_meta=1;
			break;
		case 22:
			mod_index_meta=QAM_64; code_rate_meta=R_3_4; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=936; Ndbps_meta=702; Nss_meta=3; Nes_meta=1;
			break;
		case 23:
			mod_index_meta=QAM_64; code_rate_meta=R_5_6; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=936; Ndbps_meta=780; Nss_meta=3; Nes_meta=1;
			break;
		case 24:
			mod_index_meta=QAM_2; code_rate_meta=R_1_2; Nbpsc_meta=1; Nsd=52; Nsp=4; Ncbps_meta=208; Ndbps_meta=104; Nss_meta=4; Nes_meta=1;
			break;
		case 25:
			mod_index_meta=QAM_4; code_rate_meta=R_1_2; Nbpsc_meta=2; Nsd=52; Nsp=4; Ncbps_meta=416; Ndbps_meta=208; Nss_meta=4; Nes_meta=1;
			break;
		case 26:
			mod_index_meta=QAM_4; code_rate_meta=R_3_4; Nbpsc_meta=2; Nsd=52; Nsp=4; Ncbps_meta=416; Ndbps_meta=312; Nss_meta=4; Nes_meta=1;
			break;
		case 27:
			mod_index_meta=QAM_16; code_rate_meta=R_1_2; Nbpsc_meta=4; Nsd=52; Nsp=4; Ncbps_meta=832; Ndbps_meta=416; Nss_meta=4; Nes_meta=1;
			break;
		case 28:
			mod_index_meta=QAM_16; code_rate_meta=R_3_4; Nbpsc_meta=4; Nsd=52; Nsp=4; Ncbps_meta=832; Ndbps_meta=624; Nss_meta=4; Nes_meta=1;
			break;
		case 29:
			mod_index_meta=QAM_64; code_rate_meta=R_2_3; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=1248; Ndbps_meta=832; Nss_meta=4; Nes_meta=1;
			break;
		case 30:
			mod_index_meta=QAM_64; code_rate_meta=R_3_4; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=1248; Ndbps_meta=936; Nss_meta=4; Nes_meta=1;
			break;
		case 31:
			mod_index_meta=QAM_64; code_rate_meta=R_5_6; Nbpsc_meta=6; Nsd=52; Nsp=4; Ncbps_meta=1248; Ndbps_meta=1040; Nss_meta=4; Nes_meta=1;
			break;
		case 33:
			break;
			// not yet supported	
		case 34:
			break;
			// not yet supported
		case 35:
			break;
			// not yet supported
		case 36:
			break;
			// not yet supported
		case 37:
			break;
			// not yet supported
		case 38:
			break;
			// not yet supported
		case 39:
			break;
			// not yet supported	
		case 40:
			break;
			// not yet supported
		case 41:
			break;
			// not yet supported
		case 42:
			break;
			// not yet supported
		case 43:
			break;
			// not yet supported
		case 44:
			break;
			// not yet supported
		case 45:
			break;
			// not yet supported	
		case 46:
			break;
			// not yet supported
		case 47:
			break;
			// not yet supported
		case 48:
			break;
			// not yet supported
		case 49:
			break;
			// not yet supported
		case 50:
			break;
			// not yet supported
		case 51:
			break;
			// not yet supported	
		case 52:
			break;
			// not yet supported
		case 53:
			break;
			// not yet supported
		case 54:
			break;
			// not yet supported
		case 55:
			break;
			// not yet supported
		case 56:
			break;
			// not yet supported
		case 57:
			break;
			// not yet supported	
		case 58:
			break;
			// not yet supported
		case 59:
			break;
			// not yet supported
		case 60:
			break;
			// not yet supported
		case 61:
			break;
			// not yet supported
		case 62:
			break;
			// not yet supported
		case 63:
			break;
			// not yet supported	
		case 64:
			break;
			// not yet supported
		case 65:
			break;
			// not yet supported
		case 66:
			break;
			// not yet supported
		case 67:
			break;
			// not yet supported
		case 68:
			break;
			// not yet supported
		case 69:
			break;
			// not yet supported	
		case 70:
			break;
			// not yet supported
		case 71:
			break;
			// not yet supported
		case 72:
			break;
			// not yet supported
		case 73:
			break;
			// not yet supported
		case 74:
			break;
			// not yet supported
		case 75:
			break;
			// not yet supported	
		case 76:
			break;
			// not yet supported
	}
	Nsts=Nss+STBC;
	Nsts_meta=Nss_meta+STBC_meta;
	Nst=Nsd+Nsp;
	Nofdm=Nfft+Ngi;
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
	Ness=NUM_EXTEN_SS;
	if (Ness!=0)
			Neltf=Ness+1;
	
	if (LENGTH==0)
		Nsym=0;
	else
	{
		if (STBC==0) // number of symbols (eq: 21-28) 
			Nsym=ceil_i(double(8*LENGTH+16+6*Nes)/double(Ndbps)); 
		else
			Nsym=2*ceil_i(double(8*LENGTH+16+6*Nes)/double(2*Ndbps)); 
	}
	if (LENGTH_meta==0)
		Nsym_meta=0;
	else
		Nsym_meta=ceil_i(double(8*LENGTH_meta+16+6*Nes_meta)/double(Ndbps_meta));
	Ndatacoded=Nsym*Ncbps;
	Nmetacoded=Nsym_meta*Ncbps_meta;
	Ndatasamples=Nsym*(Nfft+Ngi);
	Nmetasamples=Nsym_meta*(Nfft+Ngi);
	Npresamples=(Ndltf+Neltf+1)*Nofdm+5*Nofdm;
	Nnull=Nfft-Nst;		
}


