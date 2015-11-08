dnl 
dnl  Copyright (C) 2009  The University of Texas at Austin.
dnl  Copyright 2004,2005 Free Software Foundation, Inc.
dnl  
dnl  This file is part of Hydra: A wireless multihop testbed.
dnl  
dnl  This program is free software: you can redistribute it and/or modify
dnl  it under the terms of the GNU General Public License as published by
dnl  the Free Software Foundation, either version 3 of the License, or
dnl  (at your option) any later version.
dnl  
dnl  This program is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl  GNU General Public License for more details.
dnl  
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program.  If not, see <http://www.gnu.org/licenses/>.
dnl  
dnl 

AC_INIT
AC_PREREQ(2.57)
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER(config.h)
AC_CANONICAL_TARGET([])
AM_INIT_AUTOMAKE(gr-mimo,0.1svn)

GR_X86_64
dnl LF_CONFIGURE_CC
LF_CONFIGURE_CXX
LF_SET_WARNINGS
GR_SET_GPROF
GR_SET_PROF
AM_PROG_AS
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PATH_PROG([RM_PROG], [rm])

AC_LIBTOOL_WIN32_DLL
AC_ENABLE_SHARED	dnl do build shared libraries
AC_DISABLE_STATIC	dnl don't build static libraries
AC_PROG_LIBTOOL

dnl Locate python, SWIG, etc
GR_NO_UNDEFINED
GR_SCRIPTING

dnl Checks for libraries.

dnl check for threads (mandatory)
GR_OMNITHREAD

CFLAGS="${CFLAGS} $PTHREAD_CFLAGS"
CXXFLAGS="${CXXFLAGS} $PTHREAD_CFLAGS"

if test "x$CXX_FOR_BUILD" = x
then
  CXX_FOR_BUILD=${CXX}
fi
AC_SUBST(CXX_FOR_BUILD)

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h limits.h strings.h sys/ioctl.h sys/time.h unistd.h)
AC_CHECK_HEADERS(sys/mman.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

dnl Checks for library functions.
AC_CHECK_FUNCS([])

dnl Check for Mingw support
GR_PWIN32

PKG_CHECK_MODULES(GNURADIO_CORE, gnuradio-core >= 2)
LIBS="$LIBS $GNURADIO_CORE_LIBS"

dnl Check for IT++ library
PKG_CHECK_MODULES(ITPP, itpp >= 4.0)
#LIBS="$LIBS $ITPP_LIBS"

dnl HYDRA: Add the Hydra library using the following lines
PKG_CHECK_MODULES(HYDRA, hydra >= 0)
LIBS="$LIBS $HYDRA_LIBS"

dnl META_80211N: IEEE 80211n MIMO Physical Layer
PKG_CHECK_MODULES(META_80211N, meta-80211n >= 0)
LIBS="$LIBS $META_80211N_LIBS"

dnl Define where to find boost includes
dnl defines BOOST_CFLAGS
GR_REQUIRE_BOOST_INCLUDES

DEFINES=""
AC_SUBST(DEFINES)

#STD_DEFINES_AND_INCLUDES="$GNURADIO_CORE_CFLAGS $BOOST_CFLAGS $ITPP_CFLAGS"
STD_DEFINES_AND_INCLUDES="$GNURADIO_CORE_CFLAGS $BOOST_CFLAGS"
STD_DEFINES_AND_INCLUDES="$STD_DEFINES_AND_INCLUDES $HYDRA_CFLAGS $META_80211N_CFLAGS"
AC_SUBST(STD_DEFINES_AND_INCLUDES)

AC_CONFIG_FILES([\
	  Makefile \
	  config/Makefile \
	  doc/Makefile \
    src/Makefile \
	])
 
dnl MIMO: Add NEW Makefiles and run_tests to this command
GRC_MIMO

dnl MIMO: Build demo script
GRC_MIMO_DEMO

dnl MIMO: Add any new run_tests to this command. Create GRC_MIMO_*_RUNTESTS in
dnl        an appropriate grc_*.m4 file
GRC_MIMO_RUNTESTS

AC_OUTPUT
