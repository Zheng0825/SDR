# Makefile.am
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

grmimodatadir = $(prefix)/share/hydra

bin_SCRIPTS = \
	hydra-demo		\
	hydra-demo-csma	\
	hydra-mimo		\
	hydra-tunnel 	\
	hydra-csma

confs =	\
	sconf-1	\
	sconf-2	\
	sconf-3	\
	sconf-4

EXTRA_DIST = 	\
	hydra-demo.in		\
	hydra-demo-csma.in	\
	hydra-mimo.in		\
	hydra-tunnel.in 	\
	hydra-csma.in		\
	$(confs)

install-data-local:
	@for file in $(confs); do \
		echo "$(INSTALL_DATA) -D $(srcdir)/$$file $(DESTDIR)$(grmimodatadir)/$$file"; \
		$(INSTALL_DATA) -D $(srcdir)/$$file $(DESTDIR)$(grmimodatadir)/$$file; \
	done

uninstall-local:
	@for file in $(confs); do \
		echo "$(RM) $(DESTDIR)$(grmimodatadir)/$$file"; \
		$(RM) $(DESTDIR)$(grmimodatadir)/$$file; \
	done

