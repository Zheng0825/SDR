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
AC_DEFUN([GRC_SELF_ADAPTATION],[
    GRC_ENABLE([self-adaptation])

    AC_CONFIG_FILES([\
        src/self-adaptation/Makefile \
    ])

    passed=yes
    PKG_CHECK_MODULES(ITPP, itpp >= 4, [],
      [passed=no; AC_MSG_RESULT([self-adaptation requires IT++ 4.0 or better.])])

    # Don't build self-adaptation if additional features have been skipped
    # This isn't currently being used for anything
    for dir in $skipped_dirs
    do
      if test "$dir" = "ann"; then
        AC_MSG_RESULT([Component self-adaptation requires ann, which is not built.])
        passed=no
      fi
    done

    GRC_BUILD_CONDITIONAL([self-adaptation],[
      #AC_SUBST(ITPP_LIBS)
    ])
])
