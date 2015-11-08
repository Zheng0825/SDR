/*!  \file Precoding_MIMO_OFDM.h
 * \brief Header for "Precoding_MIMO_OFDM.cc"
 *
 * \author{Kien T. Truong (ktruong@ece.utexas.edu), Robert C. Daniels
 * (rcdaniels@ece.utexas.edu)}
 * \date 2007-12-27
 *
 * $LastChangedBy$
 * $LastChangedDate$
 * $LastChangedRevision$
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

#ifndef PRECODING_MIMO_OFDM_H
#define PRECODING_MIMO_OFDM_H

#include <itpp/itbase.h>
#include <itpp/itstat.h>

//! \brief Class for handling precoding MIMO OFDM
class Precoding_MIMO_OFDM
{
  public:
    /*!
     * \brief Constructor.
     */
    Precoding_MIMO_OFDM();
    
    /*!
     * \brief Destructor.
     */
    ~Precoding_MIMO_OFDM() {}
    
    /*!
     * \brief Compute the optimal precoding matrix for each subcarrier.
     *
     * Given input complex channel data, this function computes the optimal 
     * precoding matrix for each subcarrier of an OFDM symbol.
     *
     * \param in_chan holds the complex channel matrices for each subcarrier; 
     * the array dimensions equals the number of subcarriers while each matrix 
     * dimension has the number of columns equal to the number of spatial 
     * streams and the number of rows equal to the number of receive antennas
     * \retval precoding_matrices private variable assigned as a result of 
     * function call
     */
    void compute_optimal_matrices(itpp::Array<itpp::cmat> const& in_chan);
    
    /*!
     * \brief Set up the codebook to be used in limited feedback precoding and
     * assign appropriate codes to each subcarrier.
     *
     * Given a codebook and the channel matrices, this function computes the
     * limited feedback code index for each subcarrier of an OFDM symbol.
     *
     * \param in_chan holds the complex channel matrices for each subcarrier;
     * the array dimensions equals the number of subcarriers while each matrix
     * dimension has the number of columns equal to the number of transmit
     * antennas and the number of rows equal to the number of receive antennas
     * \param codebook_index is integer index of the limited feedback, 
     * codebook_index: 0=Grassmanian, 1=...
     * \param codebook_bits is the integer representing the number of bits for
     * each subcarrier codeword
     * \param criterion_type is an integer representing the type of precoding
     * design creterion: 0=maximize capacity, 1=minimizen the determinant of
     * mean squared error, 2=minimize the trace of mean squared error, 
     * 3=minimize the minimum singular value
     * \param pc_mode is the integer representing the number of spatial streams
     * to be used
     *
     * \retval code_indices is a private variable holding the limited feedback
     * precoding matrix index for each subcarrier
     */
    void compute_code_indices(itpp::Array<itpp::cmat> const& in_chan, unsigned int codebook_index, unsigned int codebook_bits, unsigned int criterion_type, unsigned int pc_mode);

    /*!
     * \brief Set up the codebook to be used in limited feedback precoding for
     * clustered subcarriers and assign the appropriate code for each cluster.
     * 
     * Given a codebook and the channel matrices, this function computes the
     * limited feedback code index for each cluster of subcarriers using various
     * clustering metrics.
     *
     * \param in_chan holds the complex channel matrices for each subcarriers;
     * the array dimensions equal the number of subcarriers while each matrix
     * dimension has the number of columns equal to the number of transmit
     * antennas and the number of rows equal to the number of receive antennas
     * \param codebook_index is the integer index of the limited feedback
     * codebook: 0 = Grassmanian, 1 = ...
     * \param codebook_bits is the integer representing the number of bits for
     * each subcarrier codeword
     * \param pc_mode is the integer representing the number of spatial streams
     * to be used
     * \param cluster_size is the integer representing the number of 
     * subcarriers contained within each cluster
     * \param cluster_metric is the integer representing the cluster selection
     * metric: 0=maximize the total capacity, 1=...
     *
     * \retval clustered_code_indices is a private variable holding the limited
     * feedback precoding matrix index for each cluster
     */
    void compute_clustered_code_indices(itpp::Array<itpp::cmat>const& in_chan, unsigned int codebook_index, unsigned int codebook_bits, unsigned int pc_mode, unsigned int cluster_size, unsigned cluster_metric);


    /*!
     * \brief Set up the codebook to be used in limited feedback multimode 
     * precoding for clustered subcarriers and assign the appropriate code for
     * each cluster.
     * 
     * Given some information about the codebook and the channel matrices, this
     * function computes the optimal mode, the limited feedback code index for
     * each subcarrier using various selection criteria.
     *
     * \param in_chan holds the complex channel matrices for each subcarriers;
     * the array dimensions equal the number of subcarriers while each matrix
     * dimension has the number of columns equal to the number of transmit
     * antennas and the number of rows equal to the number of receive antennas
     * \param codebook_index is the integer index of the limited feedback
     * codebook: 0 = Grassmanian, 1 = ...
     * \param codebook_bits is the integer representing the number of bits for
     * each subcarrier codeword
     * \param selection_criterion is the integer representing the selection
     * criterion to be used: 0=capacity, 1=probability of error
     * \param modes is the integer vector containing the set of modes to be
     * considered
     *
     * \retval multimode_code_indices is a private variable holding the limited
     * feedback multimode precoding matrix index for each subcarrier
     * \retval optimal_mode is a private variabl holding the optimal mode for
     * each subcarrier in the calculation of limited feedback multimode 
     * precoding matrix
     *
     * \par References:
     * D. J. Love, R. W. Heath, Jr., "Multimode Precoding for MIMO Wireless
     * Systems", IEEE Transactions on Signal Processing, Vol. 53, No. 10, pp.
     * 3674-3687, Oct. 2005
     */
    void compute_multimode_code_indices(itpp::Array<itpp::cmat> const& in_chan, unsigned int codebook_index, unsigned int codebook_bits, unsigned int selection_criterion, itpp::ivec modes);


    /*!
     * \brief Compute the limited feedback matrices based on the codebook index.
     * 
     * Given a code index in the initialized codebook, this function computes
     * the precoding matrices for each subcarrier.
     *
     * \par 1st Overloaded Function
     *
     * \param codebook_index contains the integer index of the codebook:
     * 0=Grassmanian, 1=...
     * \param codebook_bits is the integer number of bits used to represent
     * each subcarrier codeword
     * \param in_chan holds the channel matrix for each subcarrier (not needed
     * in 2nd overloaded function)
     * \param pc_mode is the integer representing the number of spatial streams
     * to be used
     *
     * \retval precoding_matrices is a private variable assigned as a result of
     * the function call
     */
    void compute_limited_feedback_matrices(unsigned int codebook_index, unsigned int codebook_bits, itpp::Array<itpp::cmat> const& in_chan, unsigned int pc_mode);


    /*!
     * \brief Compute the limited feedback matrices based on the codebook index.
     * 
     * Given a code index in the initialized codebook, this function computes
     * the precoding matrices for each subcarrier.
     *
     * \overload
     *
     * \par 2nd Overloaded Function
     *
     * \param codebook_index contains the integer index of the codebook:
     * 0=Grassmanian, 1=...
     * \param codebook_bits is the integer number of bits used to represent
     * each subcarrier codeword
     * \param num_tx contains the integer number of transmit antennas
     * \param num_rx contains the integer number of receive antennas
     * \param pc_mode is the integer representing the number of spatial streams
     * to be used
     * \retval precoding_matrices is a private variable assigned as a result of
     * the function call
     */
    void compute_limited_feedback_matrices(unsigned int codebook_index, unsigned int codebook_bits, unsigned int num_tx, unsigned int num_rx, unsigned int pc_mode);


    /*!
     * \brief Compute the limited feedback matrices based on the clustered 
     * codebook indices.
     * 
     * Given the clustered subcarrier code indices in the initialized codebook,
     * this function computes the precoding matrices for each clustered 
     * subcarriers.
     *
     * \par 1st Overloaded Function
     *
     * \param codebook_index contains the integer index of the codebook:
     * 0=Grassmanian, 1=...
     * \param codebook_bits is the integer number of bits used to represent
     * each clustered subcarrier codeword
     * \param in_chan holds the channel matrix for each subcarrier (not needed
     * in 2nd overloaded function)
     * \param pc_mode is the integer representing the number of spatial streams
     * to be used
     * \param cluster_size is the integer representing the number of subcarriers
     * within each cluster
     *
     * \retval precoding_clustered_matrices is a private variable assigned as a
     * result of the function call
     */
    void compute_clustered_matrices(unsigned int codebook_index, unsigned int codebook_bits, itpp::Array<itpp::cmat> const& in_chan, unsigned int pc_mode, unsigned int cluster_size);

    /*!
     * \brief Compute the limited feedback matrices based on the clustered 
     * codebook indices.
     * 
     * Given the clustered subcarrier code indices in the initialized codebook,
     * this function computes the precoding matrices for each clustered 
     * subcarriers.
     *
     * \overload
     *
     * \par 2nd Overloaded Function
     *
     * \param codebook_index contains the integer index of the codebook:
     * 0=Grassmanian, 1=...
     * \param codebook_bits is the integer number of bits used to represent
     * each clustered subcarrier codeword
     * \param num_tx contains the integer number of transmit antennas
     * \param num_rx contains the integer number of receive antennas
     * \param num_sc contains the integer number of subcarriers
     * \param pc_mode is the integer representing the number of spatial streams
     * to be used
     * \param cluster_size is the integer representing the number of subcarriers
     * within each cluster
     *
     * \retval precoding_clustered_matrices is a private variable assigned as a
     * result of the function call
     */
    void compute_clustered_matrices(unsigned int codebook_index, unsigned int codebook_bits, unsigned int num_tx, unsigned int num_rx, unsigned int num_sc, unsigned int pc_mode, unsigned int cluster_size);


    /*!
     * \brief Compute the limited feedback multimode precoding matrices based
     * on the codebook index and the optimal modes.
     * 
     * Given code index in the initialized codebooks and the optimal modes
     * calculated by the function call compute_multimode_code_indices(),
     * this function computes the limited feedback multimode precoding matrices
     * for each subcarrier.
     *
     * \par 1st Overloaded Function
     *
     * \param codebook_index contains the integer index of the codebook:
     * 0=Grassmanian, 1=...
     * \param codebook_bits is the integer number of bits used to represent
     * each subcarrier codeword
     * \param in_chan holds the channel matrix for each subcarrier (not needed
     * in 2nd overloaded function)
     *
     * \retval multimode_precoding_matrices is a private variable assigned as a
     * result of the function call
     */
    void compute_multimode_matrices(unsigned int codebook_index, unsigned int codebook_bits, itpp::Array<itpp::cmat> const& in_chan);

    
    /*!
     * \brief Compute the limited feedback multimode precoding matrices based
     * on the codebook index and the optimal modes.
     * 
     * Given code index in the initialized codebooks and the optimal modes
     * calculated by the function call compute_multimode_code_indices(),
     * this function computes the limited feedback multimode precoding matrices
     * for each subcarrier.
     *
     * \overload
     *
     * \par 2nd Overloaded Function
     *
     * \param codebook_index contains the integer index of the codebook:
     * 0=Grassmanian, 1=...
     * \param codebook_bits is the integer number of bits used to represent
     * each subcarrier codeword
     * \param num_tx contains the integer number of transmit antennas
     * \param num_rx contains the integer number of receive antennas
     * \param num_sc contains the integer number of subcarriers
     *
     * \retval multimode_precoding_matrices is a private variable assigned as a
     * result of the function call
     */    
    void compute_multimode_matrices(unsigned int codebook_index, unsigned int codebook_bits, unsigned int num_tx, unsigned int num_rx, unsigned int num_sc);


    /*!
     * \brief Given the bit rate, this function computes the set of potential
     * modes. Specifically, for each channel use, R bits are demultiplexed into
     * M different bit streams. M is referred to as the mode of the precoder. A
     * necessary condition that motivates using only a subset of {1,...,Ntx} is
     * that R/M is an integer.
     *
     * \param bit_rate is the bit rate R
     * \param num_tx is the number of transmit antennas
     *
     * \retval mode_set is the set of potential modes
     *
     * \par References:
     * D. J. Love, R. W. Heath, Jr., "Multimode Precoding for MIMO Wireless
     * Systems", IEEE Transactions on Signal Processing, Vol. 53, No. 10, pp.
     * 3674-3687, Oct. 2005
     */
    itpp::ivec compute_mode_set(unsigned int bit_rate, unsigned int num_tx);
    

    /*!
     * \brief Get method for retrieving the code index as used at rx for sending
     * back
     *
     * \retval code_indices is returned by this function call
     */
    inline itpp::ivec get_code_indices() {return code_indices;}


    /*!
     * \brief Get method for retrieving the clustered code index as used at rx
     * for sending back
     *
     * \retval clustered_code_indices is returned by this function call
     */
    inline itpp::ivec get_clustered_code_indices() {return clustered_code_indices;}


    /*!
     * \brief Get method for retrieving the multimode code index as used at rx
     * for sending back
     *
     * \retval multimode_code_indices is returned by this function call
     */
    inline itpp::ivec get_multimode_code_indices() {return multimode_code_indices;}
    
    
    /*!
     * \brief Get method for retrieving the optimal mode for each subcarrier as
     * used at rx for sending back
     *
     * \retval optimal_mode is returned by this function call
     */
    inline itpp::ivec get_optimal_mode() {return optimal_mode;}



    /*!
     * \brief Get method for retrieving the precoding matrices to be used in
     * the transmitter.
     *
     * \retval precoding_matrices is returned by this function call
     */
    inline itpp::Array<itpp::cmat> get_precoding_matrices() {return precoding_matrices;}


    /*!
     * \brief Get method for retrieving the postcoding matrices to be used in
     * the receiver.
     * 
     * \retval postcoding_matrices is returned by this function call
     */
    inline itpp::Array<itpp::cmat> get_postcoding_matrices() {return postcoding_matrices;}


    /*!
     * \brief Get method for retrieving the precoding matrices to be used for
     * each cluster of subcarriers in the transmitter.
     *
     * \retval clustered_precoding_matrices is returned by this function call
     */
    inline itpp::Array<itpp::cmat> get_clustered_precoding_matrices() {return clustered_precoding_matrices;}


    /*!
     * \brief Get method for retrieving the postcoding matrices to be used for 
     * each cluster of subcarriers in the receiver.
     * 
     * \retval clustered_postcoding_matrices is returned by this function call
     */
    inline itpp::Array<itpp::cmat> get_clustered_postcoding_matrices() {return clustered_postcoding_matrices;}

    
    /*!
     * \brief Get method for retrieving the multimode precoding matrices to be
     * used for each subcarriers in the receiver.
     *
     * \retval multimode_precoding_matrices is returned by this function call
     */
    inline itpp::Array<itpp::cmat> get_multimode_precoding_matrices() {return multimode_precoding_matrices;}


  protected:
    /*!
     * \brief Given the codebook index and the number of bits to be used in the
     * codebook, this function determines the exact codebook entries.
     *
     * \param codebook_index is an integer which determines the codebook type
     * to be used in the matrix creation (0=Grassmanian, 1=...)
     * \param codebook_bits is an integer which determines the number of bits
     * that represents each codebook entry; relevant for limiting the rate of
     * the feedback channel
     * \retval codebook is an IT++ array of complex matrices, which is set as a
     * private variable to hold the current codebook that is generated by this
     * private function call
     */
    void generate_codebook(unsigned int codebook_index, unsigned int codebook_bits);

    /*!
     * \brief Given the channel matrix and the candidate precoding matrix, this
     * function calculate the precoding design criterion.
     *
     * \param criterion_type is an integer representing the type of precoding
     * design creterion: 0=maximization of capacity, 1=minimozation of the 
     * determinant of mean squared error, 2=minimization of trace of mean 
     * squared error, 3=minimization of the minimum singular value
     * \param H holds the matrix channel for one subcarrier
     * \param F contains the candidate precoding matrix
     * \param SNR is the signal-to-noise ratio, which is calculated by
     * \f[
     * SNR=Es/No
     * \f]
     * where Es is the signal energy and No is the noise power
     * \param mode is an integer representing the number of spatial streams to
     * be used
     *
     * \retval cost_function_value which need to be optimized based on the
     * criterion type
     */
    double design_criterion(unsigned int criterion_type, itpp::cmat H, itpp::cmat F, double SNR, unsigned int mode);


    /*!
     * \brief Given the channel matrix and the candidate precoding matrix, this
     * function calculate the precoding design criterion.
     *
     * \param cluster_num is the interger representing the cluster number of
     * the currently considered cluster of subcarriers
     * \param cluster_size is the integer representing the number of subcarriers
     * contained in each cluster of subcarrier
     * \param cluster_metric is the integer presenting the cluster selection
     * metric: 0=maximize total capacity, 1=...
     * \param in_chan holds the matrix channel for each subcarrier
     * \param F contains the candidate precoding matrix
     * \param SNR is the signal-to-noise ratio, which is calculated by
     * \f$
     * SNR = Es/No
     * \f$
     * where, Es is the signal energy and No is the noise power
     * \param mode is an integer representing the number of spatial streams to
     * be used
     *
     * \retval cost_function_value which need to be optimized based on the
     * criterion type
     */
    double clustered_design_criterion(unsigned int cluster_num, unsigned int cluster_size, unsigned int cluster_metric, itpp::Array<itpp::cmat>const& in_chan, itpp::cmat F, double SNR, unsigned int mode);


    /*!
     * \brief Compute the singular value decomposition of each channel matrix
     *
     * This function finds the eigenvalues and the corresponding eigenvectors 
     * of the correlation matrix (H*H or HH*) of each channel matrix
     *
     * \param H is a matrix
     * \retval eigenvalues is the vector of eigenvalues of correlation matrices
     * \retval tx_eigenvectors is the complex matrix containing the 
     * eigenvectors belonging to each eigenvalue of H*H
     * \retval rx_eigenvectors is the complex matrix containing the 
     * eigenvectors belonging to each eigenvalue of HH*
     */
    void matrix_SVD(itpp::Array<itpp::cmat> const& H);
    
    //! number of subcarriers
    unsigned int Nsc;
    //! number of transmit antennas
    unsigned int Ntx;
    //! number of receive antennas
    unsigned int Nrx;
    //! maximum number of modes or spatial paths
    unsigned int Nmin;
    //! mode of the precoder, i.e. the number of spatial data streams
    unsigned int mode;

    //! code_index for each subcarrier
    itpp::ivec code_indices;
    //! precoding matrices for every subcarriers
    itpp::Array<itpp::cmat> precoding_matrices;
    //! postcoding matrices for every subcarriers
    itpp::Array<itpp::cmat> postcoding_matrices;
    
    //! code index for each cluster
    itpp::ivec clustered_code_indices;
    //! precoding matrices for every cluster of subcarriers
    itpp::Array<itpp::cmat> clustered_precoding_matrices;
    //! postcoding matrices for every cluster of subcarriers
    itpp::Array<itpp::cmat> clustered_postcoding_matrices;

    //! optimal mode for each subcarrier
    itpp::ivec optimal_mode;
    //! multimode code index for each subcarrier
    itpp::ivec multimode_code_indices;  
    //! precoding matrices for every subcarrier
    itpp::Array<itpp::cmat> multimode_precoding_matrices;

    //! \a false = error in precoding matrix computation
    bool is_matrix_valid;    

    //! basic signal_to_noise ratio SNR = Es/No in absolute value
    double SNR;
    
    //! eigenvalues of correlation matrices of size Nmin x 1
    itpp::Array<itpp::vec> eigenvalues;
    //! transmit eigenvectors of correlation matrices of size Ntx x Ntx
    itpp::Array<itpp::cmat> tx_eigenvectors;
    //! receive eigenvectors of correlation matrices of size Nrx x Nrx
    itpp::Array<itpp::cmat> rx_eigenvectors;

    //! codebook represented by a matrix
    itpp::cmat codebook;    
    
};  //! end of precoding_MIMO_OFDM class
#endif /*PRECODING_MIMO_OFDM_H*/
