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

SUBDIRS = core phy rf mpif gui

DIST_SUBDIRS = core phy rf mpif gui

#	install __init__.py to glue together installed hydra python directories
ourpythondir = $(hydrapythondir)
ourpython_PYTHON = __init__.py

CLEANFILES = *.pyc
