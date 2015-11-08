##	Makefile.am
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

mimosrcdir=$(top_srcdir)/src/lib
MIMO_INCLUDES = -I$(mimosrcdir)/common	\
								-I$(mimosrcdir)/tx	\
								-I$(mimosrcdir)/rx

# Install this stuff so that it ends up as the hydra.mimo module
# This usually ends up at:
#   ${prefix}/lib/python${python_version}/site-packages/hydra

ourpythondir = $(hydrapythondir)
ourlibdir    = $(hydrapyexecdir)

INCLUDES = $(MIMO_INCLUDES) $(STD_DEFINES_AND_INCLUDES) $(PYTHON_CPPFLAGS)

SWIGPYTHONARGS = $(MIMO_INCLUDES) $(SWIGPYTHONFLAGS) $(SWIGGRFLAGS)

NON_LOCAL_IFILES = $(GNURADIO_CORE_INCLUDEDIR)/swig/gnuradio.i \
									 $(HYDRA_INCLUDEDIR)/swig/phy.i \
									 $(HYDRA_INCLUDEDIR)/swig/mpif.i

LOCAL_IFILES = mimo.i

ALL_IFILES = 				\
	$(LOCAL_IFILES)			\
	$(NON_LOCAL_IFILES) \
	$(mimosrcdir)/common/mimocommon.i	\
	$(mimosrcdir)/tx/mimotx.i	\
	$(mimosrcdir)/rx/mimorx.i


# These files are built by SWIG.  The first is the C++ glue.
# The second is the python wrapper that loads the _mimo shared library
# and knows how to call our extensions.

BUILT_SOURCES = mimo.cc mimo.py				

# This gets mimo.py installed in the right place
ourpython_PYTHON = mimo.py

# generate _mimo.la from the convenience libraries in $(top_buildir)/src/lib
ourlib_LTLIBRARIES = _mimo.la
_mimo_la_SOURCES = mimo.cc
_mimo_la_LDFLAGS = $(NO_UNDEFINED) -module -avoid-version
_mimo_la_LIBADD =	\
	-L$(top_builddir)/src/lib -lmimo	\
	$(PYTHON_LDFLAGS)	\
	-lstdc++

# Install header files
grinclude_HEADERS = 
mimoswiginclude_HEADERS = 		\
	$(LOCAL_IFILES)
mimoinclude_HEADERS =

mimo.cc mimo.py: $(LOCAL_IFILES) $(ALL_IFILES)
	$(SWIG) $(SWIGPYTHONARGS) -module mimo -o mimo.cc $(LOCAL_IFILES)

MOSTLYCLEANFILES = $(BUILT_SOURCES) *.pyc *.pyo

#DISTCLEANFILES = mimo.d

# Don't distribute output of swig
dist-hook:
	@for file in $(BUILT_SOURCES); do echo $(RM) $(distdir)/$$file; done
	@for file in $(BUILT_SOURCES); do $(RM) $(distdir)/$$file; done
