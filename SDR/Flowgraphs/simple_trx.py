#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: BroadcastMAC
<<<<<<< HEAD
# Generated: Tue Feb  9 14:04:34 2016
=======
# Generated: Tue Feb  9 13:21:19 2016
>>>>>>> 6762b79d17fc58ed8a4620b9dc18412bfe7ff43e
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from gmsk_radio import gmsk_radio  # grc-generated hier_block
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.wxgui import forms
from gnuradio.wxgui import scopesink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import mac
import pmt
import wx


class simple_trx(grc_wxgui.top_block_gui):

    def __init__(self, ampl=0.7, args='', arq_timeout=.1*0 + 0.04, dest_addr=-1, iface='tun0', max_arq_attempts=5 * 2, mtu=128, port="12345", radio_addr=0, rate=1e6, rx_antenna="TX/RX", rx_freq=915e6, rx_gain=65-20, rx_lo_offset=0, samps_per_sym=4, tx_freq=915e6, tx_gain=45, tx_lo_offset=0):
        grc_wxgui.top_block_gui.__init__(self, title="BroadcastMAC")

        ##################################################
        # Parameters
        ##################################################
        self.ampl = ampl
        self.args = args
        self.arq_timeout = arq_timeout
        self.dest_addr = dest_addr
        self.iface = iface
        self.max_arq_attempts = max_arq_attempts
        self.mtu = mtu
        self.port = port
        self.radio_addr = radio_addr
        self.rate = rate
        self.rx_antenna = rx_antenna
        self.rx_freq = rx_freq
        self.rx_gain = rx_gain
        self.rx_lo_offset = rx_lo_offset
        self.samps_per_sym = samps_per_sym
        self.tx_freq = tx_freq
        self.tx_gain = tx_gain
        self.tx_lo_offset = tx_lo_offset

        ##################################################
        # Variables
        ##################################################
        self.user_tx_gain = user_tx_gain = tx_gain
        self.user_rx_gain = user_rx_gain = rx_gain
        self.samp_rate = samp_rate = rate

        ##################################################
        # Blocks
        ##################################################
        _user_tx_gain_sizer = wx.BoxSizer(wx.VERTICAL)
        self._user_tx_gain_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_user_tx_gain_sizer,
        	value=self.user_tx_gain,
        	callback=self.set_user_tx_gain,
        	label="TX Gain",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._user_tx_gain_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_user_tx_gain_sizer,
        	value=self.user_tx_gain,
        	callback=self.set_user_tx_gain,
        	minimum=0,
        	maximum=90,
        	num_steps=90,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_user_tx_gain_sizer)
        _user_rx_gain_sizer = wx.BoxSizer(wx.VERTICAL)
        self._user_rx_gain_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_user_rx_gain_sizer,
        	value=self.user_rx_gain,
        	callback=self.set_user_rx_gain,
        	label="RX Gain",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._user_rx_gain_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_user_rx_gain_sizer,
        	value=self.user_rx_gain,
        	callback=self.set_user_rx_gain,
        	minimum=0,
        	maximum=90,
        	num_steps=90,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_user_rx_gain_sizer)
        self.mac_802_3_tracker = mac.tracker_802_3(verbose=False)
        self.wxgui_scopesink2_0_0 = scopesink2.scope_sink_c(
        	self.GetWin(),
        	title="RX",
        	sample_rate=samp_rate,
        	v_scale=0.02,
        	v_offset=0,
        	t_scale=0.0001,
        	ac_couple=False,
        	xy_mode=False,
        	num_inputs=2,
        	trig_mode=wxgui.TRIG_MODE_NORM,
        	y_axis_label="Counts",
        )
        self.GridAdd(self.wxgui_scopesink2_0_0.win, 0, 0, 1, 1)
        self.simple_mac_1 = mac.simple_mac(
        radio_addr,
        arq_timeout,
        max_arq_attempts,
        2.0,
        False,
        0.05,
        node_expiry_delay=10.0,
        expire_on_arq_failure=False,
        only_send_if_alive=False,
        prepend_dummy=False,
        )
        self.mac_virtual_channel_encoder_0_0 = mac.virtual_channel_encoder(-1, True,mtu=mtu,
        chan_id=1,
        prepend_dummy=False,
        chan_tracker=self.mac_802_3_tracker,
        )
        self.mac_virtual_channel_encoder_0 = mac.virtual_channel_encoder(-1, True,mtu=mtu,
        chan_id=0,
        prepend_dummy=False,
        )
        self.mac_virtual_channel_decoder_0 = mac.virtual_channel_decoder(3, [0, 1])
        self.gmsk_radio_0 = gmsk_radio(
<<<<<<< HEAD
            access_code_threshold=0 + 12 + 4*0,
            samps_per_sym=samps_per_sym,
            tx_lo_offset=tx_lo_offset,
            rx_lo_offset=rx_lo_offset,
            ampl=ampl,
            rx_gain=user_rx_gain,
            rx_freq=rx_freq,
            rx_ant=rx_antenna,
            tx_freq=tx_freq,
            tx_gain=user_tx_gain,
            args=args,
            rate=samp_rate,
=======
            rate=samp_rate,
            args=args,
            tx_gain=user_tx_gain,
            tx_freq=tx_freq,
            rx_ant=rx_antenna,
            rx_freq=rx_freq,
            rx_gain=user_rx_gain,
            ampl=ampl,
            rx_lo_offset=rx_lo_offset,
            tx_lo_offset=tx_lo_offset,
            samps_per_sym=samps_per_sym,
            access_code_threshold=0 + 12 + 4*0,
>>>>>>> 6762b79d17fc58ed8a4620b9dc18412bfe7ff43e
        )
        self.blocks_tuntap_pdu_0 = blocks.tuntap_pdu(iface, mtu*0 + 1514, False)
        self.blocks_socket_pdu_0 = blocks.socket_pdu("TCP_SERVER", "", port, mtu, False)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((1, ))
        self.blocks_moving_average_xx_0 = blocks.moving_average_ff(10000, 1./10000, 40000/4)
        self.blocks_message_strobe_0 = blocks.message_strobe(pmt.intern("T"), 1)
        self.blocks_message_debug_0_0_1 = blocks.message_debug()
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(1)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_message_strobe_0, 'strobe'), (self.simple_mac_1, 'ctrl_in'))    
        self.msg_connect((self.blocks_socket_pdu_0, 'pdus'), (self.mac_virtual_channel_encoder_0, 'in'))    
        self.msg_connect((self.blocks_tuntap_pdu_0, 'pdus'), (self.mac_virtual_channel_encoder_0_0, 'in'))    
        self.msg_connect((self.gmsk_radio_0, 'msg_out'), (self.simple_mac_1, 'from_radio'))    
        self.msg_connect((self.mac_virtual_channel_decoder_0, 'out2'), (self.blocks_message_debug_0_0_1, 'print'))    
        self.msg_connect((self.mac_virtual_channel_decoder_0, 'out0'), (self.blocks_socket_pdu_0, 'pdus'))    
        self.msg_connect((self.mac_virtual_channel_decoder_0, 'out1'), (self.blocks_tuntap_pdu_0, 'pdus'))    
        self.msg_connect((self.mac_virtual_channel_decoder_0, 'out2'), (self.blocks_tuntap_pdu_0, 'pdus'))    
        self.msg_connect((self.mac_virtual_channel_decoder_0, 'out1'), (self.mac_802_3_tracker, 'in'))    
        self.msg_connect((self.mac_virtual_channel_encoder_0, 'out'), (self.simple_mac_1, 'from_app_arq'))    
        self.msg_connect((self.mac_virtual_channel_encoder_0_0, 'out'), (self.simple_mac_1, 'from_app_arq'))    
        self.msg_connect((self.simple_mac_1, 'to_radio'), (self.gmsk_radio_0, 'msg_in'))    
        self.msg_connect((self.simple_mac_1, 'to_app'), (self.mac_virtual_channel_decoder_0, 'in'))    
        self.connect((self.blocks_complex_to_mag_0, 0), (self.blocks_float_to_complex_0, 0))    
        self.connect((self.blocks_complex_to_mag_0, 0), (self.blocks_moving_average_xx_0, 0))    
        self.connect((self.blocks_float_to_complex_0, 0), (self.wxgui_scopesink2_0_0, 1))    
        self.connect((self.blocks_moving_average_xx_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_float_to_complex_0, 1))    
        self.connect((self.gmsk_radio_0, 0), (self.blocks_complex_to_mag_0, 0))    
        self.connect((self.gmsk_radio_0, 0), (self.wxgui_scopesink2_0_0, 0))    

    def get_ampl(self):
        return self.ampl

    def set_ampl(self, ampl):
        self.ampl = ampl
        self.gmsk_radio_0.set_ampl(self.ampl)

    def get_args(self):
        return self.args

    def set_args(self, args):
        self.args = args
        self.gmsk_radio_0.set_args(self.args)

    def get_arq_timeout(self):
        return self.arq_timeout

    def set_arq_timeout(self, arq_timeout):
        self.arq_timeout = arq_timeout

    def get_dest_addr(self):
        return self.dest_addr

    def set_dest_addr(self, dest_addr):
        self.dest_addr = dest_addr

    def get_iface(self):
        return self.iface

    def set_iface(self, iface):
        self.iface = iface

    def get_max_arq_attempts(self):
        return self.max_arq_attempts

    def set_max_arq_attempts(self, max_arq_attempts):
        self.max_arq_attempts = max_arq_attempts

    def get_mtu(self):
        return self.mtu

    def set_mtu(self, mtu):
        self.mtu = mtu

    def get_port(self):
        return self.port

    def set_port(self, port):
        self.port = port

    def get_radio_addr(self):
        return self.radio_addr

    def set_radio_addr(self, radio_addr):
        self.radio_addr = radio_addr

    def get_rate(self):
        return self.rate

    def set_rate(self, rate):
        self.rate = rate
        self.set_samp_rate(self.rate)

    def get_rx_antenna(self):
        return self.rx_antenna

    def set_rx_antenna(self, rx_antenna):
        self.rx_antenna = rx_antenna
        self.gmsk_radio_0.set_rx_ant(self.rx_antenna)

    def get_rx_freq(self):
        return self.rx_freq

    def set_rx_freq(self, rx_freq):
        self.rx_freq = rx_freq
        self.gmsk_radio_0.set_rx_freq(self.rx_freq)

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain
        self.set_user_rx_gain(self.rx_gain)

    def get_rx_lo_offset(self):
        return self.rx_lo_offset

    def set_rx_lo_offset(self, rx_lo_offset):
        self.rx_lo_offset = rx_lo_offset
        self.gmsk_radio_0.set_rx_lo_offset(self.rx_lo_offset)

    def get_samps_per_sym(self):
        return self.samps_per_sym

    def set_samps_per_sym(self, samps_per_sym):
        self.samps_per_sym = samps_per_sym
        self.gmsk_radio_0.set_samps_per_sym(self.samps_per_sym)

    def get_tx_freq(self):
        return self.tx_freq

    def set_tx_freq(self, tx_freq):
        self.tx_freq = tx_freq
        self.gmsk_radio_0.set_tx_freq(self.tx_freq)

    def get_tx_gain(self):
        return self.tx_gain

    def set_tx_gain(self, tx_gain):
        self.tx_gain = tx_gain
        self.set_user_tx_gain(self.tx_gain)

    def get_tx_lo_offset(self):
        return self.tx_lo_offset

    def set_tx_lo_offset(self, tx_lo_offset):
        self.tx_lo_offset = tx_lo_offset
        self.gmsk_radio_0.set_tx_lo_offset(self.tx_lo_offset)

    def get_user_tx_gain(self):
        return self.user_tx_gain

    def set_user_tx_gain(self, user_tx_gain):
        self.user_tx_gain = user_tx_gain
        self._user_tx_gain_slider.set_value(self.user_tx_gain)
        self._user_tx_gain_text_box.set_value(self.user_tx_gain)
        self.gmsk_radio_0.set_tx_gain(self.user_tx_gain)

    def get_user_rx_gain(self):
        return self.user_rx_gain

    def set_user_rx_gain(self, user_rx_gain):
        self.user_rx_gain = user_rx_gain
        self._user_rx_gain_slider.set_value(self.user_rx_gain)
        self._user_rx_gain_text_box.set_value(self.user_rx_gain)
        self.gmsk_radio_0.set_rx_gain(self.user_rx_gain)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.gmsk_radio_0.set_rate(self.samp_rate)
        self.wxgui_scopesink2_0_0.set_sample_rate(self.samp_rate)


def argument_parser():
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    parser.add_option(
        "", "--ampl", dest="ampl", type="eng_float", default=eng_notation.num_to_str(0.7),
        help="Set TX BB amp [default=%default]")
    parser.add_option(
        "-a", "--args", dest="args", type="string", default='',
        help="Set USRP device args [default=%default]")
    parser.add_option(
        "-t", "--arq-timeout", dest="arq_timeout", type="eng_float", default=eng_notation.num_to_str(.1*0 + 0.04),
        help="Set ARQ timeout [default=%default]")
    parser.add_option(
        "-d", "--dest-addr", dest="dest_addr", type="intx", default=-1,
        help="Set Destination address [default=%default]")
    parser.add_option(
        "", "--iface", dest="iface", type="string", default='tun0',
        help="Set Interface name [default=%default]")
    parser.add_option(
        "", "--max-arq-attempts", dest="max_arq_attempts", type="intx", default=5 * 2,
        help="Set Max ARQ attempts [default=%default]")
    parser.add_option(
        "", "--mtu", dest="mtu", type="intx", default=128,
        help="Set MTU [default=%default]")
    parser.add_option(
        "", "--port", dest="port", type="string", default="12345",
        help="Set TCP port [default=%default]")
    parser.add_option(
        "-l", "--radio-addr", dest="radio_addr", type="intx", default=0,
        help="Set Local address [default=%default]")
    parser.add_option(
        "-r", "--rate", dest="rate", type="eng_float", default=eng_notation.num_to_str(1e6),
        help="Set Sample rate [default=%default]")
    parser.add_option(
        "-A", "--rx-antenna", dest="rx_antenna", type="string", default="TX/RX",
        help="Set RX antenna [default=%default]")
    parser.add_option(
        "", "--rx-freq", dest="rx_freq", type="eng_float", default=eng_notation.num_to_str(915e6),
        help="Set RX freq [default=%default]")
    parser.add_option(
        "", "--rx-gain", dest="rx_gain", type="eng_float", default=eng_notation.num_to_str(65-20),
        help="Set RX gain [default=%default]")
    parser.add_option(
        "", "--rx-lo-offset", dest="rx_lo_offset", type="eng_float", default=eng_notation.num_to_str(0),
        help="Set RX LO offset [default=%default]")
    parser.add_option(
        "", "--samps-per-sym", dest="samps_per_sym", type="intx", default=4,
        help="Set Samples/symbol [default=%default]")
    parser.add_option(
        "", "--tx-freq", dest="tx_freq", type="eng_float", default=eng_notation.num_to_str(915e6),
        help="Set TX freq [default=%default]")
    parser.add_option(
        "", "--tx-gain", dest="tx_gain", type="eng_float", default=eng_notation.num_to_str(45),
        help="Set TX gain [default=%default]")
    parser.add_option(
        "", "--tx-lo-offset", dest="tx_lo_offset", type="eng_float", default=eng_notation.num_to_str(0),
        help="Set TX LO offset [default=%default]")
    return parser


def main(top_block_cls=simple_trx, options=None):
    if options is None:
        options, _ = argument_parser().parse_args()

    tb = top_block_cls(ampl=options.ampl, args=options.args, arq_timeout=options.arq_timeout, dest_addr=options.dest_addr, iface=options.iface, max_arq_attempts=options.max_arq_attempts, mtu=options.mtu, port=options.port, radio_addr=options.radio_addr, rate=options.rate, rx_antenna=options.rx_antenna, rx_freq=options.rx_freq, rx_gain=options.rx_gain, rx_lo_offset=options.rx_lo_offset, samps_per_sym=options.samps_per_sym, tx_freq=options.tx_freq, tx_gain=options.tx_gain, tx_lo_offset=options.tx_lo_offset)
    tb.Start(True)
    tb.Wait()


if __name__ == '__main__':
    main()
