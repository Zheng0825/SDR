#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
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

AC_PREREQ(2.59)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
AM_INIT_AUTOMAKE(meta-80211n, 0.1)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_PROG_LIBTOOL

# Configuration macros
LF_SET_WARNINGS

# Checks for libraries.

# Define where to look for cppunit includes and libs
# sets CPPUNIT_CFLAGS and CPPUNIT_LIBS
AM_PATH_CPPUNIT([1.9.14],[],
                [AC_MSG_ERROR([meta-80211n requires cppunit.  Stop])])

CPPUNIT_INCLUDES=$CPPUNIT_CFLAGS
AC_SUBST(CPPUNIT_INCLUDES)

# Check for IT++ library
PKG_CHECK_MODULES(ITPP, itpp >= 4)
#LIBS="$LIBS $ITPP_LIBS"

# Check for wicom library
PKG_CHECK_MODULES(WICOM, wicom >= 0)
LIBS="$LIBS $WICOM_LIBS"

AC_MSG_CHECKING(--enable-hydra argument)
AC_ARG_ENABLE(hydra,
              [  --enable-hydra   gr-hydra included.],
              [enable_hydra=$enableval],
              [enable_hydra="no"])
AC_MSG_RESULT($enable_hydra)
if test "$enable_hydra" = "yes"; then
  # Check for hydra library
  PKG_CHECK_MODULES(HYDRA, hydra >= 0)
  LIBS="$LIBS $HYDRA_LIBS"
fi

# Checks for header files.
AC_CHECK_HEADERS([sys/time.h])

DEFINES=""
AC_SUBST(DEFINES)

# Additional defines and includes
#STD_DEFINES_AND_INCLUDES="$HYDRA_INCLUDES $ITPP_INCLUDES WICOM_INCLUDES $CPPUNIT_CFLAGS"
STD_DEFINES_AND_INCLUDES="$HYDRA_INCLUDES $WICOM_INCLUDES $CPPUNIT_CFLAGS"
AC_SUBST(STD_DEFINES_AND_INCLUDES)

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Automake makefiles
AC_CONFIG_FILES([Makefile
                 config/Makefile])
GRC_META_80211N

AC_OUTPUT

# Add HAVE_HYDRA definition to config.h
hydra_included="no"
if test "$enable_hydra" = "yes"; then
  if test -e "config.h"; then
    echo -e "\n/* Define to 1 if you have libhydra.la */" >> config.h
    echo -e "#ifndef HAVE_HYDRA"   >> config.h
    echo -e "#define HAVE_HYDRA 1" >> config.h
    echo -e "#endif"               >> config.h
  fi
  hydra_included="yes"
  AC_SUBST([hydra_included], [$hydra_included])
fi
