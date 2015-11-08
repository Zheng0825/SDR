/*!     
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
 * 	\date Saturday, February 17th, 2007
 *
 *	\author{Robert C. Daniels (rdaniels@ece.utexas.edu), Kien T. Truong
 *	(kientruong@ece.utexas.edu)}
 *
 * 	- \date{02/17/07} \note{Robert Daniels: Created  BF_MIMO_OFDM.h and
 * 	BF_MIMO_OFDM.cc}
 *   	- \date{03/11/07} \note{Kien Truong: Addded information related
 *   	matrix_SVD}
 *	- \date{06/25/07} \note{Robert Daniels: Changed class declaration so
 *	that the function calls provide channel rather than constructor; Changed
 *	commenting format to work with doxygen; Changed file and class name from
 *	BF_MIMO_OFDM.h/cc to Beamforming_MIMO_OFDM.h/cc; changed class names to
 *	follow suggested naming conventions}
 */

#ifndef BEAMFORMING_MIMO_OFDM_H
#define BEAMFORMING_MIMO_OFDM_H

#include <itpp/itbase.h>
#include <itpp/itstat.h>

//! \brief Class for handling beamforming and combining vectors in MIMO OFDM
//! systems.  Includes limited feedback codebooks and subcarrier clustering.
class Beamforming_MIMO_OFDM
{
	public:
		Beamforming_MIMO_OFDM();
		
		/*! 
		 *  \brief Compute the optimal beamforming vectors for each
		 *  subcarrier 
		 *
		 *  Given input complex channel data, this function
		 *  computes the optimal beamforming vectors for each
		 *  subcarrier of an OFDM symbol.
		 *
		 *  \param in_chan holds the complex channel matrices for each
		 *  subcarrier; the array dimensions equals the number of
		 *  subcarriers while each matrix dimension has number of
		 *  columns equal to number of spatial streams and number of
		 *  rows represents the number of receive antennas
		 *  \retval transmit_vectors private variable assigned as a
		 *  result of function call
		 *  \retval receive_vectors private variable assigned as a
		 *  result of function call
		 *
		 */
		void compute_optimal_vectors(itpp::Array<itpp::cmat> const& in_chan);
	
		/*! 
		 *  \brief Set up the codebook to be used in limited feedback
		 *  beamforming and assign appropriate codes to each subcarrier 
		 * 
		 *  Given a codebook and the channel matrices this function
		 *  computes the limited feedback code index for each subcarrier
		 *  of an OFDM symbol.
		 *
		 *  \param in_chan holds the complex channel matrices for each
		 *  subcarrier; the array dimensions equal the number of
		 *  subcarriers while each matrix dimension has number of
		 *  columns equal to number of spatial streams and number of
		 *  rows represents the number of receive antennas
		 *  \param codebook_index is integer index of the limited feedback
		 *  codebook: 0 = Grassmanian, 1 = ...
		 *  \param codebook_bits is the integer representing the number
		 *  of bits for each subcarrier codeword
		 *  \retval get_code_indices holds the limited feedback
		 *  beamforming vector index for each subcarrier
		 *
		 * \par References:
		 *  D. J. Love, R. W. Heath, Jr.,  and T. Strohmer, ``Grassmannian Beamforming for Multiple-Input Multiple-Output Wireless Systems,'' IEEE Trans. on Info. Theory special issue on MIMO Communication, vol. 49, pp. 2735-2747, Oct. 2003.
		 */
		itpp::ivec compute_code_indices(itpp::Array<itpp::cmat> const& in_chan, unsigned int codebook_index, unsigned int codebook_bits);

		/*!
		 *  \brief Set up the codebook to be used in limited feedback
		 *  beamforming for clustered subcarriers and assign the
		 *  appropriate code to each cluster
		 *
		 *  Given a codebook and the channel matrices this function
		 *  computes the limited feedback code index or each cluster of
		 *  subcarriers using various clustering metrics.	
		 *
		 *  \param in_chan holds the complex channel matrices for each
		 *  subcarrier; the array dimensiona equal the number of
		 *  subcarriers while each matrix dimension has number of
		 *  columns equal to the number of spatial streams and the
		 *  number of rows representes the number of receive antennas
		 *  \param codebook_index is the integer index of the limited
		 *  feedback codebook: 0=Grassmanian, 1=...
		 *  \param codebook_bits is the integer representing the number
		 *  of bits for each subcarrier codeword
		 *  \param cluster_size is an integer representing the number of
		 *  subcarriers contained within each cluster
		 *  \retval cluster_metric is an integer representing the
		 *  cluster selection metric: 0 = maximize total cluster SNR, 1
		 *  = maximize minimum SNR per subcarrier in cluster, 2 = ... 
		 * 
		 *  \par References: 
		 *  B. Mondal  and  R. W. Heath, Jr., ``Algorithms for Quantized Precoding for MIMO OFDM Beamforming Systems,'' Proc. of Third SPIE Int. Symp. on Fluctuations and Noise, Vol. 5847, p. 80-87, Austin, May 23-26, 2005.
		 */
		itpp::ivec compute_clustered_code_indices(itpp::Array<itpp::cmat> in_chan, unsigned int codebook_index, unsigned int codebook_bits, unsigned int cluster_size, unsigned int cluster_metric);
		
		/*!
		 *  \brief Compute the limited feedback vectors based on the
		 *  codebook index  
		 * 
		 *  Given a code index in the initialized codebook this function 
		 *  computes the beamforming vectors for each subcarrier
		 *
		 *  \par 2nd Overloaded Function
		 *
		 *  \param codebook_index contains the integer index of the
		 *  codebook: 0 = Grassmanian, 1 = ...
		 *  \param codebook_bits contains the integer number of bits
		 *  used to represent each subcarrier codeword
		 *  \param code_indices is an IT++ integer vector with the
		 *  length equal to number of subcarriers that contains the code
		 *  index for each subcarrier
		 *  \param num_tx contains integer number of transmit antennas
		 *  \param num_rx contains integer number of receive antennas
		 *  result of function call
		 *  \retval receive_vectors is a private variable assigned as a
		 *  result of function call
		 *  \retval transmit_vectors is a private variable assigned as a
		 *  result of function call
		 *  
		 *  \par 1st Overloaded Function
		 *
		 *  \param codebook_index contains the integer index of the
		 *  codebook: 0 = Grassmanian, 1 = ...
		 *  \param codebook_bits contains the integer number of bits
		 *  used to represent each subcarrier codeword
		 *  \param code_indices is an IT++ integer vector with the
		 *  length equal to number of subcarriers that contains the code
		 *  index for each subcarrier
		 *  \param in_chan holds the channel matrix for each subcarrier
		 *  (not needed in second overloaded call)
		 *  \retval transmit_vectors is a private variable assigned as a
		 *  result of function call
		 *
		 */
		void compute_limited_feedback_vectors(unsigned int codebook_index, unsigned int codebook_bits, itpp::ivec code_indices, itpp::Array<itpp::cmat> const& in_chan);
		//! \overload
		void compute_limited_feedback_vectors(unsigned int codebook_index, unsigned int codebook_bits, itpp::ivec code_indices, unsigned int num_tx, unsigned int num_rx);

		/*!
		 *  \brief Compute the beamforming and combining vectors based
		 *  on clustered code indices
		 *
		 *  Given the clustered subcarrier code indices, compute the
		 *  beamforming vectors for the transmitter and the combining
		 *  vectors for the receiver
		 *  clustered beamforming vectors. 	
		 *  
		 *  \par 2nd Overloaded Function
		 *
		 *  \param codebook_index sets the codebook used to determine
		 *  the codewords for the vectors: 0 = Grassmanian, 1 = ...
		 *  \param codebook_bits defines the integer number of bits to
		 *  represent the beamforming/combining vector for each cluster
		 *  \param code_indices is an IT++ integer vector containing the
		 *  code index for each transmit beamforming vector
		 *  \param num_tx is the integer number of transmit antennas
		 *  \param num_rx is the integer number of receive antennas
		 *  \param num_sc is the integer number of subcarriers
		 *  \retval transmit_clustered_vectors private variable
		 *  assigned as a result of function call
		 *
		 *  \par 1st Overloaded Function
		 * 	
		 *  \param codebook_index sets the codebook used to determine
		 *  the codewords for the vectors: 0 = Grassmanian, 1 = ...
		 *  \param codebook_bits defines the integer number of bits to
		 *  represent the beamforming/combining vector for each cluster
		 *  \param code_indices is an IT++ integepapers/2005/asil/r vector containing the
		 *  code index for each transmit beamforming vector
		 *  subcarriers contained within each cluster
		 *  \param in_chan is an IT++ array of complex matrices
		 *  \retval transmit_clustered_vectors private variable
		 *  assigned as a result of function call
		 *  \retval receive_vectorsclustered_vectors private variable assigned
		 *  as a result of function call
		 *  representing the channel matrix for each subcarrier
		 * 
		 *  \par References:
		 *  B. Mondal  and  R. W. Heath, Jr., ``Algorithms for Quantized Precoding for MIMO OFDM Beamforming Systems,'' Proc. of Third SPIE Int. Symp. on Fluctuations and Noise, Vol. 5847, p. 80-87, Austin, May 23-26, 2005.
		 */
		void compute_clustered_vectors(unsigned int codebook_index, unsigned int codebook_bits, itpp::ivec code_indices, itpp::Array<itpp::cmat> const& in_chan);
		//! \overload
		void compute_clustered_vectors(unsigned int codebook_index, unsigned int codebook_bits, itpp::ivec code_indices, unsigned int num_tx, unsigned int num_rx, unsigned int num_sc);
		
		/*! 
		 *  \brief Interpolate the beamforming vectors to reduce
		 *  feedback
		 *
		 *  Given the size of the number of interpolation points \a
		 *  interpolation_size compute the interpolated beamforming
		 *  vectors.
		 *
		 *  \param interpolation_size integer represents the number of
		 *  subcarriers to interpolate between
		 *  \retval transmit_interpolated_vectors private variable
		 *  assigned as a result of function call
		 *  \retval receive_interpolated_vectors private variable assigned
		 *  as a result of function call
		 *  \par References:
		 *  J. Choi  and R. W. Heath, Jr., ``Interpolation Based Transmit Beamforming for MIMO-OFDM with Limited Feedback,'' IEEE Trans. on Signal Processing, vol. 53, no. 11, pp. 4125-4135, Nov. 2005.
		 */
		void compute_interpolated_vectors(unsigned int interpolation_size);

		/*!
		 *  \brief Get method for retreiving the beamforming vectors to
		 *  be used in the transmitter
		 *
		 *  \retval transmit_vectors is returned by this function call
		 */
		inline itpp::Array<itpp::cvec> get_transmit_vectors() { return
transmit_vectors; }

		/*!
		 *  \brief Get method for retreiving the combining vectors to
		 *  be used in the receiver
		 *
		 *  \retval receive_vectors is returned by this function call
		 */
		inline itpp::Array<itpp::cvec> get_receive_vectors() { return
receive_vectors; }

		/*!
		 *  \brief Get method for retreiving the clustered beamforming
		 *  vectors to be used in the transmitter
		 *
		 *  \retval transmit_clustered_vectors is returned by this function call
		 */
		inline itpp::Array<itpp::cvec> get_transmit_clustered_vectors() { return
transmit_clustered_vectors; }

		/*!
		 *  \brief Get method for retreiving the clustered combining
		 *  vectors to be used in the receiver
		 *
		 *  \retval receive_clustered_vectors is returned by this function call
		 */
		inline itpp::Array<itpp::cvec> get_receive_clustered_vectors() { return
receive_clustered_vectors; }

		/*!
		 *  \brief Get method for retreiving the interpolated
		 *  beamforming vectors to be used in the transmitter
		 *
		 *  \retval transmit_interpolated_vectors is returned by this function call
		 */
		inline itpp::Array<itpp::cvec> get_transmit_interpolated_vectors() { return
transmit_interpolated_vectors; }

		/*!
		 *  \brief Get method for retreiving the combining vectors to
		 *  be used in the receiver
		 *
		 *  \retval receive_interpolated_vectors is returned by this function call
		 */
		inline itpp::Array<itpp::cvec> get_receive_interpolated_vectors() { return
receive_interpolated_vectors; }

	protected:
		/*! 
		 *  \brief Given the codebook index and the number of bits to
		 *  use in the codebook, determine the exact codebook entries
		 *
		 *  \param cb_index vectors{determines the codebook type to be
		 *  used in the vector creation (0=Grassmanian, 1=...)
		 *  \param cb_bits {determines the number of bits that
		 *  represent each codebook entry; relevant for limiting the
		 *  rate of the feedback channel}
		 *  \retval codebook {sets the private variable to hold the
		 *  current codebook that is generated by this private function
		 *  call}
		 * 
		 * \par References:
		 * D. J. Love's Grassmannian subspace packing page: http://cobweb.ecn.purdue.edu/~djlove/grass.html
		 */
		void generate_codebook(unsigned int cb_index, unsigned int cb_bits);		

		//! beamforming vectors for every subcarrier
		itpp::Array<itpp::cvec> transmit_vectors;
		
		//! the combining vectors for every subcarrier
		itpp::Array<itpp::cvec> receive_vectors;
		
		//! beamforming vectors for every clustered subcarrier
		itpp::Array<itpp::cvec> transmit_clustered_vectors;
		
		//! the combining vectors for every clustered subcarrier
		itpp::Array<itpp::cvec> receive_clustered_vectors;
		
		//! beamforming vectors for every interpolated subcarrier
		itpp::Array<itpp::cvec> transmit_interpolated_vectors;
	
		//! the combining vectors for every interpolated subcarrier
		itpp::Array<itpp::cvec> receive_interpolated_vectors;
		
		//! \a false = error in vector computation
		bool is_vector_valid; 		
		
		/*! 
		 *  \brief{Compute the singular value decomposition of each
		 *  channel matrix}
		 *
		 *  This function finds the eigenvalues of the correlation
		 *  matrix (H*H or HH*) of each channel.
		 *
		 *  \retval matrix_SVD {TX_eigenvectors: the eigenvector
		 *  belonging to each eigenvalue of H*H, papers/2005/asil/RX_eigenvectors
		 *  belonging to each eigenvalue of HH*}
		 */
		void matrix_SVD(itpp::Array<itpp::cmat> const& H);	
		
		//! number of subcarriers
		unsigned int Nsc;
		//! number of transmit antennas
		unsigned int Ntx;
		//! number of receive antennas
		unsigned int Nrx;
		//! number of spatial paths
		unsigned int Nmin;
		
		//! eigenvalues of correlation matrices 
		itpp::Array<itpp::vec> eigenvalues; 
		//! transmit eigenvectors of correlation matrices
		itpp::Array<itpp::cmat> tx_eigenvectors; 
		//! receive eigenvectors of correlation matrices
		itpp::Array<itpp::cmat> rx_eigenvectors; 

		//! A matrix that holds the current codebook
		itpp::cmat codebook; 
};
#endif
