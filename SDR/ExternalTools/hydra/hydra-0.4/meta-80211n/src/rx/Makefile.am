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
META80211N_INCLUDES = -I$(meta80211nsrcdir)/tx

INCLUDES = $(META80211N_INCLUDES) $(STD_DEFINES_AND_INCLUDES) $(ITPPCFLAGS)

noinst_LTLIBRARIES = libmeta-80211n-rx.la
libmeta_80211n_rx_la_SOURCES = \
	cfo_estimation_11n.cc \
	channel_estimation_11n.cc \
	data_decoding_11n.cc \
	deinterleaving_11n.cc \
	descrambling_11n.cc \
	frame_alignment_11n.cc \
	frame_detection_11n.cc \
	header_decoding_11n.cc \
	soft_demapping_11n.cc \
	pilot_estimation_11n.cc \
	pilot_removal_11n.cc \
	rxvector.cc \
	signal_field_decoding_11n.cc \
	soft_bits_decoding_11n.cc \
	space_time_decoding_11n.cc \
	spatial_deparsing_11n.cc \
	spatial_equalization_11n.cc	\
	rfo_correction_11n.cc
libmeta_80211n_rx_la_LDFLAGS = $(NO_UNDEFINED) -version-info 0:0:0 $(ITPPLDFLAGS)
libmeta_80211n_rx_la_LIBADD  =

metainclude_HEADERS = \
	cfo_estimation_11n.h \
	channel_estimation_11n.h \
	data_decoding_11n.h \
	deinterleaving_11n.h \
	descrambling_11n.h \
	frame_alignment_11n.h \
	frame_detection_11n.h \
	header_decoding_11n.h \
	soft_demapping_11n.h \
	pilot_estimation_11n.h \
	pilot_removal_11n.h \
	rxvector.h \
	signal_field_decoding_11n.h \
	soft_bits_decoding_11n.h \
	space_time_decoding_11n.h \
	spatial_deparsing_11n.h \
	spatial_equalization_11n.h	\
	rfo_correction_11n.h
