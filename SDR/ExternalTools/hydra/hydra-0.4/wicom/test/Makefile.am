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
qaincludedir = $(wicomincludedir)/qa

INCLUDES = $(STD_DEFINES_AND_INCLUDES) \
					 $(ANNCFLAGS) \
					 -I$(top_srcdir)/src/precoding \
					 -I$(top_srcdir)/src/self-adaptation \
					 -I$(top_srcdir)/test \
					 $(META_80211N_CFLAGS)

# list of programs run by "make check" and "make distcheck"
TESTS = test-all

# Other files for distribution
EXTRA_DIST = \
	test_11n.h \
	test_11n.cc \
	test_11n_without_synch.cc \
	test_11n_sa_testing.cc

# Conditionally compile test-11n programs.
# Add any additional tests with meta-80211n dependency here.
if HAVE_META_80211N
TEST11N_NO_SYNCH = test-11n-sa-testing-no-synch
TEST11N = test-11n-sa-testing
else
TEST11N =
endif

noinst_PROGRAMS = \
	test-all \
	$(TEST11N) \
	$(TEST11N_NO_SYNCH)

# source files needed to run QA Unit Tests
BUILD_SRCS = \
	qa_wicom.cc	\
	qa_sa.cc

# install QA headers to install tree
qainclude_HEADERS = \
	qa_wicom.h	\
	qa_sa.h

# compile test with libwicom
test_all_SOURCES = $(BUILD_SRCS) test-all.cc
test_all_LDADD   = \
	-L$(top_builddir)/src -lwicom \
	$(CPPUNIT_LIBS)

CLEANFILES = *.exe test-11n-sa-testing test-11n-sa-testing-no-synch

# Build standalone executables
#bin_PROGRAMS = hello
#hello_SOURCES = hello.cc
#hello_LDADD = \
#	-L$(top_builddir)/src -lwicom

test_11n_sa_testing_no_synch_SOURCES = test_11n_without_synch.cc test_11n_sa_testing.cc
test_11n_sa_testing_no_synch_LDFLAGS =
test_11n_sa_testing_no_synch_LDADD =	\
	-L$(top_builddir)/src -lwicom	\
	$(META_80211N_LIBS)
test_11n_sa_testing_SOURCES = test_11n.cc test_11n_sa_testing.cc
test_11n_sa_testing_LDFLAGS =
test_11n_sa_testing_LDADD =	\
	-L$(top_builddir)/src -lwicom	\
	$(META_80211N_LIBS)
