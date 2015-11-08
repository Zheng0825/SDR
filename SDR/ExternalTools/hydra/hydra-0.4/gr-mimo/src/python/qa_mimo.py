#!/usr/bin/env python
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

from gnuradio import gr, gr_unittest
from hydra.PyHydra import GuiHelper, FuncThread
import mimo
import phy
import mpif
import gui
import rf
import time
import core

import numpy
import struct
import time

class FakeRadio(object):
    
    def __init__(self):
        # instantiate blocks
        self.pad = rf.pad()
        self.sink = rf.fake_tx()
        self.src = rf.fake_rx()

    def connect(self, fg):
        fg.connect(self.pad, self.sink)

class qa_mimo (gr_unittest.TestCase):
    def setUp (self):
        self.fg = gr.flow_graph ()

    def tearDown (self):
        self.fg = None

    def disable_packet_detector(self, pd, delay=0.0):
        if (delay>0.0): time.sleep(delay)
        pd.set_nchannels(0)

    ##  FIX ME: Does not exit correctly. This is part of gr-hydra threading bug.
    #
    def not_test_001_phy (self):
        Ntx, Nrx, MCS, s = 2, 2, 3, "helloworld"*150
        No_db = -30
        tx = mimo.tx(Ntx)
        fg = gr.flow_graph()
        radio = FakeRadio()
        radio.sink.set_nchannels(Ntx)
        radio.src.set_nchannels(Nrx)
        v = mimo.txvector()
        # check for inheritance
        self.assertTrue (isinstance(tx, phy.tx().__class__) )
        self.assertTrue (isinstance(v,  mpif.txvector().__class__) )

        tx.set_outputQ(radio.pad.inputQ() ) # connect tx to radio
        fg.connect(radio.pad, radio.sink)   # connect radio.pad to radio.tx
        
        v.set_length(len(s) )
        v.set_rate(MCS)
        tx.configure(v)
        tx.transmit(s)              # send data
        radio.pad.shutdown()        # schedule radio.tx for shutdown
        fg.run()

        self.emulate_channel(radio.sink, radio.src, Ntx, Nrx, No_db)
        self.stop_radio(radio)

        # TEST receiver
        rx = mimo.rx(Nrx)
        pd = rx.packet_detector()
        #pd.set_nchannels(1)
        pd.set_thresh(0.80)
        pd.set_window(32)
        pd.set_delay(16)
        ff = FuncThread(lambda: self.disable_packet_detector(pd, 0.5) )
        ff.start()
        self.fg.connect(radio.src,rx)
        self.fg.run()

        ## check phyglobal.ctrlQ()
        self.assertEqual (phy.phyglobal.ctrlQ().count(), 3)
        m = phy.phyglobal.ctrlQ().delete_head()
        self.assertEqual(m.type(), mpif.TXDONE)
        m = phy.phyglobal.ctrlQ().delete_head()
        self.assertEqual(m.type(), mpif.CSBUSY)
        ts_csbusy = m.arg1()
        m = phy.phyglobal.ctrlQ().delete_head()
        self.assertEqual(m.type(), mpif.RXDATA)
        ts_rxdata = m.arg1()
        print "RxProcTime = %.3f msec"%((ts_rxdata - ts_csbusy)*1.0e3)
        c = m.to_string()
        u = mimo.rxvector()
        vlen = u.from_string(c)
        self.assertComplexAlmostEqual2(-No_db, 10.0*numpy.log10(u.snr() ), -No_db*0.10)
        self.assertEqual(MCS, u.rate())
        self.assertEqual(len(s), u.length_var() )
        self.assertEqual(u.length()+vlen, len(c) )
        #print "========== c[data] =", c[vlen+u.length_meta()+u.length_base():]
        #print "========== s       =", s
        self.assertTrue(c[vlen+u.length_meta()+u.length_base():]==s)
        print "RxVector.length_meta() =", u.length_meta()
        print "RxVector.length_base() =", u.length_base()
        print "RxVector.length_bar()  =", u.length_var()
        print "RxVector.rate()   =", u.rate()
        if (u.snr()>0.0):
            print "RxVector.snr()    = %.3f dB"%(10.0*numpy.log10(u.snr() ) )
        else:
            print "RxVector.snr()    = ", u.snr()

    def emulate_channel (self, src, dst, Ntx, Nrx, No_db):
        m = src.outputQ().delete_head()
        z = GuiHelper.to_vector(m.to_string() )
        l = len(z) * gr.sizeof_gr_complex
        self.assertEqual (l, m.length() )

        noise_gain = 10.0**(No_db/10.0)
        nr = numpy.random.randn(len(z) ) * numpy.sqrt(0.5*noise_gain/float(Ntx))
        ni = numpy.random.randn(len(z) ) * numpy.sqrt(0.5*noise_gain/float(Ntx))
        awgn = nr + complex(0,1)*ni
        y = numpy.array(z) + awgn

        d = GuiHelper.to_string(tuple(y.tolist() ) )
        ymsg = gr.message_from_string(d)

        dst.inputQ().insert_tail(ymsg)    # insert modified waveform

    def stop_radio(self, r, t=0.0):
        if (time > 0.0): time.sleep(t)
        r.pad.shutdown()
        r.src.shutdown()

    def test_002_mpif(self):
        Ntx, Nrx, MCS = 2, 2, 0
        No_db = -30
        sockenable = True
        data = "helloworld"*20
        v = mimo.txvector()
        v.set_length(len(data) )
        v.set_rate(MCS)
        header = v.to_string()

        tx = mimo.tx(Ntx)
        rx = mimo.rx(Nrx)
        radio = FakeRadio()
        radio.sink.set_nchannels(Ntx)
        radio.src.set_nchannels(Nrx)

        tx.set_outputQ(radio.pad.inputQ() ) # connect tx to radio
        radio.connect(self.fg)              # connect radio.pad to radio.tx
        #radio.sink.set_outputQ(radio.src.inputQ() ) # connect fake tx directly to fake rx
        self.fg.connect(radio.src, rx)      # connect radio to rx

        mt = mimo.mpiftx(sockenable)
        mt.set_transmitter(tx)

        mr = mimo.mpifrx(sockenable)
        mr.set_inputQ(rx.outputQ() )
        mr.set_mpiftx(mt)

        # listen to mpif_rx, send to mpif_tx
        mac = mpif.socket("0.0.0.0", mr.port(), "0.0.0.0", mt.port() )
        if not mac.open(): return
        
        mt.start()
        mr.start()
        time.sleep(0.2) # pause to allow execution of mr and mt to start
        mac.ssend(struct.pack('<i',mpif.TXDATA) + header + data)
        # schedule channel emulation and radio shutdown
        ch = phy.channel()
        ch.set_noise_power_db(No_db)
        ch.set_inputQ(radio.sink.outputQ() )
        ch.set_outputQ(radio.src.inputQ() )
        ch.start()

        stop_thread = FuncThread(self.stop_radio, "stop_radio", radio, 0.5 )
        stop_thread.start()
        self.fg.run()
        mt.stop()
        mr.stop()
        ch.shutdown()
        time.sleep(0.5) # pause to allow mr execution to finish
        self.assertEqual(radio.sink.outputQ().count(), 0)

        u = mimo.rxvector()

        s = mac.srecv()
        n = u.from_string(s[gr.sizeof_int:])
        (type, ) = struct.unpack('<i', s[0:gr.sizeof_int])
        self.assertEqual(type, mpif.TXDONE)
        self.assertEqual(len(s), gr.sizeof_int+n)
        
        s = mac.srecv()
        (type, ) = struct.unpack('<i', s[0:gr.sizeof_int])
        n = u.from_string(s[gr.sizeof_int:])
        self.assertEqual(type, mpif.CSBUSY)
        self.assertEqual(len(s), gr.sizeof_int+n)

        s = mac.srecv()
        (type, ) = struct.unpack('<i', s[0:gr.sizeof_int])
        n = u.from_string(s[gr.sizeof_int:])
        self.assertEqual(type, mpif.RXDATA)
        self.assertEqual(len(s), gr.sizeof_int+n+u.length() )

        mac.close()

        txproc = mt.get_timer().timebuf()
        rxproc = mr.get_timer().timebuf()
        self.assertEqual(len(txproc), 2)
        self.assertEqual(len(rxproc), 2)
        
        print "RxProcTime = %.3f msec"%((rxproc[1] )*1.0e3)
        c = s[gr.sizeof_int:]
        u = mimo.rxvector()
        vlen = u.from_string(c)
        self.assertComplexAlmostEqual2(-No_db, 10.0*numpy.log10(u.snr() ), -No_db*0.10)
        self.assertEqual(MCS, u.rate())
        self.assertEqual(len(data), u.length_var() )
        self.assertEqual(u.length()+vlen, len(c) )
        self.assertTrue(c[vlen+u.length_meta()+u.length_base():]==data)
        print "RxVector.length_meta() =", u.length_meta()
        print "RxVector.length_base() =", u.length_base()
        print "RxVector.length_bar()  =", u.length_var()
        print "RxVector.rate()   =", u.rate()
        if (u.snr()>0.0):
            print "RxVector.snr()    = %.3f dB"%(10.0*numpy.log10(u.snr() ) )
        else:
            print "RxVector.snr()    = ", u.snr()

if __name__ == '__main__':
    gr_unittest.main ()
