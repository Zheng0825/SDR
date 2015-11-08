#!/usr/bin/env python
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

from gnuradio import gr, gr_unittest
import phy
import struct
import time

class qa_phy (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def tocomplex(self, s):
        c = []
        for k in range(len(s)/8):
            fr, fi = struct.unpack('<ff', s[k*8:k*8+8])
            c.append( complex(fr,fi) )
        return c

    def test_001_tx(self):
        ntx, q = 2, phy.phyglobal.ctrlQ()
        s = "hello"
        t = phy.tx(ntx)
        
        G = 100
        Gdb = 20
        t.set_gain_db(Gdb)
        self.assertAlmostEqual(G, t.gain() )
        t.set_gain(G)
        self.assertAlmostEqual(Gdb, t.gain_db() )
        
        t.transmit(s)
        self.assertNotEqual(t.outputQ().count(), q.count() )
        m = t.outputQ().delete_head()
        self.assertAlmostEqual (m.arg1(), ntx)
        s = m.to_string()
        x = self.tocomplex(s)
        self.assertAlmostEqual (len(x), m.arg1()*m.arg2() )

    def test_002_rx(self):
        nrx, ntx, q = 1, 1, phy.phyglobal.ctrlQ()
        s = "hello"
        q.insert_tail(gr.message_from_string(s) )
        tx = phy.tx(ntx)
        rx = phy.rx(nrx)
        tx.transmit(s)
        x = self.tocomplex(tx.outputQ().delete_head().to_string() )
        
        src = gr.vector_source_c (x)
        self.tb.connect(src, rx)
        self.tb.run()
        self.assertEqual (rx.outputQ().count(), 1)
        m = rx.outputQ().delete_head().to_string()
        self.assertEqual (m, s)

if __name__ == '__main__':
  gr_unittest.main ()
