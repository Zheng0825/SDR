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
AC_DEFUN([GRC_HYDRA_MPIF],[
    AC_CONFIG_FILES([\
			src/lib/mpif/Makefile \
			src/lib/mpif/common/Makefile \
			src/lib/mpif/tx/Makefile \
			src/lib/mpif/rx/Makefile \
			src/swig/mpif/Makefile \
			src/python/mpif/Makefile \
			src/python/mpif/run_tests \
	])  
])  

dnl Add any new run_tests to this command
dnl run_tests is created from run_tests.in.  Make it executable.
AC_DEFUN([GRC_HYDRA_MPIF_RUNTESTS],[
		AC_CONFIG_COMMANDS([run_tests_mpif], [chmod +x src/python/mpif/run_tests])
])  
