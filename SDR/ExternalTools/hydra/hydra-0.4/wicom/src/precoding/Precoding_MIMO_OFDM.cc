/*  \file
 *  \brief Precoding Matrix Creator for MIMO-OFDM Systems
 *  \author{Kien T. Truong (ktruong@ece.utexas.edu), Robert C. Daniels
 *  (rcdaniels@ece.utexas.edu)}
 *
 *  \date 2007-12-27
 *
 *  $LastChangedBy$
 *  $LastChangedDate$
 *  $LastChangedRevision$
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

#include "Precoding_MIMO_OFDM.h"

using namespace itpp;

using std::cout;
using std::endl;


Precoding_MIMO_OFDM::Precoding_MIMO_OFDM()
{
  // error checking until successful matrix creation
  is_matrix_valid = false;
  // \brief Set default parameters as 802.11n 2x2 system
  Ntx = 2; Nrx = 2; Nsc = 56; Nmin = 2;   
  // mode to be used
  mode = Nmin;
  // signal_to_noise ratio in absolute value
  SNR = 10;

  // initialize the precoding matrices
  precoding_matrices.set_size(Nsc,false);
  postcoding_matrices.set_size(Nsc,false);
  double init_value = 1/sqrt(Ntx*mode);  // normalization
  for(unsigned int a=0; a<Nsc; a++)
  {
    precoding_matrices(a) = init_value*ones_c(Ntx,mode);
    postcoding_matrices(a) = init_value*ones_c(Nrx,mode);
  }
}


void
Precoding_MIMO_OFDM::compute_optimal_matrices(itpp::Array<itpp::cmat> const& in_chan)
{
  Nrx = in_chan(0).rows();
  Ntx = in_chan(0).cols();
  Nsc = in_chan.size();
  Nmin = std::min(Ntx,Nrx);
  mode = Nmin;

  // Initialize the precoding matrices
  precoding_matrices.set_size(Nsc,false);
  postcoding_matrices.set_size(Nsc,false);
  for(unsigned int a=0; a<Nsc; a++)
  {
    precoding_matrices(a) = ones_c(Ntx,mode);
    postcoding_matrices(a) = ones_c(mode,Nrx);
  }

  // Check the system dimensions
  switch(Ntx*10+Nrx)
  {
    case 21: // 2 transmit antennas - 1 receive antenna - 1 spatial stream
    case 31: // 3 transmit antennas - 1 receive antenna - 1 spatial stream
    case 41: // 4 transmit antennas - 1 receive antenna - 1 spatial stream
      /* MISO system: matrices are indeed vectors, which are created below
       * based on maximal ratio transmission (MRC)
       */
      {
        double H_norm;
        cvec postcoding_matrix_trivial = ones_c(Nrx);
        for(unsigned int b=0; b<Nsc; b++)
        {
          H_norm = norm(in_chan(b));
          precoding_matrices(b) = in_chan(b)/H_norm;
          // trivial postcoding matrix
          postcoding_matrices(b) = postcoding_matrix_trivial;
        }
      }
      break;
    case 22: // 2 transmit antennas - 2 receive antennas - 2 spatial streams
    case 23: // 2 transmit antennas - 3 receive antennas - 2 spatial streams
    case 24: // 2 transmit antennas - 4 receive antennas - 2 spatial streams
    case 32: // 3 transmit antennas - 2 receive antennas - 2 spatial streams
    case 33: // 3 transmit antennas - 3 receive antennas - 3 spatial streams
    case 34: // 3 transmit antennas - 4 receive antennas - 3 spatial streams
    case 42: // 4 transmit antennas - 2 receive antennas - 2 spatial streams
    case 43: // 4 transmit antennas - 3 receive antennas - 3 spatial streams
    case 44: // 4 transmit antennas - 4 receive antennas - 4 spatial streams
      {
        // MIMO precoding matrix creation based on the eigenpaths
        matrix_SVD(in_chan);

        cout << "rx eigenvectors: " << rx_eigenvectors << endl;

        cmat temp_postcoding_matrix = ones_c(Nrx,mode);

        for(unsigned int c=0; c<Nsc; c++)
        {   
          /* assigns the optimal precoding and postcoding matrices for each
           * subcarrier
           */
          for(unsigned int d=0; d<mode; d++)
          {           
            precoding_matrices(c).set_col(d,tx_eigenvectors(c).get_col(d));
            
            temp_postcoding_matrix.set_col(d,rx_eigenvectors(c).get_col(d));
          }
          postcoding_matrices(c) = hermitian_transpose(temp_postcoding_matrix);
        }
        // unitary constraint normalization
        for(unsigned int e=0; e<Nsc; e++)
        {
          precoding_matrices(e) *= 1/sqrt(mode);
        }
      }
      break;
    default:
      {
        cout << "ERROR: System dimensions are not currently supported for optimal precoding matrix creation!" << endl;
        is_matrix_valid = false;
      }
      break;
  }
}


void
Precoding_MIMO_OFDM::compute_code_indices(itpp::Array<itpp::cmat> const& in_chan, unsigned int codebook_index, unsigned int codebook_bits, unsigned int criterion_type,unsigned int pc_mode)
{
  Nrx = in_chan(0).rows();
  Ntx = in_chan(0).cols();
  Nsc = in_chan.size();
  Nmin = std::min(Ntx,Nrx);

  code_indices = zeros_i(Nsc);

  // check the validity of pc_mode input variable
  if(pc_mode <= Nmin)
  {
    mode = pc_mode;
    generate_codebook(codebook_index, codebook_bits);

    cmat codeword = ones_c(Ntx,mode);

    vec cost_function_value; // holds cost function value for each subcarrier
    unsigned int num_codewords = codebook.rows()/Ntx; // number of codewords
    cost_function_value.set_size(num_codewords);

    // loop finds the precoding matrices for each subcarrier
    for(unsigned int a=0; a<Nsc; a++)
    {
      // loop calculates cost function value for each codeword
      for(unsigned int b=0; b<num_codewords; b++)
      {
        // loop determines the codeword
        for(unsigned int c=0; c<Ntx; c++)
        {
          codeword.set_row(c,codebook.get_row(c+b*Ntx));
        }       
        cost_function_value(b) = design_criterion(criterion_type,in_chan(a),codeword,SNR,mode);        
      }

      // find the optimized cost function value for each subcarrier
      unsigned int code_index = 0;
      switch(criterion_type)
      {
        case 0: // maximizes the capacity
        case 3: // maximizes the minimum singular value
          {
            code_index = max_index(cost_function_value);
          }
          break;
        case 1: // minimizes the determinant of mean squared error
        case 2: // minimizes the trace of mean squared error
          {
            code_index = min_index(cost_function_value);
          }
          break;
        default:
          {
            cout << "ERROR: the criterion type is not currently supported!" << endl;
            is_matrix_valid = false;
          }
          break;
      } // end of criterion_type switch
      code_indices(a) = code_index;
    }
  }
  else
  {
    cout << "ERROR: the pc_mode input variable exceeds the maximum potential mode!" << endl;
    is_matrix_valid = false;
  }
}


void
Precoding_MIMO_OFDM::compute_clustered_code_indices(itpp::Array<itpp::cmat>const& in_chan, unsigned int codebook_index, unsigned int codebook_bits, unsigned int pc_mode, unsigned int cluster_size, unsigned cluster_metric)
{
  Nrx = in_chan(0).rows();
  Ntx = in_chan(0).cols();
  Nsc = in_chan.size();
  Nmin = std::min(Ntx,Nrx);

  unsigned int num_clusters=ceil_i(double(Nsc)/double(cluster_size));

  clustered_code_indices = zeros_i(num_clusters);

  // check the validity of pc_mode input variable
  if(pc_mode <= Nmin)
  {
    mode = pc_mode;
    generate_codebook(codebook_index, codebook_bits);

    cmat codeword = ones_c(Ntx,mode);

    vec cost_function_value; // holds cost function value for each subcarrier
    unsigned int num_codewords = codebook.rows()/Ntx; // number of codewords
    cost_function_value.set_size(num_codewords);

    // loop finds the precoding matrices for each subcarrier
    for(unsigned int a=0; a<num_clusters; a++)
    {
      unsigned int cluster_num = a; // cluster number

      // loop calculates the cost function value for each codeword
      for(unsigned int b=0; b<num_codewords; b++)
      {
        // loop determines the index_list for the codeword
        for(unsigned int c=0; c<Ntx; c++)
        {
          codeword.set_row(c,codebook.get_row(c+b*Ntx));
        }
        cost_function_value(b) = clustered_design_criterion(cluster_num,cluster_size,cluster_metric,in_chan,codeword,SNR,mode);        
      }

      // find the optimized cost function value for each subcarrier
      unsigned int code_index = 0;
      switch(cluster_metric)
      {
        case 0: // maximizes the total capacity
          {
            code_index = max_index(cost_function_value);
          }
          break;
        case 1:
          break;
        default:
          {
            cout << "ERROR: the cluster selection metric is not currently supported!" << endl;
            is_matrix_valid = false;
          }
          break;
      } // end of cluster_metric switch
      clustered_code_indices(a) = code_index;
    }
  }
  else
  {
    cout << "ERROR: the pc_mode input variable exceeds the maximum potential mode!" << endl;
    is_matrix_valid = false;
  }
}


void
Precoding_MIMO_OFDM::compute_multimode_code_indices(itpp::Array<itpp::cmat> const& in_chan, unsigned int codebook_index, unsigned int codebook_bits, unsigned int selection_criterion, itpp::ivec modes)
{
  Ntx = in_chan(0).cols();
  Nrx = in_chan(0).rows();
  Nmin = std::min(Ntx,Nrx);
  Nsc = in_chan.size();  

  unsigned int num_modes = length(modes); // number of modes to be considered 

  multimode_code_indices.set_size(Nsc,false);
  optimal_mode.set_size(Nsc,false);

  switch(selection_criterion)
  {
    case 0: // capacity selection
      {
        unsigned int criterion_type = 0; // maximizes the capacity
        // loop determines the multimode precoding matrices for each  subcarrier
        for(unsigned int a=0; a<Nsc; a++)
        {
          ivec temp_mm_code_indices = zeros_i(num_modes);
          vec temp_mm_cost_function_value = zeros(num_modes);
          // loop calculates the precoding matrices corresponding to each mode 
          for(unsigned int b=0; b<num_modes; b++)
          {
            mode = modes(b);
            generate_codebook(codebook_index,codebook_bits);
            cmat codeword = ones_c(Ntx,mode);

            vec cost_function_value; // holds cost function value for each subcarrier
            unsigned int num_codewords = codebook.rows()/Ntx; // number of codewords
            cost_function_value.set_size(num_codewords);

            // loop calculates cost function value for each codeword
            for(unsigned int c=0; c<num_codewords; c++)
            {
              // loop determines the codeword
              for(unsigned int d=0; d<Ntx; d++)
              {
                codeword.set_row(d,codebook.get_row(d+c*Ntx));
              }
              cost_function_value(c) = design_criterion(criterion_type,in_chan(a),codeword,SNR,mode);              
            }
            temp_mm_code_indices(b) = max_index(cost_function_value);
            temp_mm_cost_function_value(b) = max(cost_function_value);
          }
          optimal_mode(a) = max_index(temp_mm_cost_function_value);
          multimode_code_indices(a) = temp_mm_code_indices(optimal_mode(a));
        }
      }
      break;
    case 1: // probability of error selection
      break;
    default:
      {
        cout << "ERROR: the selection criterion is not currently supported" << endl;
        is_matrix_valid = false;
      }
      break;
  } // end of selection_criterion switch
}


void
Precoding_MIMO_OFDM::compute_limited_feedback_matrices(unsigned int codebook_index, unsigned int codebook_bits, unsigned int num_tx, unsigned int num_rx, unsigned int pc_mode)
{
  Ntx = num_tx;
  Nrx = num_rx;
  Nmin = std::min(Ntx,Nrx);
 
  // check the validity of pc_mode value
  if(pc_mode <= Nmin)
  {
    mode = pc_mode;
    Nsc = length(code_indices);
    generate_codebook(codebook_index, codebook_bits);

    precoding_matrices.set_size(Nsc,false);

    for(unsigned int a=0; a<Nsc; a++)
    {
      // initialize the precoding matrices for each subcarrier
      precoding_matrices(a) = ones_c(Ntx,mode);
      // assigns the precoding matrices for each subcarrier
      for(unsigned int b=0; b<Ntx; b++)
      {
        precoding_matrices(a).set_row(b,codebook.get_row(b+code_indices(a)*Ntx));
      }
    }
  }
  else
  {
    cout << "ERROR: the pc_mode value exceeds the maximum potential mode!" << endl;
    is_matrix_valid = false;
  }
}


void
Precoding_MIMO_OFDM::compute_limited_feedback_matrices(unsigned int codebook_index, unsigned int codebook_bits, itpp::Array<itpp::cmat> const& in_chan, unsigned int pc_mode)
{
  Ntx = in_chan(0).cols();
  Nrx = in_chan(0).rows();
  Nmin = std::min(Ntx,Nrx);

  // check the validity of pc_mode value
  if(pc_mode <= Nmin)
  {
    mode = pc_mode;
    Nsc = length(code_indices);
    generate_codebook(codebook_index,codebook_bits);

    precoding_matrices.set_size(Nsc);
    
    for(unsigned int a=0; a<Nsc;a++)
    {
      // assigns the precoding matrices for each subcarrier
      for(unsigned int b=0; b<Ntx; b++)
      {
        precoding_matrices(a).set_row(b,codebook.get_row(b+code_indices(a)*Ntx));     
      }
    }
  }
  else
  {
    cout << "ERROR: the pc_mode value exceeds the maximum potential mode!" << endl;
    is_matrix_valid = false;
  }
}


void
Precoding_MIMO_OFDM::compute_clustered_matrices(unsigned int codebook_index, unsigned int codebook_bits, unsigned int num_tx, unsigned int num_rx, unsigned int num_sc, unsigned int pc_mode, unsigned int cluster_size)
{
  Ntx = num_tx;
  Nrx = num_rx;  
  Nmin = std::min(Ntx,Nrx);
  Nsc = num_sc;

  // check the validity of pc_mode value
  if(pc_mode <= Nmin)
  {
    mode = pc_mode;
    unsigned int num_clusters = length(clustered_code_indices);
    generate_codebook(codebook_index, codebook_bits);
    unsigned int cluster_size = ceil_i(double(Nsc)/double(num_clusters));
    clustered_precoding_matrices.set_size(Nsc,false);

    for(unsigned int a=0; a<num_clusters; a++)
    {
      unsigned int cluster_num = a; // cluster number
      // index of the first subcarrier in the cluster
      unsigned int start_subcarrier_index = cluster_num*cluster_size;
      // index of the last subcarrier in the cluster
      unsigned int end_subcarrier_index = std::min(Nsc-1,(cluster_num+1)*cluster_size);

      // loop determines the precoding matrix for each subcarrier in the cluster
      for(unsigned int b=start_subcarrier_index; b<=end_subcarrier_index; b++)
      {
        // initialize the precoding matrices for each subcarrier
        clustered_precoding_matrices(b) = ones_c(Ntx,mode);
        // assigns the precoding matrices for each subcarrier
        for(unsigned int c=0; c<Ntx; c++)
        {
          clustered_precoding_matrices(b).set_row(c,codebook.get_row(c+clustered_code_indices(a)*Ntx));
        }
      }
    }
  }
  else
  {
    cout << "ERROR: the pc_mode value exceeds the maximum potential mode!" << endl;
    is_matrix_valid = false;
  }
}


void
Precoding_MIMO_OFDM::compute_clustered_matrices(unsigned int codebook_index, unsigned int codebook_bits, itpp::Array<itpp::cmat> const& in_chan, unsigned int pc_mode, unsigned int cluster_size)
{
  Ntx = in_chan(0).cols();
  Nrx = in_chan(0).rows();
  Nmin = std::min(Ntx,Nrx);
  Nsc = in_chan.size();

  // check the validity of pc_mode value
  if(pc_mode <= Nmin)
  {
    mode = pc_mode;
    unsigned int num_clusters = length(clustered_code_indices);
    generate_codebook(codebook_index,codebook_bits);
    unsigned int cluster_size = ceil_i(double(Nsc)/double(num_clusters));
    clustered_precoding_matrices.set_size(Nsc,false);
    
    for(unsigned int a=0; a<num_clusters;a++)
    {
      unsigned int cluster_num = a; // cluster number
      // index of the first subcarrier in the cluster
      unsigned int start_subcarrier_index = cluster_num*cluster_size;
      // index of the last subcarrier in the cluster
      unsigned int end_subcarrier_index = std::min(Nsc-1,(cluster_num+1)*cluster_size);

      /* loop determines the precoding matrix for each subcarrier in the 
       * cluster
       */
      for(unsigned int b=start_subcarrier_index; b<=end_subcarrier_index; b++)
      {
        clustered_precoding_matrices(b) = ones_c(Ntx,mode);
        // assigns the precoding matrices for each subcarrier
        for(unsigned int c=0; c<Ntx; c++)          
        {
          clustered_precoding_matrices(b).set_row(c,codebook.get_row(c+clustered_code_indices(a)*Ntx));     
        }
      }
    }
  }
  else
  {
    cout << "ERROR: the pc_mode value exceeds the maximum potential mode!" << endl;
    is_matrix_valid = false;
  }
}


void
Precoding_MIMO_OFDM::compute_multimode_matrices(unsigned int codebook_index, unsigned int codebook_bits, itpp::Array<itpp::cmat> const& in_chan)
{
  Nrx = in_chan(0).rows();
  Ntx = in_chan(0).cols();
  Nsc = in_chan.size();
  Nmin = std::min(Ntx,Nrx);

  multimode_precoding_matrices.set_size(Nsc,false);

  for(unsigned int a=0; a<Nsc; a++)
  {
    mode = optimal_mode(a);
    generate_codebook(codebook_index,codebook_bits);
    // initialize the precoding matrices for each subcarrier
    precoding_matrices(a) = ones_c(Ntx,mode);
    // assigns the multimode precoding matrices for each subcarrier
    for(unsigned int b=0;b<Ntx;b++)
    {
      multimode_precoding_matrices(a).set_row(b,codebook.get_row(b+multimode_code_indices(a)*Ntx));
    }
  }
}


void
Precoding_MIMO_OFDM::compute_multimode_matrices(unsigned int codebook_index, unsigned int codebook_bits, unsigned int num_tx, unsigned int num_rx, unsigned int num_sc)
{
  Ntx = num_tx;
  Nrx = num_rx;
  Nmin = std::min(Ntx,Nrx);
  Nsc = num_sc;

  multimode_precoding_matrices.set_size(Nsc,false);

  for(unsigned int a=0; a<Nsc; a++)
  {
    mode = optimal_mode(a);
    generate_codebook(codebook_index,codebook_bits);
    // initialize the precoding matrices for each subcarrier
    precoding_matrices(a) = ones_c(Ntx,mode);
    // assigns the multimode precoding matrices for each subcarrier
    for(unsigned int b=0;b<Ntx;b++)
    {
      multimode_precoding_matrices(a).set_row(b,codebook.get_row(b+multimode_code_indices(a)*Ntx));
    }
  }
}


void
Precoding_MIMO_OFDM::generate_codebook(unsigned int codebook_index, unsigned int codebook_bits)
{
  switch(codebook_index)
  {
    case 0: // Grassmanian codebooks
      // Codebook construction depends on the system dimensions
      switch(Ntx*100+mode*10+codebook_bits)
      {
        // 2 transmit antennas - 1 spatial stream - 2 bit codebook
			  case 212:
				  codebook.set("-0.1612-0.7348i -0.5135-0.4128i; -0.0787-0.3192i -0.2506+0.9106i; -0.2399+0.5985i -0.7641-0.0212i; -0.9541 0.2996");
				  break;
			  // 2 transmit antennas - 1 spatial stream - 3 bit codebook
			  case 213: 
				  codebook.set("0.8393-0.2939i -0.1677+0.4256i; -0.3427+0.9161i 0.0498+0.2019i; -0.2065+0.3371i 0.9166+0.0600i; 0.3478-0.3351i 0.2584+0.8366i; 0.1049+0.6820i 0.6537+0.3106i; 0.0347-0.2716i 0.0935-0.9572i;-0.7457+0.1181i -0.4553-0.4719i; -0.7983+0.3232i 0.5000+0.0906i");
				  break;
			  // 2 transmit antennas - 1 spatial stream - 4 bit codebook
			  case 214:
				  codebook.set("-0.6865 0.4857+0.5412i;-0.8034 -0.0913+0.5885i;-0.5655 -0.1019-0.8184i;-0.7125 -0.6074-0.3514i;-0.8255 -0.1831-0.5338i;-0.4803 -0.0391+0.8762i;-0.5957 0.5508-0.5846i;-0.7995 -0.5835+0.1426i;-0.8478 0.2917-0.4429i;-0.8173 0.5748-0.0404i;-0.9794 -0.1997-0.0288i;-0.6328 -0.5940+0.4968i;0.1918 -0.9757-0.1055i;-0.3683 -0.9059-0.2090i;-0.6089 0.7893+0.0789i;-0.9545 0.2115+0.2101i");
				  break;
			  // 2 transmit antennas - 1 spatial stream - 5 bit codebook
			  case 215:
				  codebook.set("-0.7846 -0.4317-0.4450i;-0.7583 0.6461-0.0871i;-0.6386 -0.2088-0.7407i;-0.7104 0.1794-0.6806i;-0.4655 0.3752-0.8016i;-0.8872 -0.0554-0.4580i;-0.4411 -0.8910+0.1075i;-0.5470 0.7958+0.2597i;-0.8714 0.3255-0.3671i;-0.8849 -0.4046+0.2307i;-0.6897 -0.6735+0.2659i;-0.5742 0.7735-0.2684i;-0.3000 -0.4672-0.8317i;-0.7090 0.4990-0.4983i;-0.8691 0.2619+0.4195i;-0.7627 0.5739+0.2982i;-0.8360 -0.1028+0.5390i;-0.4772 -0.4081+0.7783i;-0.6968 -0.4915+0.5224i;-0.9209 0.3863+0.0520i;-0.5777 -0.6267-0.5229i;-0.6617 0.1089+0.7419i;-0.6876 0.4117+0.5981i;-0.9682 -0.0324+0.2480i;-0.1479 -0.4838+0.8626i;-0.9923 0.0517-0.1122i;-0.2295 0.9363-0.2657i;-0.4283 0.4065+0.8071i;-0.9387 -0.3059-0.1592i;-0.6696 -0.7343-0.1119i;-0.8193 -0.5652-0.0968i;-0.6779 -0.1948+0.7088i");
				  break;
			  // 3 transmit antennas - 1 spatial stream - 2 bit codebook
			  case 312:
				  codebook.set("0.7939+0.0590i -0.4126-0.0807i -0.0853-0.4269i; 0.2189+0.0654i 0.1844-0.3191i -0.8804-0.1921i; 0.3087-0.4341i -0.2454-0.6507i 0.4817+0.0258i; 0.5915-0.1175i 0.3113+0.6635i 0.2941+0.1128i");
				  break;
			  // 3 transmit antennas - 1 spatial stream - 3 bit codebook
			  case 313:
				  codebook.set("0.4170+0.0350i 0.2583+0.5635i 0.4826+0.4558i; -0.5521+0.2276i 0.2496+0.4156i -0.1060-0.6301i; 0.2148+0.0059i -0.4115-0.8788i -0.0625-0.0911i; 0.1814+0.3412i -0.7407+0.0188i 0.5024+0.2221i; 0.9126+0.1634i -0.2865+0.0983i -0.1510+0.1608i; 0.4883-0.5125i 0.4955-0.1385i -0.4266+0.2284i; 0.2020-0.4667i -0.1941+0.2372i -0.7891-0.1576i; 0.4294-0.1103i 0.1998-0.5339i 0.6693+0.1747i");
				  break;
        // 3 transmit antennas - 1 spatial stream - 4 bit codebook
			  case 314:
				  codebook.set("0.7274+0.3026i -0.1659+0.4141i -0.4227+0.0405i; 0.4118-0.2298i -0.4077+0.2244i 0.7207+0.2040i; 0.1600-0.0152i -0.0562-0.1872i 0.9478-0.1939i; 0.5793-0.3144i -0.2044+0.1672i 0.0451-0.7027i; 0.0562+0.2690i -0.6267+0.1016i -0.6783+0.2478i; 0.3820-0.1753i -0.5300-0.0226i -0.5920-0.4375i; -0.3962-0.1652i  -0.7409-0.1551i  -0.3035+0.3882i; -0.0021+0.5972i  0.6062-0.4902i  0.1047-0.1568i;  0.2134-0.8091i  0.1887-0.2612i -0.3483-0.2734i;  0.2383+0.4676i 0.0177+0.2231i -0.8036-0.1698i;  -0.7293-0.0393i -0.2017+0.1928i  0.4653+0.4150i; 0.1396+0.3488i 0.5725+0.2910i 0.0246+0.6676i; 0.0857+0.2018i  0.4897+0.2938i -0.6664-0.4264i;  0.8821+0.2361i  0.3456-0.0126i -0.1816+0.1166i; 0.3873-0.1979i -0.2215+0.8061i  0.3050+0.1375i; 0.5116+0.1282i 0.5332+0.6481i 0.0425+0.1255i");
				  break;
			  // 4 transmit antennas - 1 spatial stream - 4 bit codebook 
			  case 414:
				  codebook.set("-0.2832-0.3026i -0.4318+0.2306i -0.4143-0.0092i 0.6457; 0.7715 -0.1213+0.1684i 0.5813+0.0449i -0.1475-0.0049i; 0.0490+0.2645i 0.3039+0.1429i 0.1790+0.5045i 0.7268; 0.6851 0.1861+0.2946i -0.3828-0.2366i -0.1392+0.4328i; 0.5480-0.0557i  0.3246-0.1762i 0.0337-0.4986i 0.5572; 0.7318 -0.4010-0.3151i -0.3431-0.0759i -0.2387-0.1545i; -0.2314-0.2726i -0.0087-0.0200i 0.4820-0.3299i 0.7283; -0.2075-0.2030i -0.0327+0.0432i 0.8885 -0.3461+0.0595i; 0.0573+0.3799i -0.1914-0.5340i 0.6762 0.2247+0.1510i; -0.0681+0.1830i -0.0628-0.6066i -0.1660+0.0868i 0.7449; 0.6596 -0.3825+0.1394i -0.0604+0.1930i 0.5696+0.1838i; 0.6576 0.3519+0.2595i -0.2176+0.0574i -0.1051-0.5610i; 0.3039+0.5663i 0.6450 -0.3047+0.2050i 0.0075+0.1896i; 0.1730+0.0383i 0.6533 -0.1297-0.6396i -0.2886-0.1806i; -0.1293-0.0810i 0.7675 -0.3124-0.0060i 0.4108-0.3483i; 0.1200-0.3160i 0.7791 0.5003+0.1412i 0.0638+0.0668i");
				  break;
			  // 4 transmit antennas - 1 spatial stream - 6 bit codebook
			  //case 416:
				  //codebook.set("0.5000 0.5000 0.5000 0.5000; -0.4619+0.1913i  0.0000+0.5000i -0.5000+0.0000i 0.4976+0.0490i; 0.3536-0.3536i -0.5000+0.0000i  0.5000-0.0000i 0.4904+0.0975i; -0.1913+0.4619i -0.0000-0.5000i -0.5000+0.0000i 0.4785+0.1451i; -0.0000-0.5000i 0.5000-0.0000i  0.5000-0.0000i 0.4619+0.1913i; 0.1913+0.4619i 0.0000+0.5000i -0.5000+0.0000i 0.4410+0.2357i; -0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.4157+0.2778i; 0.4619+0.1913i -0.0000-0.5000i -0.5000+0.0000i 0.3865+0.3172i; -0.5000+0.0000i 0.5000-0.0000i  0.5000-0.0000i 0.3536+0.3536i; 0.4619-0.1913i 0.0000+0.5000i -0.5000+0.0000i 0.3172+0.3865i; -0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.2778+0.4157i; 0.1913-0.4619i -0.0000-0.5000i -0.5000+0.0000i 0.2357+0.4410i;-0.0000+0.5000i 0.5000-0.0000i 0.5000-0.0000i 0.1913+0.4619i; -0.1913-0.4619i -0.0000+0.5000i -0.5000-0.0000i 0.1451+0.4785i; 0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.0975+0.4904i; -0.4619-0.1913i -0.0000-0.5000i -0.5000+0.0000i 0.0490+0.4976i; 0.5000-0.0000i 0.5000-0.0000i 0.5000-0.0000i 0.0000+0.5000i; -0.4619+0.1913i -0.0000+0.5000i -0.5000-0.0000i -0.0490+0.4976i; 0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.0975+0.4904i; -0.1913+0.4619i -0.0000-0.5000i -0.5000+0.0000i -0.1451+0.4785i; 0.0000-0.5000i 0.5000-0.0000i 0.5000-0.0000i -0.1913+0.4619i; 0.1913+0.4619i -0.0000+0.5000i -0.5000-0.0000i -0.2357+0.4410i; -0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.2778+0.4157i; 0.4619+0.1913i -0.0000-0.5000i -0.5000+0.0000i -0.3172+0.3865i; -0.5000-0.0000i 0.5000-0.0000i 0.5000-0.0000i -0.3536+0.3536i; 0.4619-0.1913i -0.0000+0.5000i -0.5000-0.0000i -0.3865+0.3172i; -0.3536+0.3536i -0.5000-0.0000i 0.5000+0.0000i -0.4157+0.2778i; 0.1913-0.4619i -0.0000-0.5000i -0.5000+0.0000i -0.4410+0.2357i; 0.0000+0.5000i 0.5000-0.0000i 0.5000-0.0000i -0.4619+0.1913i; -0.1913-0.4619i -0.0000+0.5000i -0.5000-0.0000i -0.4785+0.1451i; 0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.4904+0.0975i; -0.4619-0.1913i -0.0000-0.5000i -0.5000+0.0000i -0.4976+0.0490i; 0.5000-0.0000i 0.5000-0.0000i 0.5000-0.0000i -0.5000+0.0000i; -0.4619+0.1913i 0.0000+0.5000i -0.5000+0.0000i -0.4976-0.0490i; 0.3536-0.3536i -0.5000-0.0000i 0.5000+0.0000i -0.4904-0.0975i; -0.1913+0.4619i -0.0000-0.5000i -0.5000+0.0000i -0.4785-0.1451i; 0.0000-0.5000i 0.5000-0.0000i 0.5000-0.0000i -0.4619-0.1913i; 0.1913+0.4619i 0.0000+0.5000i -0.5000+0.0000i -0.4410-0.2357i;-0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.4157-0.2778i; 0.4619+0.1913i -0.0000-0.5000i -0.5000+0.0000i -0.3865-0.3172i; -0.5000+0.0000i 0.5000-0.0000i 0.5000-0.0000i -0.3536-0.3536i; 0.4619-0.1913i 0.0000+0.5000i -0.5000+0.0000i -0.3172-0.3865i; -0.3536+0.3536i -0.5000-0.0000i 0.5000+0.0000i -0.2778-0.4157i; 0.1913-0.4619i -0.0000-0.5000i -0.5000+0.0000i -0.2357-0.4410i; 0.0000+0.5000i 0.5000-0.0000i 0.5000-0.0000i -0.1913-0.4619i; 0.1913-0.4619i 0.0000+0.5000i -0.5000+0.0000i -0.1451-0.4785i; 0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i -0.0975-0.4904i; -0.4619-0.1913i 0.0000-0.5000i -0.5000-0.0000i -0.0490-0.4976i; 0.5000+0.0000i 0.5000-0.0000i 0.5000-0.0000i -0.0000-0.5000i; -0.4619+0.1913i 0.0000+0.5000i -0.5000+0.0000i 0.0490-0.4976i; 0.3536-0.3536i -0.5000-0.0000i 0.5000+0.0000i 0.0975-0.4904i; -0.1913+0.4619i -0.0000-0.5000i -0.5000+0.0000i 0.1451-0.4785i; -0.0000-0.5000i 0.5000+0.0000i 0.5000+0.0000i 0.1913-0.4619i; 0.1913+0.4619i 0.0000+0.5000i -0.5000+0.0000i 0.2357-0.4410i; -0.3536-0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.2778-0.4157i; 0.4619+0.1913i 0.0000-0.5000i -0.5000-0.0000i 0.3172-0.3865i; -0.5000+0.0000i 0.5000-0.0000i 0.5000-0.0000i 0.3536-0.3536i; 0.4619-0.1913i 0.0000+0.5000i -0.5000+0.0000i 0.3865-0.3172i;-0.3536+0.3536i -0.5000-0.0000i 0.5000+0.0000i 0.4157-0.2778i; 0.1913-0.4619i -0.0000-0.5000i -0.5000+0.0000i 0.4410-0.2357i; -0.0000+0.5000i 0.5000-0.0000i 0.5000-0.0000i 0.4619-0.1913i; -0.1913-0.4619i 0.0000+0.5000i -0.5000+0.0000i 0.4785-0.1451i; 0.3536+0.3536i -0.5000+0.0000i 0.5000-0.0000i 0.4904-0.0975i; -0.4619-0.1913i 0.0000-0.5000i -0.5000-0.0000i 0.4976-0.0490i");
          //break;
        // 4 transmit antennas - 2 spatial streams - 2 bit codebook  
        case 422:
          codebook.set("0.4949 -0.4260; -0.1396+0.2203i 0.6019+0.3983i; -0.3120+0.5539i -0.4176-0.1331i;-0.0072-0.5318i -0.1630-0.2809i; 0.6879 0.2754; 0.2577+0.2058i 0.6060-0.1378i; 0.5832-0.2416i -0.3313+0.5103i; -0.1393-0.0126i -0.0315+0.4085i; -0.7101 -0.4345; 0.1241-0.4773i -0.1706-0.1901i; 0.0052+0.2047i 0.0428+0.0043i;0.2778-0.3653i -0.3029+0.8077i; 0.4605 -0.0655; -0.4159-0.4254i -0.0958+0.4589i;-0.2868-0.4510i 0.5593-0.3799i; -0.3851+0.0025i -0.4512+0.3395i");
          break;
        // 4 transmit antennas - 2 spatial streams - 3 bit codebook
        case 423:
          codebook.set("1 0; 0 1; 0 0; 0 0; 1 0; 0 0; 0 1; 0 0; 1 0; 0 0; 0 0; 0 1; 0 0; 1 0; 0 1; 0 0; 0 0; 1 0; 0 0; 0 1; 0 0; 0 0; 1 0; 0 1; 0.5538 -0.4154; -0.2073-0.6654i 0.0351+0.0075i; -0.1567-0.2529i 0.2281-0.6236i; 0.1476-0.3120i -0.3151-0.5348i; 0.4494 0.5407; 0.6853+0.0676i 0.1920+0.1144i; -0.2991-0.3953i 0.3640+0.3312i; 0.0317-0.2777i -0.4500+0.4616i");
          break;
        // 4 transmit antennas - 2 spatial streams - 4 bit codebook
        case 424:
          codebook.set("-0.9376 0.1742;-0.2106-0.0822i -0.5174+0.0220i;-0.0111-0.2112i 0.0581-0.6442i;-0.0469+0.1508i -0.0186-0.5317i; 0.2793 -0.3160; -0.4549-0.3187i -0.5960+0.2898i;-0.2066+0.1564i 0.2284+0.2808i; 0.4571+0.5809i -0.5147+0.2549i; 0.6665 0.1409;0.2386+0.2056i -0.3661+0.0843i; -0.4740+0.3594i 0.1921+0.4991i;0.2610-0.1860i 0.0956+0.7374i; 0.3749 0.5542; 0.0592-0.4261i 0.0204+0.3257i;-0.1408-0.5923i 0.2967+0.4753i; 0.1484-0.5307i -0.1153-0.5091i; -0.0937 0.8273;0.2588+0.2191i -0.1878+0.4828i; 0.4871+0.0701i 0.0058+0.0128i;-0.0082+0.7963i 0.2157+0.0231i; -0.6191 0.6507; -0.0195+0.4202i 0.4164+0.3445i;-0.2878-0.1665i -0.1708-0.3982i; 0.4765+0.3194i 0.1256+0.2847i; -0.6140 -0.4076;0.2396-0.1489i -0.0448+0.2205i; -0.2639-0.0265i 0.8670-0.1049i;0.6025+0.3318i 0.0925-0.1097i; 0.1682 -0.8382; -0.0317-0.0724i 0.3439+0.3663i;-0.5681+0.5930i -0.0392+0.1338i; -0.5331+0.0832i -0.1518-0.0504i; -0.6090 0.2396;-0.0121-0.2358i 0.2285+0.3424i; 0.0215+0.6921i -0.2282+0.4261i;0.0015+0.3065i 0.7064-0.2011i; 0.4123 0.4869; 0.3118-0.7414i 0.0920+0.4852i;-0.3064+0.0145i -0.4390+0.5277i; 0.2460+0.1687i 0.0887-0.1999i; -0.7582 0.2986;0.1112-0.5079i -0.3613-0.3139i; 0.0509-0.0521i 0.6040-0.1737i; 0.1201+0.3675i -0.4273+0.3228i;0.1041 -0.4215; 0.1417+0.8540i 0.1145-0.0838i; -0.0879-0.1321i -0.7662-0.2031i;-0.3343-0.3206i 0.2808-0.3083i; 0.4203 -0.2287; -0.4349-0.3735i 0.3887-0.5172i;-0.1869+0.2088i -0.1969+0.4818i; 0.0376+0.6440i 0.4911-0.1302i; -0.4668 0.5186;-0.3856-0.2549i -0.4030-0.1294i; -0.1575-0.4429i -0.4858+0.2062i;0.5858-0.0658i 0.1724+0.4936i; -0.2243 -0.8379; -0.7912+0.2776i -0.0030-0.0445i;-0.2226-0.1619i -0.1878+0.2328i; 0.2817+0.3025i -0.3819-0.2462i; -0.6327 0.2901;-0.4231+0.0686i -0.6546-0.1568i; -0.2278-0.3770i -0.1221+0.6026i;-0.4708-0.0184i -0.2414-0.1626i;");
          break;
        default:
          {
          cout << "ERROR: Codebook dimensions are not currently supported" << endl;
          is_matrix_valid = false;
          }
          break;
      }
      break;
    default:
      {
      cout << "ERROR: Codebook type is not currently supported!" << endl;
      is_matrix_valid = false;
      }
      break;
  } // end of switch codebook_index
}


double
Precoding_MIMO_OFDM::design_criterion(unsigned int criterion_type, itpp::cmat H, itpp::cmat F, double SNR, unsigned int pc_mode)
{
  unsigned int num_rx = H.rows();
  unsigned int num_tx = H.cols();
  unsigned int max_mode = std::min(num_rx,num_tx);

  double cost_function_value = 0;
  
  // check the validity of pc_mode value
  if(pc_mode <= max_mode)
  {
    cmat regularized_combined_chan;
    regularized_combined_chan.set_size(pc_mode,false);
    cmat H_F = H*F;
    cmat H_F_correlation_matrix = hermitian_transpose(H_F)*H_F;
    regularized_combined_chan = eye(pc_mode) + (SNR/pc_mode)*H_F_correlation_matrix;
    cmat M_F = inv(regularized_combined_chan); // mean squared error matrix

    switch(criterion_type)
    {
      case 0: // maximizes the capacity
        {
          //double I_F = itpp::log2(abs(det(regularized_combined_chan)));
          double I_F = itpp::logb(2,abs(det(regularized_combined_chan)));
          cost_function_value = I_F;
        }
        break;
      case 1: // minimizes the determinant of mean squared error
        {
          double det_M_F = abs(det(M_F));
          cost_function_value = det_M_F;
        }
        break;
      case 2: // minimizes the trace of mean squared error
        {
          double trace_M_F = abs(trace(M_F));
          cost_function_value = trace_M_F;
        }
        break;
      case 3: // maximizes the minimum singular value
        {
          vec singular_value_H_F(max_mode);
          singular_value_H_F = svd(H_F);
          double min_singular_value_H_F = itpp::min(singular_value_H_F);
          cost_function_value = min_singular_value_H_F;
        }
        break;
      default:
        {
          cout << "ERROR: the criterion type is not currently supported!" << endl;
          is_matrix_valid = false;
        }
        break;
    } // end of criterion_type switch
  }
  else
  {
    cout << "ERROR: the pc_mode value exceeds the maximum potential mode!" << endl;
    is_matrix_valid = false;
  }
  return cost_function_value;
}


double
Precoding_MIMO_OFDM::clustered_design_criterion(unsigned int cluster_num, unsigned int cluster_size, unsigned int cluster_metric, itpp::Array<itpp::cmat>const& in_chan, itpp::cmat F, double SNR, unsigned int pc_mode)
{
  unsigned int num_rx = in_chan(0).rows(); // number of receive antennas
  unsigned int num_tx = in_chan(0).cols(); // number of transmit antennas
  unsigned int max_mode = std::min(num_rx,num_tx); // maximum potential mode
  unsigned int num_sc = in_chan.size(); // number of subcarriers

  double cost_function_value = 0;
  
  // check the validity of pc_mode value
  if(pc_mode <= max_mode)
  {    
    // the index of the first subcarrier in the cluster
    unsigned int start_subcarrier_index = cluster_num*cluster_size;
    // the index of the last subcarrier in the cluster
    unsigned int end_subcarrier_index = std::min(num_sc-1,(cluster_num+1)*cluster_size);

    switch(cluster_metric)
    {
      case 0: // maximizes the total capacity
        {
          for(unsigned int a=start_subcarrier_index; a<=end_subcarrier_index; a++)
          {
            cmat regularized_combined_chan;
            regularized_combined_chan.set_size(pc_mode,false);
            cmat H_F = in_chan(a)*F;
            cmat H_F_correlation_matrix = hermitian_transpose(H_F)*H_F;
            regularized_combined_chan = eye(pc_mode) + (SNR/pc_mode)*H_F_correlation_matrix;

            //double I_F = itpp::log2(abs(det(regularized_combined_chan)));
            double I_F = itpp::logb(2,abs(det(regularized_combined_chan)));
            cost_function_value += I_F;
          }
        }
        break;
      case 1: 
        break;
      default:
        {
          cout << "ERROR: the cluster selection metric is not currently supported!" << endl;
          is_matrix_valid = false;
        }
        break;
    } // end of cluster_metric switch
  }
  else
  {
    cout << "ERROR: the pc_mode value exceeds the maximum potential mode!" << endl;
    is_matrix_valid = false;
  }
  return cost_function_value;
}


ivec
Precoding_MIMO_OFDM::compute_mode_set(unsigned int bit_rate, unsigned int num_tx)
{
  // loop determines the size of the set of potential modes
  unsigned int mode_set_size=0;
  for(unsigned int a=1;a<=num_tx;a++)
  {
    if(mod(bit_rate,a)==0)
    {
      mode_set_size += 1;
    }
  }
  // loop calculates the set of potential modes
  ivec modes = ones_i(mode_set_size);
  unsigned int mode_index = 0;
  for(unsigned int b=1;b<=num_tx;b++)
  {
    if(mod(bit_rate,b)==0)
    {
      mode_index += 1;
      modes(mode_index) = b;
    }
  }
  return modes;
}


void
Precoding_MIMO_OFDM::matrix_SVD(itpp::Array<itpp::cmat> const& H)
{
  // Reserve memory for eigenvalues, eigenvectors
  eigenvalues.set_size(Nsc);
  tx_eigenvectors.set_size(Nsc);
  rx_eigenvectors.set_size(Nsc);

  // Initialize eigenvalues and eigenvectors
  for(unsigned int a=0; a<Nsc; a++)
  {
    eigenvalues(a) = ones(Nmin);
    tx_eigenvectors(a) = ones_c(Ntx,Nmin);
    rx_eigenvectors(a) = ones_c(Nrx,Nmin);    
  }

  bool svd_test;

  is_matrix_valid = true;

  // compute singular value decomposition for each subcarrier
  for(unsigned int b=0; b<Nsc; b++)
  {
    svd_test = svd(H(b),rx_eigenvectors(b),eigenvalues(b),tx_eigenvectors(b));
    if(!svd_test)
      is_matrix_valid=false;
  }
}
