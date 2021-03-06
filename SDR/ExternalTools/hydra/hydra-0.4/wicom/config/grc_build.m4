dnl 
dnl Copyright (C) 2009  The University of Texas at Austin.
dnl 
dnl This file is part of Hydra: A wireless multihop testbed.
dnl 
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.
dnl 

# Create --enable-foo argument for named component, create variables as needed
# $1 is component name
AC_DEFUN([GRC_ENABLE],[
    AC_ARG_ENABLE([$1],
		  AC_HELP_STRING([--enable-$1],
				 [Fail build if $1 fails configuration]),
		  [],
                  [[enable_]m4_bpatsubst($1,-,_)=$enable_all_components])
])

# Conditionally build named component.
# $1 is component name
# $2 is executed if configuration passes and build is desired
# Use $passed=no to indicate configuration failure
# Any other value of $passed, including blank, assumes success 
AC_DEFUN([GRC_BUILD_CONDITIONAL],[
    if test x$passed = xno; then
        skipped_dirs="$skipped_dirs $1"
	if test x$[enable_]m4_bpatsubst($1,-,_) = xyes; then
	    AC_MSG_ERROR([Component $1 has errors, stopping.])
	else
	    AC_MSG_RESULT([Not building component $1.])
	fi	
    else
        if test x$[enable_]m4_bpatsubst($1,-,_) != xno; then
	    $2
	    build_dirs="$build_dirs $1"
	    AC_MSG_RESULT([Component $1 passed configuration checks, building.])
	else
            skipped_dirs="$skipped_dirs $1"
	    AC_MSG_RESULT([Component $1 passed configuration checks, but not building.])
	fi
    fi
])
