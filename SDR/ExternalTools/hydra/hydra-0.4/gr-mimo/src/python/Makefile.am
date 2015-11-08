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

EXTRA_DIST = run_tests.in

mimo_pythondir = $(hydrapythondir)/PyMimo

mimo_python_PYTHON = 	\
	__init__.py								\
	mimo_adapt.py							\
	mimo_channel.py						\
	mimo_control.py						\
	mimo_csi.py								\
	mimo_mpif.py							\
	mimo_rate_history.py			\
	mimo_self_adaptation.py		\
	mimo_transceiver.py				\
	mimo_manager.py						\
	dot11mac.py								\
	csma.py

TESTS = 				\
	run_tests


noinst_PYTHON = 	\
	qa_mimo.py			\
	run_tunserver.py

CLEANFILES = *.pyc
