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

# Install m4 macros in this directory
m4datadir = $(datadir)/aclocal

# List your m4 macros here
m4macros = 			\
	acx_pthread.m4		\
	bnv_have_qt.m4		\
	cppunit.m4		\
	lf_cc.m4		\
	lf_cxx.m4		\
	lf_warnings.m4		\
	lf_x11.m4		\
	mkstemp.m4		\
	onceonly.m4		\
	pkg.m4                  \
	grc_ann.m4	\
	grc_precoding.m4	\
	grc_self_adaptation.m4	\
	grc_wicom.m4	\
	grc_meta_80211n.m4

# Don't install m4 macros anymore
# m4data_DATA = $(m4macros) 

EXTRA_DIST = $(m4macros) 
