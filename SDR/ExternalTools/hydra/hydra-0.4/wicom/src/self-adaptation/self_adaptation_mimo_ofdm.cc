/* -*- c++ -*- */
/* 
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
 */

#include "self_adaptation_mimo_ofdm.h" 
#include <fstream>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <ANN/ANN.h>

using namespace itpp;
using std::cout;
using std::endl;

Self_Adaptation_MIMO_OFDM::Self_Adaptation_MIMO_OFDM(int feature_index, int classifier_index, int equalizer_index, int fec_index, int parameter_set_index, int frame_length, int number_of_subcarriers, int fft_size, int precoding_index)
{
	// initialize self adaptation parameters
	param.ftidx=feature_index;
  param.clidx=classifier_index;
  param.eqidx=equalizer_index; 
  param.ecidx=fec_index; 
  param.psidx=parameter_set_index;
  param.frlen=frame_length;
  param.nsubs=number_of_subcarriers; 
	param.dbsize=1000;
	param.preidx=precoding_index;
	param.errdbsize=200;
	param.ndbs=0;
	param.k=1;
	param.e=1;
	param.fftsz=fft_size;
	secarray=0;
	pscost="0";
	fsdpub=1;
	explore=false;
	preserve=false;

	// create subcarrier mapping vector array
	{
		subcarrier_mapping.set_size(114);
		for (int idx=0; idx<114; idx++)
			subcarrier_mapping(idx).set_size(0);
		subcarrier_mapping(56)="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63";
	}
	
	// create cyclic shift vectors
	{
		cyclic_shift_vectors.set_size(114);
		for (int idx=0; idx<114; idx++)
			cyclic_shift_vectors(idx).set_size(0);
		cyclic_shift_vectors(56)="0 8 4 12";
	}
	
	// create subcarrier ordering sets	
	subcarrier_ordering_set_8_taps = "{ [13] [0 6] [2 11 37] [0 2 8 28] [2 8 12 20 37] [2 6 10 14 22 38] }";
	
	// remaining class initialization
	determine_feature_dimensions();

	current_mode=0;
	nack_count=0;
	ack_count=0;	
	ackub=10;
	nackub=2;
}

Self_Adaptation_MIMO_OFDM::~Self_Adaptation_MIMO_OFDM()
{		
	if (annobjarray.size()!=0)
	{
		for (int entidx=annobjarray.size()-1; entidx>=0; entidx--)
			 delete annobjarray[entidx];
	}
	if (entriesarray.size()!=0)
	{	
		for (int entidx=entriesarray.size()-1; entidx>=0; entidx--)
			 delete [] entriesarray[entidx];
	}
	entriesarray.clear();
	annobjarray.clear();
	if (secarray!=NULL)
		delete [] secarray;
	annClose();
}

void Self_Adaptation_MIMO_OFDM::print_parameters(std::ostream& out)
{
	param.print_parameters(out);
}

void Self_Adaptation_MIMO_OFDM::print_data(std::ostream& out, bool print_entries)
{
	out << endl <<  "***** Self Adaptation Data Listing *****" << endl << endl;
	{
		out << "-----------" << endl;
		out << "Timestamps:" << endl;
		out << "-----------" << endl;
		for (int dbidx=0; dbidx<param.ndbs; dbidx++)
	 	{
			out << ">>> DB#: " << dbidx+1 << endl << "| ";
			for (int szidx=0; szidx<param.dbsize; szidx++)
				out << szidx << "/" << secarray[dbidx][szidx] << " | ";
			out << endl;
		}
	}
	{
		out << "-----------------" << endl;
		out << "Database Filling:" << endl;
		out << "-----------------" << endl;
		for (int dbidx=0; dbidx<param.ndbs; dbidx++)
		{
			out << ">>> DB#: " << dbidx+1 << endl;
			out << "Error Entries Left = " << errleft[dbidx];
			out << " | Success Entries Left = " << ackleft[dbidx];
			out << endl;
		}
	}
	if (print_entries)
	{
		out << "----------- " << endl;
		out << "Entry Data: " << endl;
		out << "----------- " << endl;
		for (int dbidx=0; dbidx<param.ndbs; dbidx++)
	 	{
			out << ">>> DB#: " << dbidx+1 << endl << "| ";
			for (int szidx=0; szidx<param.dbsize; szidx++)
			{
				out << szidx << "/";
				for (int dimidx=0; dimidx<dimarray[dbidx]; dimidx++)
					out << entriesarray[dbidx][szidx][dimidx] << "/";
				out << " | ";
			}
			out << endl;
		}
	}
	out << endl << "****************************************" << endl << endl;
}		

int Self_Adaptation_MIMO_OFDM::recommend_parameters(Array<cmat> const& channel, double N0)
{
	Array<vec> ps_features;
	extract_features(channel,N0,ps_features);
  //std::cerr << "[SELF ADAPTATION] ps_features = " << ps_features << "\n";
	if (explore)
		return (explore_mapping(query_adaptation_database(ps_features)));
	else
		return query_adaptation_database(ps_features);
}

int Self_Adaptation_MIMO_OFDM::arf_recommend_parameters()
{
	int nextmode=current_mode;
	if (nack_count>=nackub)
	{ // step down in rate
		nack_count=0;
		ack_count=0;
		nextmode=0;
		for (int modeind=current_mode; modeind>=0; modeind--)
		{
			if ((pscost(modeind)<pscost(current_mode))&&(pscost(modeind)>pscost(nextmode)))
				nextmode=modeind;
		}
	}
	else if (ack_count>=ackub)
	{ // step up in rate
		nack_count=0;
		ack_count=0;	
		nextmode=15;
		for (int modeind=current_mode; modeind<pscost.length(); modeind++)
		{
			if ((pscost(modeind)>pscost(current_mode))&&(pscost(modeind)<pscost(nextmode)))
				nextmode=modeind;
		}
	
	}
	current_mode=nextmode;
	return nextmode;
}

int Self_Adaptation_MIMO_OFDM::recommend_parameters(Array<mat> const& post_process_snr)
{
	Array<vec> ps_features;
	extract_features(post_process_snr,ps_features);
	if (explore)
		return (explore_mapping(query_adaptation_database(ps_features)));
	else
		return query_adaptation_database(ps_features);
}

int Self_Adaptation_MIMO_OFDM::recommend_parameters_and_update(Array<cmat> const& channel, double N0, int param_used, bool is_error)
{	
	Array<vec> ps_features(param.ndbs);
	extract_features(channel,N0,ps_features);
	update_database(ps_features(param_used),param_used,is_error);
	if (explore)
		return (explore_mapping(query_adaptation_database(ps_features)));
	else
		return query_adaptation_database(ps_features);
}

int Self_Adaptation_MIMO_OFDM::recommend_parameters_and_update(Array<mat> const& post_process_snr, int param_used, bool is_error)
 {	
	Array<vec> ps_features(param.ndbs);
	extract_features(post_process_snr,ps_features);
	update_database(ps_features(param_used),param_used,is_error);
	if (explore)
		return (explore_mapping(query_adaptation_database(ps_features)));
	else
		return query_adaptation_database(ps_features);
}

void Self_Adaptation_MIMO_OFDM::extract_feature(mat const& post_process_snr, int param_index, vec& feature)
{
	int Nst=post_process_snr.cols();
	int Ns=post_process_snr.rows();
	feature.set_size(dimarray[param_index]);
	switch (param.ftidx) // feature index
	{
		case 0: // average SNR
		case 1: // average SNR and condition
			{
				feature(0)=sumsum(post_process_snr)/(Ns*Nst);
				for (int dimidx=1; dimidx<dimarray[param_index]; dimidx++)
					feature(dimidx)=feature(0);
			}
		case 2: // subcarrier ordering
			{
				double snrsforsort[Nst*Ns];
				for (int subidx=0; subidx<Nst; subidx++)
				{
					for (int strmidx=0; strmidx<Ns; strmidx++)
						snrsforsort[subidx*Ns+strmidx]=post_process_snr(strmidx,subidx);
				}
				//for (int tt=0; tt<Nst*Ns; tt++)
				//	cout << "snrs[tt]: "  << snrsforsort[tt] << endl;
				std::sort((double *)snrsforsort,snrsforsort+Nst*Ns);
				//for (int ttt=0; ttt<Nst*Ns; ttt++)
				//	cout << "snrs[ttt]: "  << snrsforsort[ttt] << endl;
				
				for (int dimidx=0; dimidx<dimarray[param_index]; dimidx++)
				{
					feature(dimidx)=snrsforsort[subcarrier_ordering_set_8_taps(dimarray[param_index]-1)(dimidx)];
					//cout << "element: " << subcarrier_ordering_set_8_taps(dimarray[param_index]-1)(dimidx) << endl;
				}
		}
	}
}

void Self_Adaptation_MIMO_OFDM::extract_features(Array<mat> const& post_process_snr, Array<vec>& features)
{
	features.set_size(param.ndbs);
	for (int dbidx=0; dbidx<param.ndbs; dbidx++)
		extract_feature(post_process_snr(psstrm(dbidx)-1),dbidx,features(dbidx));
}

void Self_Adaptation_MIMO_OFDM::extract_features(Array<cmat> const& channel_query, double N0, Array<vec>& features)
{
	int Nst=channel_query.size();
	switch (param.ftidx) // feature index
	{
		case 0: // average SNR
		case 2: // subcarrier ordering
			{
				Array<mat> snrs(max(psstrm));
				compute_post_processing_snr(channel_query,N0,snrs);
				extract_features(snrs,features);
				break;
			}
		case 1: // average SNR and Demmel condition number
			{
				// calculate average SNR first
				Array<mat> snrs(max(psstrm));
				compute_post_processing_snr(channel_query,N0,snrs);
				extract_features(snrs,features);
				// now calculate condition number
				double conds[Nst];
				compute_condition_number(channel_query,(double *)conds);
				for (int subidx=0; subidx<Nst; subidx++)
				{	
					if (dimarray[0]>0)
						features(0)(1)=features(0)(1)+conds[subidx];
				}
				features(0)=features(0)/Nst;
				for (int dbidx=1; dbidx<param.ndbs; dbidx++)
				{
					if (dimarray[dbidx]>0)
						features(dbidx)(1)=features(0)(1);
					else
						cout << "ERROR: Must have at least 2-dimensions for this feature set!" << endl;
				}
			}
			break;
		default: 
			break;
	}
}

Array<vec> Self_Adaptation_MIMO_OFDM::extract_features(Array<cmat> const& channel_query, double N0)
{
	Array<vec> cf;
	extract_features(channel_query,N0,cf);
	return cf;
}

		
int Self_Adaptation_MIMO_OFDM::query_adaptation_database(Array<cmat> const& channel_query, double N0)
{	
	Array<vec> ftvec=extract_features(channel_query,N0);
	return query_adaptation_database(ftvec);
}

void Self_Adaptation_MIMO_OFDM::find_database_neighbors(vec const& feature_query, int database_index, int k, int* neighbor_indices, double* neighbor_distances)
{
	ANNpoint query;
	query = annAllocPt(dimarray[database_index]);
	determine_classifier_parameters();
	for (int dimidx=0; dimidx<dimarray[database_index]; dimidx++)
	{
		query[dimidx]=feature_query(dimidx);
	}
	annobjarray[database_index]->annkSearch(query,k,neighbor_indices,neighbor_distances,0);
}
   	
int Self_Adaptation_MIMO_OFDM::query_adaptation_database(Array<vec> const& feature_query)
{
	ANNpoint query;
	double dbcost[param.ndbs];
	determine_classifier_parameters();
	for (int dbidx=0; dbidx<param.ndbs; dbidx++)
	{
		query = annAllocPt(dimarray[dbidx]);
		{
			for (int dimidx=0; dimidx<dimarray[dbidx]; dimidx++)
			{
				query[dimidx]=feature_query(dbidx)(dimidx);
	//			cout << "query[" << dimidx << "]=" << query[dimidx] << endl;
			}
		}
		ANNidxArray nninds;
		ANNdistArray nndists;
		nninds = new ANNidx[param.k];
		nndists = new ANNdist[param.k];
		annobjarray[dbidx]->annkSearch(query,param.k,nninds,nndists,0);
//		for (int idx=0; idx<param.k; idx++)
//		{
//			cout << "nnindx[" << idx << "]=" << nninds[idx] << endl;
//			cout << "nndists[" << idx << "]=" << nndists[idx] << endl;
//		}
		delete [] nninds;
		delete [] nndists;
		annDeallocPt(query);
		dbcost[dbidx]=std::min(param.e-total_errors(nninds,dbidx)+1,1)*pscost(dbidx);
	//	if (dbidx==7)
	//	{
	//		annobjarray[dbidx]->Print(ANNtrue,std::cout);
	//		cout << "dbcost[" << dbidx << "]=" << dbcost[dbidx] << endl;
	//	}
	}
	return max_double_index(dbcost,param.ndbs);
}		

void Self_Adaptation_MIMO_OFDM::update_database(Array<cmat> const& channel_observed, double N0, int parameter_index, bool is_error)
{
	Array<vec> fq=extract_features(channel_observed,N0); // feature query defined
  //std::cerr << "[SELF_ADAPTATION] extracted features = " << fq << "\n";
	update_database(fq(parameter_index), parameter_index, is_error); // update databse with query
}

void Self_Adaptation_MIMO_OFDM::update_database(mat const& post_process_snr, int param_index, bool is_error)
{
 	vec fq;
	extract_feature(post_process_snr,param_index,fq);
	//cout << "feature: " << fq << endl;
	update_database(fq,param_index,is_error);
}

void Self_Adaptation_MIMO_OFDM::initialize_database(int feature_index, int classifier_index, int equalizer_index, int fec_index, int parameter_set_index, int frame_length, int number_of_subcarriers, int database_size, int errors_in_database, int fft_size)
{
	param.ftidx=feature_index;
  param.clidx=classifier_index;
  param.eqidx=equalizer_index; 
  param.ecidx=fec_index; 
  param.psidx=parameter_set_index;
  param.frlen=frame_length;
  param.nsubs=number_of_subcarriers; 
	param.fftsz=fft_size;
	initialize_database(database_size,errors_in_database); // create blank database
}

void Self_Adaptation_MIMO_OFDM::initialize_database(int database_size, int errors_in_database)
{
	param.dbsize=database_size;
	param.errdbsize=errors_in_database;
	load_database("empty"); // loads an empty database
}

void Self_Adaptation_MIMO_OFDM::load_database(std::string file_name)
{
	const char* fname=file_name.c_str();
	// clear memory from old database
	if (annobjarray.size()!=0)
	{
		for (int entidx=annobjarray.size()-1; entidx>=0; entidx--)
		 delete annobjarray[entidx];
	}
	if (entriesarray.size()!=0)
	{
		for (int entidx=entriesarray.size()-1; entidx>=0; entidx--)
		 delete [] entriesarray[entidx];
	}
	entriesarray.clear();
	annobjarray.clear();
	if (secarray!=NULL)
		delete [] secarray;
	
	// check input file
	std::ifstream ifile(fname,std::ios::in|std::ios::binary); // open input file
	if ((file_name=="empty")||(!ifile.good())) // create blank database
	{
		determine_feature_dimensions();
		print_parameters(std::cout);
		secarray = new time_t*[param.ndbs];
		for (int secidx=0; secidx<param.ndbs; secidx++)
			secarray[secidx] = new time_t[param.dbsize];
		errleft = new int[param.ndbs];
		ackleft = new int[param.ndbs];
		for (int dbidx=0; dbidx<param.ndbs; dbidx++)
		{		
			entriesarray.push_back(ANNpointArray());
			errleft[dbidx]=param.errdbsize;
			ackleft[dbidx]=param.dbsize-param.errdbsize;
			// determine the dimensions of features
			entriesarray[dbidx] = annAllocPts(param.dbsize,dimarray[dbidx]);
			// need to fill in a blank database
			int szidx;	
			for (szidx=0; szidx<param.errdbsize; szidx++)
			{
				secarray[dbidx][szidx]=time(NULL);
				for (int dimidx=0; dimidx<dimarray[dbidx]; dimidx++)
					entriesarray[dbidx][szidx][dimidx]=0;
			}
			for (szidx=param.errdbsize; szidx<param.dbsize; szidx++)
			{
				secarray[dbidx][szidx]=time(NULL);
				for (int dimidx=0; dimidx<dimarray[dbidx]; dimidx++)
					entriesarray[dbidx][szidx][dimidx]=40;
			}
				
		 	annobjarray.push_back(new ANNbruteForce(entriesarray[dbidx],param.dbsize,dimarray[dbidx]));
		}
	}
	else // load database from file
	{
		// read in system parameters
		ifile.seekg(0,std::ios::beg);
		ifile.read((char *)&param,sizeof(Self_Adaptation_Parameters));
		determine_feature_dimensions();
		print_parameters(std::cout);
		secarray = new time_t*[param.ndbs];
		for (int secidx=0; secidx<param.ndbs; secidx++)
		{
			secarray[secidx] = new time_t[param.dbsize];
			for (int szidx=0; szidx<param.dbsize; szidx++)
			{
				ifile.read((char *)&secarray[secidx][szidx],sizeof(time_t));
			//	cout << "secarray[" << secidx << "][" << szidx << "]=" << secarray[secidx][szidx] << endl;
			}
		}
		errleft = new int[param.ndbs];
		ackleft = new int[param.ndbs];
		ifile.read((char *)ackleft,param.ndbs*sizeof(int));
		ifile.read((char *)errleft,param.ndbs*sizeof(int));
		// read in database elements
		for (int dbidx=0; dbidx<param.ndbs; dbidx++)
		{
			//cout << "errleft[" << dbidx << "]=" << errleft[dbidx] << endl;
			//cout << "ackleft[" << dbidx << "]=" << ackleft[dbidx] << endl;
			entriesarray.push_back(ANNpointArray());
			entriesarray[dbidx] = annAllocPts(param.dbsize,dimarray[dbidx]);
			for (int szidx=0; szidx<param.dbsize; szidx++)
			{
				for (int dimidx=0; dimidx<dimarray[dbidx]; dimidx++)
					ifile.read((char *)&entriesarray[dbidx][szidx][dimidx],sizeof(double));
			}
		 	annobjarray.push_back(new ANNbruteForce(entriesarray[dbidx],param.dbsize,dimarray[dbidx]));
		}			
	}
}

void Self_Adaptation_MIMO_OFDM::update_database(vec const& feature_observed, int parameter_index, bool is_error)
{
	if (is_error)
	{
  //	std::cerr << "[SELF ADAPTATION] Updating NACK entry: " << feature_observed << endl;
 // 	std::cerr << "[SELF ADAPTATION] NACKs left: " << errleft[parameter_index] << endl;
		// only remove error coordinates when databasee full
		nack_count++;
		ack_count=0;
		if (errleft[parameter_index]==0)
		{
			if (preserve) // feature set diversity preservation
			{
				int* nninds;
				double* nndists;
				nninds = new int[param.k];
				nndists = new double[param.k];
				find_database_neighbors(feature_observed,parameter_index,param.k,(int*)nninds,(double*)nndists);
				int oldest_index=nninds[0];
				double largest_distance=nndists[0];
				int largest_index=nninds[0];
//				std::cerr << "[SELF ADAPTATION] Distances = [ " << nndists[0];
				for (int nbidx=1; nbidx<dimarray[parameter_index]; nbidx++) // find oldest neighbor and largest distance
				{
//					std::cerr << ", " << nndists[nbidx];
					if (secarray[parameter_index][nninds[nbidx]]<secarray[parameter_index][oldest_index])
						oldest_index=nninds[nbidx];
					if (nndists[nbidx]>largest_distance)
					{
						largest_distance=nndists[nbidx];
						largest_index=nninds[nbidx];
					}
				}
//				std::cerr << " ]" << endl;
 // 			std::cerr << "[SELF ADAPTATION] NACK FSDP: ";
 // 			std::cerr << "Largest Distance = " << largest_distance << ", Oldest Index = " << oldest_index << endl;
			//	std::cerr << "[SELF ADAPTATION] kth Neighbor (entry " << largest_index << ") = [";
	//			for (int aaa=0; aaa<dimarray[parameter_index]; aaa++)
			//		std::cerr << " " << entriesarray[parameter_index][largest_index][aaa];
			//	std::cerr << " ]" << endl;
				delete [] nninds;
				delete [] nndists;
				double neighbor_density=param.k/pow(largest_distance,dimarray[parameter_index]);;
				if (neighbor_density>fsdpub) // bound on feature set density
				{
//					std::cerr << "[SELF ADAPTATION] Upper Bound Exceeded!" << endl;
//					std::cerr << "[SELF ADAPTATION] Density = " << neighbor_density << ", Bound = " << fsdpub  << endl;
					if (oldest_index<param.errdbsize) // entry is from NACK
					{
						for (int dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
							entriesarray[parameter_index][oldest_index][dimidx]=feature_observed(dimidx);
						secarray[parameter_index][oldest_index]=time(NULL);
		//				std::cerr << "[SELF ADAPTATION] Entry " << oldest_index << " replaced (1)" << endl;
					}
					else // entry is from ACK
					{		
						int dimidx;	
						// replace oldest neighbor with boundary value
						for (dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
							entriesarray[parameter_index][oldest_index][dimidx]=5e3;
						secarray[parameter_index][oldest_index]=0;
			//			std::cerr << "[SELF ADAPTATION] Entry " << oldest_index << " erased (2)" << endl;
						// replace oldest entry with current one
						oldest_index=min_time_index(secarray[parameter_index],param.errdbsize);
						secarray[parameter_index][oldest_index]=time(NULL);
						for (dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
							entriesarray[parameter_index][oldest_index][dimidx]=feature_observed(dimidx);
			//			std::cerr << "[SELF ADAPTATION] Entry " << oldest_index << " replaced (3)" << endl;
					}
				}
				else // density bound not exceeded, use age-based updating
				{
			//		cout << "[SELF ADAPTATION] Age Based Updating" << endl;
					oldest_index=min_time_index(secarray[parameter_index],param.errdbsize);
					secarray[parameter_index][oldest_index]=time(NULL);
					for (int dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
							entriesarray[parameter_index][oldest_index][dimidx]=feature_observed(dimidx);
			//		std::cerr << "[SELF ADAPTATION] Entry " << oldest_index << " replaced (4)" << endl;
				}
			}
			else
			{
				int oldest_index=min_time_index(secarray[parameter_index],param.errdbsize);
				secarray[parameter_index][oldest_index]=time(NULL);
				for (int dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
					entriesarray[parameter_index][oldest_index][dimidx]=feature_observed(dimidx);
	//			std::cerr << "[SELF ADAPTATION] Entry " << oldest_index << " replaced (5)" << endl;
			}
		}
		else
		{
//			std::cerr << "[SELF ADAPTATION] Adding to unfilled entry " << param.errdbsize-errleft[parameter_index] << " (6)" << endl;
			//cout << "errleft[parameter_index]=" << errleft[parameter_index] << endl;
			 for (int dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
			{	
				entriesarray[parameter_index][param.errdbsize-errleft[parameter_index]][dimidx]=feature_observed(dimidx);
				//cout << "indices: [" << parameter_index << "][" << param.errdbsize-errleft[parameter_index] << "][" << dimidx << "]" << endl;
			}
			secarray[parameter_index][param.errdbsize-errleft[parameter_index]]=time(NULL);
			errleft[parameter_index]=errleft[parameter_index]-1;
		}
	}
	else
	{
		ack_count++;
		nack_count=0;
  //	std::cerr << "[SELF ADAPTATION] Updating ACK entry: " << feature_observed << endl;
  //	std::cerr << "[SELF ADAPTATION] ACKs left: " << ackleft[parameter_index] << endl;
		// only remove ack coordinates when database full
		if (ackleft[parameter_index]==0)
		{
			if (preserve) // feature set diversity preservation
			{
				int* nninds;
				double* nndists;
				nninds = new int[param.k];
				nndists = new double[param.k];
				find_database_neighbors(feature_observed,parameter_index,param.k,(int*)nninds,(double*)nndists);
				int oldest_index=nninds[0];
				double largest_distance=nndists[0];
				int largest_index=nninds[0];
//				std::cerr << "[SELF ADAPTATION] Distances = [ " << nndists[0];
				for (int nbidx=1; nbidx<param.k; nbidx++) // find oldest neighbor and largest distance
				{
//					std::cerr << ", " << nndists[nbidx];
					if (secarray[parameter_index][nninds[nbidx]]<secarray[parameter_index][oldest_index])
						oldest_index=nninds[nbidx];
					if (nndists[nbidx]>largest_distance)
					{	
						largest_distance=nndists[nbidx];
						largest_index=nninds[nbidx];
					}
				}	
//				std::cerr << " ]" << endl;
//				std::cerr << "[SELF ADAPTATION] ACK FSDP: ";
 // 			std::cerr << "Largest Distance = " << largest_distance << ", Oldest Index = " << oldest_index << endl;
//				std::cerr << "[SELF ADAPTATION] kth Neighbor (entry " << largest_index << ") = [";
//				for (int aaa=0; aaa<dimarray[parameter_index]; aaa++)
//					std::cerr << " " << entriesarray[parameter_index][largest_index][aaa];
//				std::cerr << " ]" << endl;
				delete [] nninds;
				delete [] nndists;
				double neighbor_density=param.k/pow(largest_distance,dimarray[parameter_index]);;
				if (neighbor_density>fsdpub) // bound on feature set density
				{
	//				std::cerr << "[SELF ADAPTATION] Density = " << neighbor_density << ", Bound = " << fsdpub  << endl;
	//				std::cerr << "[SELF ADAPTATION] Upper Bound Exceeded!" << endl;
					if (oldest_index>=param.errdbsize) // entry is from ACK
					{
						for (int dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
							entriesarray[parameter_index][oldest_index][dimidx]=feature_observed(dimidx);
						secarray[parameter_index][oldest_index]=time(NULL);
///						std::cerr << "[SELF ADAPTATION] Entry " << oldest_index << " replaced (1)" << endl;
					}
					else // entry is from NACK
					{		
						int dimidx;	
						// replace oldest neighbor with boundary value
						for (dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
							entriesarray[parameter_index][oldest_index][dimidx]=5e3;
						secarray[parameter_index][oldest_index]=0;
//						std::cerr << "[SELF ADAPTATION] Entry " << oldest_index << " erased (2)" << endl;
						// replace oldest ACK entry with current one
						oldest_index=min_time_index(secarray[parameter_index]+param.errdbsize,param.dbsize-param.errdbsize);
						secarray[parameter_index][param.errdbsize+oldest_index]=time(NULL);
						for (dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
							entriesarray[parameter_index][param.errdbsize+oldest_index][dimidx]=feature_observed(dimidx);
//						std::cerr << "[SELF ADAPTATION] Entry " << param.errdbsize+oldest_index << " replaced (3)" << endl;
					}
				}
				else // density bound not exceeded, use age-based updating
				{
					oldest_index=min_time_index(secarray[parameter_index]+param.errdbsize,param.dbsize-param.errdbsize);
					secarray[parameter_index][param.errdbsize+oldest_index]=time(NULL);
					for (int dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
						entriesarray[parameter_index][param.errdbsize+oldest_index][dimidx]=feature_observed(dimidx);
//					std::cerr << "[SELF ADAPTATION] Entry " << param.errdbsize+oldest_index << " replaced (4)" << endl;
				}
			}
			else
			{
				int oldest_index=min_time_index(secarray[parameter_index]+param.errdbsize,param.dbsize-param.errdbsize);
				secarray[parameter_index][param.errdbsize+oldest_index]=time(NULL);
				for (int dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
					entriesarray[parameter_index][param.errdbsize+oldest_index][dimidx]=feature_observed(dimidx);
//				std::cerr << "[SELF ADAPTATION] Entry " << param.errdbsize+oldest_index << " replaced (5)" << endl;
			}
		}
		else
		{
//			std::cerr << "[SELF ADAPTATION] Adding to unfilled entry " << param.dbsize-ackleft[parameter_index] << " (6)" << endl;
			for (int dimidx=0; dimidx<dimarray[parameter_index]; dimidx++)
				entriesarray[parameter_index][param.dbsize-ackleft[parameter_index]][dimidx]=feature_observed(dimidx);
			secarray[parameter_index][param.dbsize-ackleft[parameter_index]]=time(NULL);
			ackleft[parameter_index]=ackleft[parameter_index]-1;
		}
	}	
}

void Self_Adaptation_MIMO_OFDM::save_database(std::string file_name)
{
	const char* fname=file_name.c_str();
	if (entriesarray.size()==0)
 		cout << "WARNING: Empty Database - No data written!" << endl;
	else
	{
		std::ofstream ofile(fname,std::ios::out|std::ios::binary); // open output file
		// write parameters to file
		ofile.seekp(0);
		ofile.write((char *)&param,sizeof(Self_Adaptation_Parameters));
		for (int secidx=0; secidx<param.ndbs; secidx++)
		{	
			for (int szidx=0; szidx<param.dbsize; szidx++)
				ofile.write((char *)&secarray[secidx][szidx],sizeof(time_t));
		}
		ofile.write((char *)ackleft,param.ndbs*sizeof(int));
		ofile.write((char *)errleft,param.ndbs*sizeof(int));
		// write database to file
		for (int dbidx=0; dbidx<param.ndbs; dbidx++)
		{
			for (int szidx=0; szidx<param.dbsize; szidx++)
			{
				for (int dimidx=0; dimidx<dimarray[dbidx]; dimidx++)
					ofile.write((char *)&entriesarray[dbidx][szidx][dimidx],sizeof(double));
			}
		}
		ofile.close(); // close output file
	}
}

int Self_Adaptation_MIMO_OFDM::total_errors(ANNidx* indices, int db_index)
{
	int error_count=0;
	for (int tempidx=0; tempidx<param.k; tempidx++)
	{
		if (indices[tempidx]<param.errdbsize)
			error_count++;		
	}
	return error_count;
}

void Self_Adaptation_MIMO_OFDM::determine_classifier_parameters()
{
	switch (param.clidx)
	{
		case 0: // 1 in 10 (10 % PER) 
			param.k=10;param.e=1;
			break;
		case 1: // 2 in 20 (10 % PER)
			param.k=20;param.e=2;
			break;
		case 2: // 3 in 30 (10 % PER)
			param.k=30;param.e=3;
			break;
		case 3: // 4 in 40 (10 % PER)
			param.k=40;param.e=4;
			break;
		case 4: // 5 in 50 (10 % PER) 
			param.k=50;param.e=5;
			break;
		case 5: // 10 in 100 (10 % PER)
			param.k=100;param.e=10;
			break;
		case 6: // 1 in 20 (5 % PER)
			param.k=20;param.e=1;
			break;
		case 7: // 2 in 40 (5 % PER) 
			param.k=40;param.e=2;
			break;
		case 8: // 3 in 60 (5 % PER)
			param.k=60;param.e=3;
			break;
		case 9: // 4 in 80 (5 % PER)
			param.k=80;param.e=4;
			break;
		case 10: // 5 in 100 (5 % PER)
			param.k=100;param.e=5;
			break;
		case 11: // 1 in 25 (4 % PER) 
			param.k=25;param.e=1;
			break;
		case 12: // 2 in 50 (4 % PER)
			param.k=50;param.e=2;
			break;
		case 13: // 3 in 75 (4 % PER)
			param.k=75;param.e=3;
			break;
		case 14: // 4 in 100 (4 % PER)
			param.k=100;param.e=4;
			break;
		case 15: // 3 in 100 (3 % PER)
			param.k=100;param.e=3;
			break;
		case 16: // 1 in 50 (2 % PER)
			param.k=50;param.e=1;
			break;
		case 17: // 2 in 100 (2 % PER)
			param.k=100;param.e=2;
			break;
		case 18: // 1 in 100 (1 % PER)
			param.k=100;param.e=1;
			break;
	}
}

void Self_Adaptation_MIMO_OFDM::determine_feature_dimensions()
{
	switch (param.psidx)
	{
		case 0: // MCS 0-7, 1 dimensions per feature
			if (param.ndbs>0)
				delete [] dimarray; 
			param.ndbs=8; // number of separate databases
			dimarray = new int[param.ndbs];
			for (int dbidx=0; dbidx<param.ndbs; dbidx++)
				dimarray[dbidx]=1;
			psstrm="1 1 1 1 1 1 1 1";
			pscost="6.5 13 19.5 26 39 52 58.5 65";
			exmap="1 2 3 4 5 6 7 7"; 
			break;
		case 1: // MCS 0-15, 1 dimensions per feature
			if (param.ndbs>0)
				delete [] dimarray;
			param.ndbs=16; // number of separate databases
			dimarray = new int[param.ndbs];
			for (int dbidx=0; dbidx<param.ndbs; dbidx++)
				dimarray[dbidx]=1;
			psstrm="1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2";
			pscost="6.5 13 19.5 26 39 52 58.5 65 13 26 39 52 78 104 117 130"; 
			exmap="1 2 3 4 5 6 7 7 9 10 11 12 13 14 15 15;8 9 9 10 11 12 12 12 9 10 11 12 13 14 15 15"; 
			break;
		case 2: // MCS 0-7, 2 dimensions per feature
			if (param.ndbs>0)
				delete [] dimarray; 
			param.ndbs=8; // number of separate databases
			dimarray = new int[param.ndbs];
			for (int dbidx=0; dbidx<param.ndbs; dbidx++)
				dimarray[dbidx]=2;
			psstrm="1 1 1 1 1 1 1 1";
			pscost="6.5 13 19.5 26 39 52 58.5 65"; 
			exmap="1 2 3 4 5 6 7 7"; 
			break;
		case 3: // MCS 0-15, 2 dimensions per feature
			if (param.ndbs>0)
				delete [] dimarray;
			param.ndbs=16; // number of separate databases
			dimarray = new int[param.ndbs];
			for (int dbidx=0; dbidx<param.ndbs; dbidx++)
				dimarray[dbidx]=2;
			psstrm="1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2";
			pscost="6.5 13 19.5 26 39 52 58.5 65 13 26 39 52 78 104 117 130"; 
			exmap="1 2 3 4 5 6 7 7 9 10 11 12 13 14 15 15;8 9 9 10 11 12 12 12 9 10 11 12 13 14 15 15"; 
			break;
		case 4: // MCS 0-7, 4 dimensions per feature
			if (param.ndbs>0)
				delete [] dimarray; 
			param.ndbs=8; // number of separate databases
			dimarray = new int[param.ndbs];
			for (int dbidx=0; dbidx<param.ndbs; dbidx++)
				dimarray[dbidx]=4;
			psstrm="1 1 1 1 1 1 1 1";
			pscost="6.5 13 19.5 26 39 52 58.5 65"; 
			exmap="1 2 3 4 5 6 7 7"; 
			break;
		case 5: // MCS 0-15, 4 dimensions per feature
			if (param.ndbs>0)
				delete [] dimarray;
			param.ndbs=16; // number of separate databases
			dimarray = new int[param.ndbs];
			for (int dbidx=0; dbidx<param.ndbs; dbidx++)
				dimarray[dbidx]=4;
			psstrm="1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2";
			pscost="6.5 13 19.5 26 39 52 58.5 65 13 26 39 52 78 104 117 130"; 
			exmap="1 2 3 4 5 6 7 7 9 10 11 12 13 14 15 15;8 9 9 10 11 12 12 12 9 10 11 12 13 14 15 15"; 
			break;
	}
}

int Self_Adaptation_MIMO_OFDM::explore_mapping(int parameter_index)
{
	int ns=exmap.rows();
	RNG_randomize();
	return exmap(randi(0,ns-1),parameter_index);	
}

void Self_Adaptation_MIMO_OFDM::compute_post_processing_snr(Array<cmat> const& channel_observed, double N0, Array<mat>& snr_values)
{
	int Nst=channel_observed.size();
	int Ntx=channel_observed(0).cols();
	snr_values.set_size(max(psstrm));
	int Ns=std::min(channel_observed(0).rows(),channel_observed(0).cols());
	Ns=std::min(Ns,max(psstrm));
	vec stream_inv_snr=zeros(Ns);
	int strmidx;
	cmat precoding_matrix;
	for (strmidx=0; strmidx<Ns; strmidx++)
	{ 
		snr_values(strmidx).set_size(strmidx+1,Nst);
		for (int subidx=0; subidx<Nst; subidx++)
		{
			switch (param.preidx)
			{
				case 0: // antenna selection
					precoding_matrix=antenna_selection_precoding(Ntx,strmidx+1);
					break;
				case 1: // eigenmode precoding
					precoding_matrix=eigenmode_precoding(channel_observed(subidx),strmidx+1);
					break;
				case 2: // cyclic delay precoding
					precoding_matrix=cyclic_delay_precoding(Ntx,strmidx+1,subidx,Nst,param.fftsz);
		//			cout << "pre" << strmidx << subidx << "=" << precoding_matrix << endl;
					break;	
			}
			switch (param.eqidx)
			{
				case 0: // MRC (1 stream), ZF (2+ stream)
					stream_inv_snr=diag(real(inv(hermitian_transpose(channel_observed(subidx)*precoding_matrix)*channel_observed(subidx)*precoding_matrix)));
					snr_values(strmidx).set_col(subidx,10*log10(elem_div(ones(strmidx+1),N0*stream_inv_snr)));
					break;
				case 1: // MMSE
					stream_inv_snr=diag(real(inv(hermitian_transpose(channel_observed(subidx)*precoding_matrix)*channel_observed(subidx)*precoding_matrix+N0*eye_c(Ns))));
					snr_values(strmidx).set_col(subidx,10*log10(elem_div(ones(strmidx+1),N0*stream_inv_snr)-1));
					break;
			}
		}
	}
	if (max(psstrm)>Ns)
	{
		for (strmidx=Ns; strmidx<max(psstrm); strmidx++)
		{
			for (int subidx=0; subidx<Nst; subidx++)
			{
				snr_values(strmidx)=concat_vertical(snr_values(Ns-1),zeros(strmidx+1-Ns,Nst));
			}
		}
	}
}

void Self_Adaptation_MIMO_OFDM::compute_condition_number(Array<cmat> const& channel_observed, double* condition_numbers)
{
	int Ns=channel_observed(0).cols();
	int Nst=channel_observed.size();
	vec singvals(Ns);
	for (int subidx=0; subidx<Nst; subidx++)
	{
		singvals=svd(channel_observed(subidx));
		condition_numbers[subidx]=sqrt(sum(sqr(singvals)))/(1/min(singvals));
	}
}

cmat Self_Adaptation_MIMO_OFDM::antenna_selection_precoding(int num_of_tx_ants, int num_streams)
{
	switch(num_of_tx_ants*10+num_streams)
	{
		case 11:
			return eye_c(1);
		case 21:
			return "1;1";
		case 22:
			return eye_c(2);
		case 31:
			return "1;1;1";
		case 32:
			return "1,0;0,1;0.7071,0.7071";
		case 33:
			return eye_c(3);
		case 41:
			return "1;1;1;1";
		case 42:
			return "1,0;0,1;1,0;0,1";
		case 43:
			return "1,0,0;0,1,0;0,0,1;0.57735,0.57735,0.57735";
		case 44:
			return eye_c(4);	
	}
}

cmat Self_Adaptation_MIMO_OFDM::eigenmode_precoding(cmat const& channel_matrix, int num_streams)
{
	cmat U,V; vec s; // variables for SVD	
	svd(channel_matrix,U,s,V);
	cmat F=V.get_cols(0,num_streams-1);
	return F/max(abs(diag(F*hermitian_transpose(F)))); // normalized power per antenna
}

cmat Self_Adaptation_MIMO_OFDM::cyclic_delay_precoding(int num_of_tx_ants, int num_streams, int sub_index, int num_subs, int fft_size)
{
	// define cyclic shift matrix
	std::complex<double> j(0,1);
	cmat CSD=eye_c(num_of_tx_ants);
	for (int txidx=0; txidx<num_of_tx_ants; txidx++)
		CSD(txidx,txidx)=exp(-j*2*pi*double(subcarrier_mapping(num_subs)(sub_index))*cyclic_shift_vectors(num_subs)(txidx)/double(fft_size));
	// multiple cyclic shift matrix by antenna selection matrix
	return CSD*antenna_selection_precoding(num_of_tx_ants,num_streams);
}



