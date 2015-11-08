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

wicomsrcdir=$(top_srcdir)/src
#WICOM_INCLUDES = -I$(wicomsrcdir)/rx

INCLUDES = $(WICOM_INCLUDES) $(STD_DEFINES_AND_INCLUDES) $(ITPPCFLAGS) $(ANNCFLAGS)

noinst_LTLIBRARIES = libwicom-self-adaptation.la
libwicom_self_adaptation_la_SOURCES = \
	self_adaptation_mimo_ofdm.cc
libwicom_self_adaptation_la_LDFLAGS = $(NO_UNDEFINED) -version-info 0:0:0 $(ITPPLDFLAGS) $(ANNLDFLAGS)
libwicom_self_adaptation_la_LIBADD  =

wicominclude_HEADERS =	\
	self_adaptation_mimo_ofdm.h
