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

# Create folder for QA header files in install tree
qaincludedir = $(metaincludedir)/qa

INCLUDES = $(STD_DEFINES_AND_INCLUDES) \
					 -I$(top_srcdir)/src/common \
					 -I$(top_srcdir)/src/rx \
					 -I$(top_srcdir)/src/tx \
					 -I$(top_srcdir)/test

# list of programs run by "make check" and "make distcheck"
TESTS = test-all

# Other files for distribution
EXTRA_DIST =

noinst_PROGRAMS = \
	test-all \
	test_simulation

# source files needed to run QA Unit Tests
BUILD_SRCS =	\
	test_11n.cc \
	qa_foo.cc \
	qa_sample_simulation.cc \
	qa_meta_80211n.cc

# install QA headers to install tree
qainclude_HEADERS =	\
	test_11n.h \
	qa_foo.h \
	qa_sample_simulation.h \
	qa_meta_80211n.h 

# compile test with libmeta-80211n
test_all_SOURCES = $(BUILD_SRCS) test-all.cc
test_all_LDADD   = \
	-L$(top_builddir)/src -lmeta-80211n \
	$(CPPUNIT_LIBS)

CLEANFILES = *.exe

# Build standalone executables
#bin_PROGRAMS = hello
#hello_SOURCES = hello.cc
#hello_LDADD = \
#	-L$(top_builddir)/src -lmeta-80211n

test_simulation_SOURCES = test_11n.cc test_simulation.cc
test_simulation_LDADD = \
	-L$(top_builddir)/src -lmeta-80211n
