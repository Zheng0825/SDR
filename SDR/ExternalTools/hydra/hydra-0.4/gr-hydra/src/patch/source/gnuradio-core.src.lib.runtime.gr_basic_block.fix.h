/* -*- c++ -*- */
/*
 * Copyright (C) 2009  The University of Texas at Austin.
 * Copyright 2006,2008 Free Software Foundation, Inc.
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

#ifndef INCLUDED_GR_BASIC_BLOCK_H
#define INCLUDED_GR_BASIC_BLOCK_H

/* Hydra Patch:
 *    Define preprocessor Macro that can be used to detect if file has been
 *    patched.
 */
#ifndef USE_HYDRA_MAX_BUFFER_SIZE
#define USE_HYDRA_MAX_BUFFER_SIZE
#endif

#include <gr_runtime_types.h>
#include <gr_sptr_magic.h>
#include <boost/enable_shared_from_this.hpp>
#include <string>

/*!
 * \brief The abstract base class for all signal processing blocks.
 * \ingroup internal
 *
 * Basic blocks are the bare abstraction of an entity that has a name
 * and a set of inputs and outputs.  These are never instantiated
 * directly; rather, this is the abstract parent class of both gr_hier_block,
 * which is a recursive container, and gr_block, which implements actual
 * signal processing functions.
 */

class gr_basic_block : public boost::enable_shared_from_this<gr_basic_block>
{
protected:
    friend class gr_flowgraph;
    friend class gr_flat_flowgraph; // TODO: will be redundant

    enum vcolor { WHITE, GREY, BLACK };

    std::string          d_name;
    gr_io_signature_sptr d_input_signature;
    gr_io_signature_sptr d_output_signature;
    long                 d_unique_id;
    vcolor               d_color;

    //! Protected constructor prevents instantiation by non-derived classes
    gr_basic_block(const std::string &name,
                   gr_io_signature_sptr input_signature,
                   gr_io_signature_sptr output_signature);

    //! may only be called during constructor
    void set_input_signature(gr_io_signature_sptr iosig) {
        d_input_signature = iosig;
    }
    
    //! may only be called during constructor
    void set_output_signature(gr_io_signature_sptr iosig) {
        d_output_signature = iosig;
    }

    /*!
     * \brief Allow the flowgraph to set for sorting and partitioning
     */
    void set_color(vcolor color) { d_color = color; }
    vcolor color() const { return d_color; }

public:
    virtual ~gr_basic_block();
    long unique_id() const { return d_unique_id; }
    std::string name() const { return d_name; }
    gr_io_signature_sptr input_signature() const  { return d_input_signature; }
    gr_io_signature_sptr output_signature() const { return d_output_signature; }
    gr_basic_block_sptr basic_block(); // Needed for Python type coercion

    /*!
     * \brief Confirm that ninputs and noutputs is an acceptable combination.
     *
     * \param ninputs	number of input streams connected
     * \param noutputs	number of output streams connected
     *
     * \returns true if this is a valid configuration for this block.
     *
     * This function is called by the runtime system whenever the
     * topology changes.  Most classes do not need to override this.
     * This check is in addition to the constraints specified by the input
     * and output gr_io_signatures.
     */
    virtual bool check_topology(int ninputs, int noutputs) { return true; }

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
    virtual inline int max_buffer_size() const { return -1; }
#endif
};

inline bool operator<(gr_basic_block_sptr lhs, gr_basic_block_sptr rhs)
{
  return lhs->unique_id() < rhs->unique_id();
}

typedef std::vector<gr_basic_block_sptr> gr_basic_block_vector_t;
typedef std::vector<gr_basic_block_sptr>::iterator gr_basic_block_viter_t;

long gr_basic_block_ncurrently_allocated();

inline std::ostream &operator << (std::ostream &os, gr_basic_block_sptr basic_block)
{
    os << basic_block->name() << "(" << basic_block->unique_id() << ")";
    return os;
}

#endif /* INCLUDED_GR_BASIC_BLOCK_H */
