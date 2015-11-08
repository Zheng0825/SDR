## Makefile.am
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

patchsrcdir=$(top_srcdir)/src/patch/source
patchscriptsdir=$(top_srcdir)/src/patch/scripts

EXTRA_DIST = \
	gnuradio_core_patch.in \
	gr_usrp_patch.in

BUILT_SOURCES = \
	gnuradio_core_patch-install \
	gr_usrp_patch-install


uninstall-am: \
	gnuradio_core_patch-uninstall \
	gr_usrp_patch-uninstall


gnuradio_core_patch-install: \
	$(patchsrcdir)/gnuradio-core.src.lib.runtime.gr_basic_block.fix.h \
	$(patchsrcdir)/gnuradio-core.src.lib.runtime.gr_basic_block.fix.i \
	$(patchsrcdir)/gnuradio-core.src.lib.runtime.gr_flat_flowgraph.fix.cc
	$(patchscriptsdir)/gnuradio_core_patch

gr_usrp_patch-install: \
	$(patchsrcdir)/gr-usrp.src.usrp_source_base.fix.cc \
	$(patchsrcdir)/gr-usrp.src.usrp_source_base.fix.h
	$(patchscriptsdir)/gr_usrp_patch

gnuradio_core_patch-uninstall: \
	$(patchsrcdir)/gnuradio-core.src.lib.runtime.gr_basic_block.fix.h \
	$(patchsrcdir)/gnuradio-core.src.lib.runtime.gr_basic_block.fix.i \
	$(patchsrcdir)/gnuradio-core.src.lib.runtime.gr_flat_flowgraph.fix.cc
	$(patchscriptsdir)/gnuradio_core_patch uninstall

gr_usrp_patch-uninstall: \
	$(patchsrcdir)/gr-usrp.src.usrp_source_base.fix.cc \
	$(patchsrcdir)/gr-usrp.src.usrp_source_base.fix.h
	$(patchscriptsdir)/gr_usrp_patch uninstall

CLEANFILES = *.bak
