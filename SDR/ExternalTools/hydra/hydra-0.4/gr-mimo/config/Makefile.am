#
# Copyright (C) 2009  The University of Texas at Austin.
# Copyright 2001 Free Software Foundation, Inc.
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
# 

include $(top_srcdir)/Makefile.common

# Install m4 macros in this directory
m4datadir = $(datadir)/aclocal

# List your m4 macros here
m4macros = 			\
	acx_pthread.m4		\
	bnv_have_qt.m4		\
	cppunit.m4		\
	gr_boost.m4		\
	grc_mimo.m4	\
	gr_check_createfilemapping.m4 \
	gr_check_usrp.m4	\
	gr_check_mc4020.m4	\
	gr_check_shm_open.m4	\
	gr_doxygen.m4		\
	gr_gprof.m4		\
	gr_libgnuradio_core_extra_ldflags.m4 \
	gr_no_undefined.m4	\
	gr_omnithread.m4	\
	gr_pwin32.m4		\
	gr_python.m4		\
	gr_require_mc4020.m4	\
	gr_scripting.m4		\
	gr_set_md_cpu.m4	\
	gr_swig.m4		\
	gr_sysv_shm.m4		\
	gr_x86_64.m4		\
	lf_cc.m4		\
	lf_cxx.m4		\
	lf_warnings.m4		\
	lf_x11.m4		\
	mkstemp.m4		\
	onceonly.m4		\
	pkg.m4			\
	usrp_fusb_tech.m4	\
	usrp_libusb.m4		\
	usrp_sdcc.m4		


# Don't install m4 macros anymore
# m4data_DATA = $(m4macros) 

EXTRA_DIST = $(m4macros) 
