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

#ifndef INCLUDED_self_adaptation_mimo_ofdm_H
#define INCLUDED_self_adaptation_mimo_ofdm_H

#include <itpp/itbase.h>
#include <itpp/itstat.h>
#include <itpp/itsignal.h>
#include <iostream>
#include <vector>
#include <ANN/ANN.h>

/*!
 * \brief Self-Adaptation Parameters Object 
 *
 * This class defines all parameters to initialize Self_Adaptive_MIMO_OFDM
 * databases.  Self_Adaptation_Parameters can be passed to the constructor of
 * Self_Adaptive_MIMO_OFDM and can also be retrieved from Self_Adaptive_MIMO_OFDM
 * since that object maintains Self_Adaptation_Parameters as a private member.
 *
 */
class Self_Adaptation_Parameters
{
	public:
		/*!
   	 * \brief Default Constructor for Self_Adapation_Parameters
  	 */
		Self_Adaptation_Parameters() {ftidx=0;clidx=0;eqidx=0;ecidx=0;psidx=0;frlen=1000;dbsize=100;errdbsize=50;ndbs=0;k=10;e=1;fftsz=64;};
		
		/*!
 		 * \brief Self_Adaptation_Parameters: member list
 		 *
 		 */
		
		/*!
 		 * \brief Feature index characterizes the features to be extracted from
 		 * the wireless channel over all subcarriers.
 		 * 		ftidx=0 -> feature set includes average SNR over all subcarriers and 
 		 * 		spatial streams
 		 * 		ftidx=1 -> feature set includes average SNR over all subcarriers and 
 		 * 		spatial stream as well as condition number averaged over all subcarriers
 		 * 		ftidx=2 -> feature set uses subcarrier ordering (i.e. selecting a subset
 		 * 		of ordered post-processing snr values associated with a set of distinct spatial
 		 * 		streams and subcarriers
 		 */
		int ftidx; // feature index
		/*! 
     * \brief Classifier index determines how k-nearest neighbor is processed.
     * e is selected such that e/k approximates packet error rate
     *		clidx=0 -> k=10,e=1 (10% PER)
     */
		int clidx; // classifier index
		/*!
 		 * \brief Equalizer index determines what equalization will be used in the 
 		 * receiver (for post-processing SNR calculation)
 		 * 	eqidx=0 -> Zero Forcing Spatial Equalization (MRC for one stream)
 		 * 	eqidx=1 -> MMSE Spatial Equalization
 		 */ 	
		int eqidx; // equalizer index
		/*!
 		 * \brief Forward error correction mode
 		 *		ecidx=0 -> IEEE 802.11 standard BCC
 		 *		ecidx=1 -> IEEE 802.11n standard LDPC
 		 */ 	
		int ecidx; // forward error correction index
		/*!
 		 * \brief Parameter set index determines the physical layer parameters
 		 * as well as the dimensionality of the feature space 
 		 *		psidx=0 -> IEEE 802.11n MCS 0-7, 1 dimension per feature
 		 *		psidx=1 -> IEEE 802.11n MCS 0-15, 1 dimension per feature
 		 *		psidx=2 -> IEEE 802.11n MCS 0-7, 2 dimensions per feature
 		 *		psidx=3 -> IEEE 802.11n MCS 0-15, 2 dimensions per feature
 		 *		psidx=4 -> IEEE 802.11n MCS 0-7, 4 dimensions per feature
 		 *		psidx=4 -> IEEE 802.11n MCS 0-15, 4 dimensions per feature
 		 */	
		int psidx; // parameter set index
		/*!
 		 * \brief Frame Length determines the number of bytes in each physical
 		 * layer packet
 		 */ 
		int frlen; // frame length (Bytes)
		/*!
		 * \brief Precoding index determines which precoder used to determine
		 * post-processing SNR
		 * 		preidx=0 -> Antenna Selection
		 * 		preidx=1 -> Eigenmode Beamforming
		 * 		preidx=2 -> Cyclic Delay Diversity
		 */ 
		int preidx; // precoding index
		/*!
 		 * \brief Number of subcarriers used for data and pilots
 		 */
		int nsubs; // number of subcarriers
		/*!
 		 * \brief FFT size (>=nsubs)
 		 */
		int fftsz; // FFT size
		/*! 
 		 * \brief The total size of the dabase for each parameter index
 		 * (number of packet errors held + number of packet successes held)
 		 */
		int dbsize; // database size
		/*!
 		 * \brief The number of packet errors held in the database for each
 		 * parameter index
 		 */
		int errdbsize; // error database size
		/*!
 	   * \brief The number of databases (number of parameter indices)
 	   */ 	
		int ndbs; // number of databases
		/*!
 		 * \brief e/k approximates the packet error rate (defined by clidx)
 		 */
		int k,e; // e/k approximates PER
	 	
		/*!
   	 * \brief Print the parameters of the Self_Adapation_Parameters object
   	 *
   	 * \param out Output Format (e.g. std::cout)
  	 */
   	void print_parameters(std::ostream& out) { out << std::endl << "**** Self Adaptation Parameter List ****" << std::endl << std::endl; out << ">>> " << ftidx << "/" << clidx << "/" << eqidx << "/" << ecidx << "/" << psidx << "/" << frlen << "/" << preidx << "/" << nsubs << "/" << dbsize << "/" << errdbsize << "/" << ndbs << "/" << k << "/" << e << "/" << fftsz << "<<<" << std::endl; out << std::endl << "****************************************" << std::endl << std::endl;}
};


/*!
 * \brief Self-Adaptation for MIMO-OFDM object
 *
 * This object allows the MAC layer of a MIMO-OFDM Wireless Network to
 * implement self-adaptation through channel measurements obtained from 
 * the PHY layer  
 *
 */
class Self_Adaptation_MIMO_OFDM
{
	public:
		Self_Adaptation_MIMO_OFDM() {};
		Self_Adaptation_MIMO_OFDM(int feature_index, int classifier_index, int equalizer_index, int fec_index, int parameter_set_index, int frame_length, int number_of_subcarriers, int fft_size, int precoding_index);
		// constructor with parameter structure
		Self_Adaptation_MIMO_OFDM(Self_Adaptation_Parameters parameters) {param=parameters;} 
		~Self_Adaptation_MIMO_OFDM(); // destructor

    /*! \brief Get method for getting a pointer to local parameter object. */
    inline Self_Adaptation_Parameters * params() { return &param; }

    /*!
   	 * \brief Set/Get methods for adaptation modes
  	 *
     * Setup the adaptation parameters
     *
     * set_feature_space
     * get_feature_space
     * \param feature_index 0 -> one-dimensional SNR
     *                      1 -> ordered subcarrier post-processing SNR (see Daniels, Caramanis, and Heath)
     * 
     * set_classifier
     * get_classifier
     * \param classifier_index 0 -> 1-out-of-10-nearest-neighbor (see Daniels, Caramanis, and Heath)
     *                         1 -> 5-out-of-50-nearest-neighbor (see Daniels, Caramanis, and Heath)
     * 
     * set_equalizer
     * get_equalizer
	 	 * \param equalizer_index 0 -> Zero-Forcing with Soft Outputs
	 	 * 
	 	 * set_forward_error_correction
	 	 * get_forward_error_correction
	 	 * \param fec_index 0 -> IEEE 802.11n (133,171) with Round-Robin Spatial Parsing and Block Interleaving
	 	 * set_parameter_set
	 	 * get_parameter_set
	 	 * \param parameter_set_index 0 -> IEEE 802.11n MCS 00-07
	 	 * \param parameter_set_index 1 -> IEEE 802.11n MCS 00-15
	 	 * \param parameter_set_index 2 -> IEEE 802.11n MCS 00-23
	 	 * \param parameter_set_index 3 -> IEEE 802.11n MCS 00-31
  	 */
   	inline void set_feature_space (int feature_index) { param.ftidx=feature_index; }
   	inline int get_feature_space () { return param.ftidx; }
   	inline void set_classifier (int classifier_index) { param.clidx=classifier_index; }
   	inline int get_classifier () { return param.clidx; }
   	inline void set_equalizer (int equalizer_index) { param.eqidx=equalizer_index; }
   	inline int get_equalizer () { return param.eqidx; }
   	inline void set_forward_error_correction (int fec_index) { param.ecidx=fec_index; }
   	inline int get_forward_error_correction () { return param.ecidx; }
   	inline void set_parameter_set (int parameter_set_index) { param.psidx=parameter_set_index; }
   	inline int get_parameter_set () { return param.psidx; }
   	inline void set_frame_length (int frame_length) { param.frlen=frame_length; }
   	inline int get_frame_length () { return param.frlen; }
   	inline void set_number_of_subcarriers (int number_of_subcarriers) { param.nsubs=number_of_subcarriers; }
   	inline int get_number_of_subcarriers () { return param.nsubs; }
		inline void set_fft_size (int fft_size) { param.fftsz=fft_size; }
		inline int get_fft_size () { return param.fftsz; }
		inline void set_precoding (int precoding_index) { param.preidx=precoding_index; }
		inline int get_precoding () { return param.preidx; }
		inline void set_database_size (int database_size) { param.dbsize=database_size; }
		inline int get_database_size () { return param.dbsize; }	
		inline void set_database_errors (int errors_in_database) { param.errdbsize=errors_in_database; }
		inline int get_database_errors () { return param.errdbsize; }	
		inline int get_number_of_databases () { return param.ndbs; }
		inline int get_error_spaces_in_database (int parameter_index) { return errleft[parameter_index]; }
		inline int get_success_spaces_in_database (int parameter_index) { return ackleft[parameter_index]; }
		
		/*!
 		 *
 		 */ 		
		void extract_feature(itpp::mat const& post_process_snr, int param_index, itpp::vec& feature);
		void extract_features(itpp::Array<itpp::mat> const& post_process_snr, itpp::Array<itpp::vec>& features);
		void extract_features(itpp::Array<itpp::cmat> const& channel_query, double N0, itpp::Array<itpp::vec>& features);
		itpp::Array<itpp::vec> extract_features(itpp::Array<itpp::cmat> const& channel_query, double N0);


    /*!
 		 * \brief Find the neighbors and their distances
 		 */
		void find_database_neighbors(itpp::vec const& feature_query, int database_index, int k, int* neighbor_indices, double* neighbor_distances);

		/*!
   	 * \brief Query the current feature space database 
   	 *
     * Given a channel realization, determine the best parameter set index.
     * This parameter set index could be, for example, a modulation and 
     * coding scheme.
     *
     * \param channel_query array of complex matrices where the size of
     * the array is determined by the number of subcarriers and the size of
     * the matrix is Nr x Ns where Nr is the number of receive antennas and 
     * Ns is the number of spatial streams (independent spatial layers)
     * \param feature_query vector or real numbers containing the feature space; 
     * use of this input must be used with caution to ensure it matches internal
     * features
   	 */
   	int query_adaptation_database(itpp::Array<itpp::cmat> const& channel_query, double N0);
		int query_adaptation_database(itpp::Array<itpp::vec> const& feature_query);
		
		/*!
   	 * \brief Update the database
     *
     * Update the feature set database with new observations of the channel
     * \param channel_observed array of complex matrices where the size of
     * the array is determined by the number of subcarriers and the size of
     * the matrix is Nr x Ns where Nr is the n 4.34404,  0.602378umber of receive antennas and 
     * Ns is the number of spatial streams (independent spatial layers)
     * \param feature_pbserved vector or real numbers containing the feature space; 
     * use of this input must be used with caution to ensure it matches internal
     * features
     * \param parameter_index integer index of the parameter used (e.g. MCS) 
     * for the current channel/feature observation
     * \param is_error true indicates there was a frame error
   	 */
   	void update_database(itpp::Array<itpp::cmat> const& channel_observed, double N0, int parameter_index, bool is_error);
   	void update_database(itpp::mat const& post_process_snr, int param_index, bool is_error);
		void update_database(itpp::vec const& feature_observed, int parameter_index, bool is_error);
		
		/*!
 		 *
 		 */ 	
		void trigger_exploration_mode(bool is_explore_on) { explore=is_explore_on; }		

		/*!
 		 *
 		 */ 	
		void trigger_diversity_preservation_mode(bool is_fsdp_on) { preserve=is_fsdp_on; }		
		void trigger_diversity_preservation_mode(bool is_fsdp_on, double fsdp_upper_bound) { preserve=is_fsdp_on; fsdpub=fsdp_upper_bound;}		

		/*!
 		 *
 		 */ 		
		void initialize_database(int feature_index, int classifier_index, int equalizer_index, int fec_index, int parameter_set_index, int frame_length, int number_of_subcarriers, int database_size, int errors_in_database, int fft_size);
		void initialize_database(int database_size, int errors_in_database);

		/*!
 		 *
 		 */ 		
		void load_database(std::string file_name);
	
		/*!
 		 *
 		 */
		void save_database(std::string file_name); 		
		
		/*!
 		 *
 		 */
		int recommend_parameters(itpp::Array<itpp::cmat> const& channel, double N0);
		int recommend_parameters(itpp::Array<itpp::mat> const& post_process_snr);
		
		int arf_recommend_parameters();

		/*!
 		 *
 		 */
		int recommend_parameters_and_update(itpp::Array<itpp::cmat> const& channel, double N0, int param_used, bool is_error);
		int recommend_parameters_and_update(itpp::Array<itpp::mat> const& post_process_snr, int param_used, bool is_error);
		
		/*!
 		 *
 		 */
		void print_parameters(std::ostream& out);
		void print_data(std::ostream& out, bool print_entries);

	private:
		Self_Adaptation_Parameters param;
		std::vector<ANNbruteForce*> annobjarray;
		std::vector<ANNpointArray> entriesarray; // data entry array 
		void determine_feature_dimensions();
		void determine_classifier_parameters();
		void compute_post_processing_snr(itpp::Array<itpp::cmat> const& channel_observed, double N0, itpp::Array<itpp::mat>& snr_values);
		void compute_condition_number(itpp::Array<itpp::cmat> const& channel_observed, double* condition_numbers);
		int *dimarray, *errleft, *ackleft;
		itpp::vec pscost;
		itpp::ivec psstrm;
		time_t** secarray;
		itpp::Array<itpp::ivec> subcarrier_mapping;
		itpp::Array<itpp::vec> cyclic_shift_vectors;
		itpp::Array<itpp::ivec> subcarrier_ordering_set_8_taps;
		int total_errors(ANNidx* indices, int db_index);		
		int explore_mapping(int parameter_index);
	
		int min_time_index(time_t* seconds, int array_size)
		{
			int min_index=0;
	//		std::cout << "time[0]=" << seconds[0] << std::endl;
			for (int timeidx=1; timeidx<array_size; timeidx++)
			{
//				std::cout << "time[" << timeidx << "]=" << seconds[timeidx] << std::endl;
				if (seconds[timeidx]<seconds[min_index])
					min_index=timeidx;
			}
			return min_index;
		}
		int max_double_index(double* double_array, int array_size)
		{
			int max_index=0;
			for (int doubidx=1; doubidx<array_size; doubidx++)
			{
				if (double_array[doubidx]>double_array[max_index])
					max_index=doubidx;
			}
			if (double_array[max_index]<=0)
				return -1;
			else
				return max_index;
		}
		itpp::cmat antenna_selection_precoding(int num_of_tx_ants, int num_streams);
		itpp::cmat eigenmode_precoding(itpp::cmat const& channel_matrix, int num_streams);
		itpp::cmat cyclic_delay_precoding(int num_of_tx_ants, int num_streams, int sub_index, int num_subs, int fft_size);
		itpp::imat exmap;
		bool explore, preserve;
		double fsdpub;
		int nack_count, ack_count, current_mode, ackub, nackub;
};

#endif  /* INCLUDED_self_adaptation_mimo_ofdm_H */
