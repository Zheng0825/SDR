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
AC_DEFUN([GRC_META_80211N],[
    GRC_ENABLE([meta-80211n])

    havemeta80211n=yes
    PKG_CHECK_MODULES(META_80211N, meta-80211n >= 0, [],
      [havemeta80211n=no; AC_MSG_RESULT([meta-80211n is not available.])])
    AC_SUBST(META_80211N_LIBS)
    AC_SUBST(META_80211N_CFLAGS)

    AM_CONDITIONAL([HAVE_META_80211N], [test x$havemeta80211n = xyes])
])
