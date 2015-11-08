#
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
# 

include $(top_srcdir)/Makefile.common

mpifsrcdir=$(top_srcdir)/src/lib/mpif
physrcdir=$(top_srcdir)/src/lib/phy
coresrcdir=$(top_srcdir)/src/lib/core
MPIF_INCLUDES = -I$(mpifsrcdir)/common	\
								-I$(mpifsrcdir)/tx	\
								-I$(mpifsrcdir)/rx	\
								-I$(physrcdir)/tx	\
								-I$(coresrcdir)/common

# Install this stuff so that it ends up as the hydra.mpif module
# This usually ends up at:
#   ${prefix}/lib/python${python_version}/site-packages/hydra

ourpythondir = $(hydrapythondir)
ourlibdir    = $(hydrapyexecdir)

INCLUDES = $(MPIF_INCLUDES) $(STD_DEFINES_AND_INCLUDES) $(PYTHON_CPPFLAGS)

SWIGPYTHONARGS = $(MPIF_INCLUDES) $(SWIGPYTHONFLAGS) $(SWIGGRFLAGS)

NON_LOCAL_IFILES = $(GNURADIO_CORE_INCLUDEDIR)/swig/gnuradio.i

LOCAL_IFILES = mpif.i

ALL_IFILES = 				\
	$(LOCAL_IFILES)			\
	$(NON_LOCAL_IFILES) \
	$(mpifsrcdir)/common/mpifcommon.i	\
	$(mpifsrcdir)/tx/mpiftx.i	\
	$(mpifsrcdir)/rx/mpifrx.i

OTHER_DEPS = 		\
	$(top_srcdir)/src/lib/libhydra.la

# These files are built by SWIG.  The first is the C++ glue.
# The second is the python wrapper that loads the _mpif shared library
# and knows how to call our extensions.

BUILT_SOURCES = mpif.cc mpif.py				

# This gets mpif.py installed in the right place
ourpython_PYTHON = mpif.py

# generate _mpif.la from libhydra.la
ourlib_LTLIBRARIES = _mpif.la
_mpif_la_SOURCES = mpif.cc
_mpif_la_LDFLAGS = $(NO_UNDEFINED) -module -avoid-version
_mpif_la_LIBADD =	\
	-L$(top_builddir)/src/lib -lhydra	\
	$(PYTHON_LDFLAGS)	\
	-lstdc++

# Install header files
grinclude_HEADERS = 
hydraswiginclude_HEADERS = 		\
	$(LOCAL_IFILES)
hydrainclude_HEADERS =

mpif.cc mpif.py: $(LOCAL_IFILES) $(ALL_IFILES) $(OTHER_DEPS)
	$(SWIG) $(SWIGPYTHONARGS) -module mpif -o mpif.cc $(LOCAL_IFILES)

MOSTLYCLEANFILES = $(BUILT_SOURCES) *.pyc *.pyo

#DISTCLEANFILES = mpif.d

# Don't distribute output of swig
dist-hook:
	@for file in $(BUILT_SOURCES); do echo $(RM) $(distdir)/$$file; done
	@for file in $(BUILT_SOURCES); do $(RM) $(distdir)/$$file; done
