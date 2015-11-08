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

meta80211nsrcdir=$(top_srcdir)/src
META80211N_INCLUDES = -I$(meta80211nsrcdir)/rx

INCLUDES = $(META80211N_INCLUDES) $(STD_DEFINES_AND_INCLUDES) $(ITPPCFLAGS)

noinst_LTLIBRARIES = libmeta-80211n-tx.la
libmeta_80211n_tx_la_SOURCES =	\
	channel_encoding_11n.cc \
	constellation_mapping_11n.cc \
	interleaving_11n.cc \
	ltf_formatting_11n.cc \
	payload_formatting_11n.cc \
	pilot_formatting_11n.cc \
	preamble_formatting_11n.cc \
	precoding_matrices_11n.cc \
	scrambling_11n.cc \
	signal_field_formatting_11n.cc \
	space_time_coding_11n.cc \
	spatial_mapping_11n.cc \
	spatial_parsing_11n.cc \
	stf_formatting_11n.cc \
	transmission_formatting_11n.cc \
	transmission_interface_11n.cc \
	txvector.cc
libmeta_80211n_tx_la_LDFLAGS = $(NO_UNDEFINED) -version-info 0:0:0 $(ITPPLDFLAGS)
libmeta_80211n_tx_la_LIBADD  =

metainclude_HEADERS =	\
	channel_encoding_11n.h \
	constellation_mapping_11n.h \
	interleaving_11n.h \
	ltf_formatting_11n.h \
	payload_formatting_11n.h \
	pilot_formatting_11n.h \
	preamble_formatting_11n.h \
	precoding_matrices_11n.h \
	scrambling_11n.h \
	signal_field_formatting_11n.h \
	space_time_coding_11n.h \
	spatial_mapping_11n.h \
	spatial_parsing_11n.h \
	stf_formatting_11n.h \
	transmission_formatting_11n.h \
	transmission_interface_11n.h \
	txvector.h
