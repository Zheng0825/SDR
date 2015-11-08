#! /usr/bin/env python
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

from gnuradio import gr, gr_unittest
import gui
import matplotlib
import numpy

class qa_gui (gr_unittest.TestCase):
    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_helper(self):
        x = (complex(1,1), complex(-1,-1), complex(-1,1) )
        s = gui.helper.to_string_complex(x[0:])
        v = gui.helper.to_cvector(s)
        self.assertComplexTuplesAlmostEqual(x, v, len(x)+1)

if __name__ == '__main__':
    gr_unittest.main ()
