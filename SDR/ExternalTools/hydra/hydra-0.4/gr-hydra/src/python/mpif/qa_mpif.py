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
import mpif
import core
from functhread import FuncThread
import phy
import rf

import struct
import time
import sys

class FakeRadio(gr.hier_block2):
    
    def __init__(self):
        # instantiate blocks
        self.pad = rf.pad()
        self.sink = rf.fake_tx()
        self.src = rf.fake_rx()

        # connect blocks and call hier_block2 constructor
        gr.hier_block2.__init__(self, "FakeRadio",
                                gr.io_signature(0, 0, 0), 
                                gr.io_signature(1, 1, gr.sizeof_gr_complex))
        self.connect(self.pad, self.sink)
        self.connect(self.src, self)

class qa_mpif (gr_unittest.TestCase):
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
        ##  FIX ME: This test is completely broken!
        #           Don't use it until someone can fix it.
        #
        ntx, G, q = 2, 100, phy.phyglobal.ctrlQ()
        data = "hello"
        v = mpif.txvector()
        v.set_length(len(data) )
        header = v.to_string()
        t = phy.tx(ntx)
        t.set_gain(G)
        mt = mpif.tx()
        mt.set_transmitter(t)
        
        mt.inputQ().insert_tail(gr.message_from_string(header+data, mpif.TXDATA) )
        mt.start()
        time.sleep(0.2) # pause to allow execution of mt
        mt.stop()
        time.sleep(0.2) # pause to allow mt execution to finish
        
        self.assertNotEqual(t.outputQ().count(), q.count() )
        self.assertEqual(t.outputQ().count(), 1)
        m = t.outputQ().delete_head()
        self.assertAlmostEqual (m.arg1(), ntx)
        s = m.to_string()
        x = self.tocomplex(s)
        self.assertAlmostEqual (len(x), int(m.arg1()*m.arg2()) )

    def test_002_rx(self):
        nrx, ntx, q = 1, 1, phy.phyglobal.ctrlQ()
        data = "hello"
        v = mpif.txvector()
        v.set_length(len(data) )
        header = v.to_string()

        tx = phy.tx(ntx)
        mt = mpif.tx()
        mt.set_transmitter(tx)
        mt.inputQ().insert_tail(gr.message_from_string(header+data, mpif.TXDATA, 1.0) )
        q.insert_tail(gr.message_from_string("txdone", mpif.TXDONE, 2.0) )
        
        rx = phy.rx(nrx)
        mr = mpif.rx()
        mr.set_inputQ(rx.outputQ() )
        mr.set_mpiftx(mt)
        q.insert_tail(gr.message_from_string("csbusy", mpif.CSBUSY, 3.0) )
        q.insert_tail(gr.message_from_string(data, mpif.RXDATA, 4.0) )
        
        mt.start()
        mr.start()
        time.sleep(0.2) # pause to allow execution of mr
        mt.stop()
        mr.stop()
        time.sleep(0.2) # pause to allow mr execution to finish

        self.assertEqual (mr.outputQ().count(), 3)
        txproc = mt.get_timer().timebuf()
        rxproc = mr.get_timer().timebuf()
        proc_time = 1.0
        self.assertAlmostEqual(txproc[0], 1.0)
        self.assertAlmostEqual(txproc[1], proc_time)
        self.assertAlmostEqual(rxproc[0], 3.0)
        self.assertAlmostEqual(rxproc[1], proc_time)
        time.sleep(1)

    def stop_radio(self, r, t):
        time.sleep(t)
        r.pad.shutdown()
        r.src.shutdown()

    def test_003_socket(self):
        nrx, ntx, q = 1, 1, phy.phyglobal.ctrlQ()
        sockenable = True
        data = "hello"
        v = mpif.txvector()
        v.set_length(len(data) )
        header = v.to_string()

        tx = phy.tx(ntx)
        rx = phy.rx(nrx)
        radio = FakeRadio()
        tx.set_outputQ(radio.pad.inputQ() )        # connect tx to radio
        radio.sink.set_outputQ(radio.src.inputQ() ) # connect fake tx to fake rx
        self.tb.connect(radio, rx)                 # connect radio to rx
        
        mt = mpif.tx(sockenable)
        mt.set_transmitter(tx)
        
        mr = mpif.rx(sockenable)
        mr.set_inputQ(rx.outputQ() )
        mr.set_mpiftx(mt)

        # listen to mpif_rx, send to mpif_tx
        mac = mpif.socket("0.0.0.0", mr.port(), "0.0.0.0", mt.port() )
        if not mac.open(): return

        mt.start()
        mr.start()
        time.sleep(0.2) # pause to allow execution of mr and mt to start
        mac.send(struct.pack('<i',mpif.TXDATA) + header + data)
        #schedule radio shutdown
        stop_thread = FuncThread(self.stop_radio, "stop_radio", radio, 3)
        stop_thread.start()
        
        self.tb.run()
        q.insert_tail(gr.message_from_string("csbusy", mpif.CSBUSY, 3.0) )
        q.insert_tail(gr.message_from_string(data, mpif.RXDATA, 4.0) )
        mt.stop()
        mr.stop()
        time.sleep(0.2) # pause to allow mr execution to finish
        #self.assertEqual(radio.sink.outputQ().count(), 0)
        self.assertEqual(radio.sink.outputQ().count(), 1)

        v = mpif.rxvector()

        s = mac.recv()
        n = v.from_string(s[gr.sizeof_int:])
        (type, ) = struct.unpack('<i', s[0:gr.sizeof_int])
        self.assertEqual(type, mpif.TXDONE)
        self.assertEqual(len(s), gr.sizeof_int+n)
        
        s = mac.recv()
        (type, ) = struct.unpack('<i', s[0:gr.sizeof_int])
        n = v.from_string(s[gr.sizeof_int:])
        self.assertEqual(type, mpif.CSBUSY)
        self.assertEqual(len(s), gr.sizeof_int+n)

        s = mac.recv()
        (type, ) = struct.unpack('<i', s[0:gr.sizeof_int])
        n = v.from_string(s[gr.sizeof_int:])
        self.assertEqual(type, mpif.RXDATA)
        self.assertEqual(len(s), gr.sizeof_int+n+v.length() )
        self.assertEqual(data, s[gr.sizeof_int+n:] )

        mac.close()

        txproc = mt.get_timer().timebuf()
        rxproc = mr.get_timer().timebuf()
        proc_time = 1.0
        self.assertEqual(len(txproc), 2)
        self.assertAlmostEqual(rxproc[0], 3.0)
        self.assertAlmostEqual(rxproc[1], proc_time)


if __name__ == '__main__':
    gr_unittest.main ()
