##  Makefile.am
# 
# Copyright (C) 2009  The University of Texas at Austin.
# 
# This file is part of Hydra: A wireless multihop testbed.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

include $(top_srcdir)/Makefile.common

EXTRA_DIST =	\
	ann.pc.in	\
	README

INCLUDES = $(ANNCFLAGS)

# ANN not installed
lib_LTLIBRARIES = libann.la

libann_la_SOURCES = ann_fix.cc
libann_la_LDFLAGS = $(NO_UNDEFINED) -version-info 0:0:0
libann_la_LIBADD =	\
	-L$(ANNSRCDIR)/src/lib -lANN

# install header files
anninclude_HEADERS = \
	$(ANNSRCDIR)/src/include/ANN/ANN.h \
	$(ANNSRCDIR)/src/include/ANN/ANNperf.h \
	$(ANNSRCDIR)/src/include/ANN/ANNx.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ann.pc

ann_fix.cc: src/lib/libANN.a
	echo "#include <ANN/ANN.h>" > ann_fix.cc;

src/lib/libANN.a:
	pushd src; $(MAKE) linux-g++; popd;

clean-local:
	pushd src; $(MAKE) clean; popd;

maintainer-clean-local:
	pushd src; $(MAKE) realclean; popd

MOSTLYCLEANFILES = ann_fix.cc
