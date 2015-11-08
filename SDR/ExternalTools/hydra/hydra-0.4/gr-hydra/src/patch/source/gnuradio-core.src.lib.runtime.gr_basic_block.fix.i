/* -*- c++ -*- */
/*
 * Copyright (C) 2009  The University of Texas at Austin.
 * Copyright 2006 Free Software Foundation, Inc.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

class gr_basic_block;
typedef boost::shared_ptr<gr_basic_block> gr_basic_block_sptr;
%template(gr_basic_block_sptr) boost::shared_ptr<gr_basic_block>;

// support vectors of these...
namespace std {
  %template(x_vector_gr_basic_block_sptr) vector<gr_basic_block_sptr>;
};

/* Hydra Patch:
 *    Define preprocessor Macro that can be used to detect if file has been
 *    patched.
 */
#ifndef USE_HYDRA_MAX_BUFFER_SIZE
#define USE_HYDRA_MAX_BUFFER_SIZE
#endif

class gr_basic_block
{
protected:
    gr_basic_block();

public:
    virtual ~gr_basic_block();
    std::string name() const;
    gr_io_signature_sptr input_signature() const;
    gr_io_signature_sptr output_signature() const;
    long unique_id() const;
    gr_basic_block_sptr basic_block();
    bool check_topology (int ninputs, int noutputs);
    bool check_topology (int ninputs, int noutputs);

#ifdef USE_HYDRA_MAX_BUFFER_SIZE
    /* Hydra Patch:
     *   Allows gr_blocks in Hydra to allocate bigger buffers.
     */

    /*!
     *  \brief This is a kludge.
     *
     *  In gr_flat_flowgraph, gnuradio uses a fixed buffer size of 32*1024
     *  bytes. In order to allow gr_blocks in Hydra to specify their own
     *  input/output buffer sizes, it was necessary to create this publicly
     *  accessible virtual method. This is called from
     *  gr_flat_flowgraph::allocate_buffer() to help determine the proper size
     *  of input/output buffers to gr_blocks.
     */
    virtual inline int max_buffer_size() const;
#endif
};

%rename(block_ncurrently_allocated) gr_basic_block_ncurrently_allocated;
long gr_basic_block_ncurrently_allocated();

%pythoncode %{
gr_basic_block_sptr.__repr__ = lambda self: "<gr_basic_block %s (%d)>" % (self.name(), self.unique_id ())
%}