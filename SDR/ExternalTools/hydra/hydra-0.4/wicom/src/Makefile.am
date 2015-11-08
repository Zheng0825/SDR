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

EXTRA_DIST = wicom.pc.in

SUBDIRS = @build_dirs@
DIST_SUBDIRS = @build_dirs@ @skipped_dirs@

lib_LTLIBRARIES = libwicom.la

libwicom_la_SOURCES =
libwicom_la_LDFLAGS =
libwicom_la_LIBADD =	\
	precoding/libwicom-precoding.la \
	self-adaptation/libwicom-self-adaptation.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = wicom.pc
