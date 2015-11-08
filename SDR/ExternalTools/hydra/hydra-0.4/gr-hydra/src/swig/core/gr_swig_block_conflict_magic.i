/* -*- c++ -*- */
/*
 * Copyright (C) 2009  The University of Texas at Austin.
 * Copyright 2004 Free Software Foundation, Inc.
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

/*
 *****************************************************************************
 * This method should only be used when the name of a gr_block conflicts with
 * that of a similarly named class in a different module. For example: if
 * phy_square_ff has been previously defined, then use this method to create a
 * SWIG wrapper for mimo_square_ff (i.e. an object with the same name, but in
 * the mimo module)
 *****************************************************************************

 * First arg is the package prefix.
 * Second arg is the name of the class minus the prefix.
 *
 * This does some behind-the-scenes magic so we can
 * access phy_square_ff from python as phy.square_ff

 * Use this method to create a SWIG wrapper for any c++ class.
 * 
 * usage: GR_SWIG_BLOCK_CONFLICT_MAGIC(pkg, name)
 *      pkg  = package prefix
 *      name = name of class minus the prefix
 * Example:
 *      GR_SWIG_BLOCK_CONFLICT_MAGIC (Foo, Bar);
 * This will add the c++ class Foo_Bar to the python package Foo.
 * This allows the c++ class Foo_Bar to be accessed in python as Foo.Bar.
 *
 * In order to use GR_SWIG_BLOCK_CONFLICT_MAGIC you must use the naming
 * convention used in the example, i.e. to add create a python
 * class NAME in package PKG using GR_SWIG_BLOCK_CONFLICT_MAGIC your 
 * c++ class should be named PKG_NAME.
 */
%define GR_SWIG_BLOCK_CONFLICT_MAGIC(PKG, BASE_NAME)
_GR_SWIG_BLOCK_CONFLICT_MAGIC_HELPER(PKG, PKG ## _ ## BASE_NAME, BASE_NAME)
%enddef

%define _GR_SWIG_BLOCK_CONFLICT_MAGIC_HELPER(PKG, NAME, BASE_NAME)
%module PKG;
class NAME;
typedef boost::shared_ptr<NAME> NAME ## _sptr;
%template(NAME ## _sptr) boost::shared_ptr<NAME>;
%rename(tt_ ## NAME) PKG ## _make_ ## BASE_NAME;
%inline {
  gr_block_sptr NAME ## _block (NAME ## _sptr r)
  {
    return gr_block_sptr (r);
  }
}

%pythoncode %{
def BASE_NAME (*args):
  return tt_ ## NAME (*args)
NAME ## _sptr.block = lambda self: NAME ## _block (self)
NAME ## _sptr.__repr__ = lambda self: "<gr_block %s (%d)>" % (self.name(), self.unique_id ())
%}

%ignore NAME;
%enddef
