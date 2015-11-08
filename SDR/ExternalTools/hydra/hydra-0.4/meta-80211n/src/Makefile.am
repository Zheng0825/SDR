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

EXTRA_DIST = meta-80211n.pc.in

SUBDIRS = common rx tx
DIST_SUBDIRS = common rx tx

lib_LTLIBRARIES = libmeta-80211n.la

libmeta_80211n_la_SOURCES =
libmeta_80211n_la_LDFLAGS =
libmeta_80211n_la_LIBADD =	\
	common/libmeta-80211n-common.la		\
	rx/libmeta-80211n-rx.la		\
	tx/libmeta-80211n-tx.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = meta-80211n.pc
