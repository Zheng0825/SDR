#
# Copyright (C) 2009  The University of Texas at Austin.
# Copyright 2004 Free Software Foundation, Inc.
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

macdatadir = $(prefix)/share/hydra
macpythondir = $(hydrapythondir)/PyHydra/mac

macpython_PYTHON =  \
	__init__.py		\
	fsm.py			\
	tunconfig.py	\
	tunserver.py	\
	tunclient.py

TESTS =

confs = 	\
	serverconf

noinst_PYTHON = 

EXTRA_DIST = 	\
	$(confs)

install-data-local:
	@for file in $(confs); do \
		echo "$(INSTALL_DATA) -D $(srcdir)/$$file $(DESTDIR)$(macdatadir)/$$file"; \
		$(INSTALL_DATA) -D $(srcdir)/$$file $(DESTDIR)$(macdatadir)/$$file; \
	done

uninstall-local:
	@for file in $(confs); do \
		echo "$(RM) $(DESTDIR)$(macdatadir)/$$file"; \
		$(RM) $(DESTDIR)$(macdatadir)/$$file; \
	done

CLEANFILES = *.pyc *.pyo
