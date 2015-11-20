#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Fri Nov 20 14:18:25 2015
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

from gnuradio import analog
from gnuradio import audio
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.wxgui import forms
from gnuradio.wxgui import scopesink2
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import time
import wx


class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_rate_send = samp_rate_send = 250e3
        self.samp_rate_rec = samp_rate_rec = 5000000
        self.audio_rate = audio_rate = 44100
        self.audio_interp = audio_interp = 4
        self.Freq = Freq = 88.5e6

        ##################################################
        # Blocks
        ##################################################
        _Freq_sizer = wx.BoxSizer(wx.VERTICAL)
        self._Freq_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_Freq_sizer,
        	value=self.Freq,
        	callback=self.set_Freq,
        	label='Freq',
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._Freq_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_Freq_sizer,
        	value=self.Freq,
        	callback=self.set_Freq,
        	minimum=80e6,
        	maximum=115e6,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_Freq_sizer)
        self.wxgui_scopesink2_0_0 = scopesink2.scope_sink_f(
        	self.GetWin(),
        	title="Scope Plot",
        	sample_rate=samp_rate_send,
        	v_scale=0,
        	v_offset=0,
        	t_scale=0,
        	ac_couple=False,
        	xy_mode=False,
        	num_inputs=1,
        	trig_mode=wxgui.TRIG_MODE_AUTO,
        	y_axis_label="Counts",
        )
        self.Add(self.wxgui_scopesink2_0_0.win)
        self.wxgui_scopesink2_0 = scopesink2.scope_sink_c(
        	self.GetWin(),
        	title="Scope Plot",
        	sample_rate=samp_rate_send,
        	v_scale=0,
        	v_offset=0,
        	t_scale=0,
        	ac_couple=False,
        	xy_mode=False,
        	num_inputs=1,
        	trig_mode=wxgui.TRIG_MODE_AUTO,
        	y_axis_label="Counts",
        )
        self.Add(self.wxgui_scopesink2_0.win)
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(samp_rate_rec)
        self.uhd_usrp_source_0.set_center_freq(Freq, 0)
        self.uhd_usrp_source_0.set_gain(0, 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate_rec)
        self.uhd_usrp_sink_0.set_center_freq(Freq, 0)
        self.uhd_usrp_sink_0.set_gain(0, 0)
        self.digital_qam_mod_0 = digital.qam.qam_mod(
          constellation_points=16,
          mod_code="gray",
          differential=True,
          samples_per_symbol=2,
          excess_bw=0.35,
          verbose=False,
          log=False,
          )
        self.digital_qam_demod_0 = digital.qam.qam_demod(
          constellation_points=16,
          differential=True,
          samples_per_symbol=2,
          excess_bw=0.35,
          freq_bw=6.28/100.0,
          timing_bw=6.28/100.0,
          phase_bw=6.28/100.0,
          mod_code="gray",
          verbose=False,
          log=False,
          )
        self.blks2_packet_encoder_0 = grc_blks2.packet_mod_f(grc_blks2.packet_encoder(
        		samples_per_symbol=4,
        		bits_per_symbol=4,
        		preamble="",
        		access_code="",
        		pad_for_usrp=True,
        	),
        	payload_length=0,
        )
        self.blks2_packet_decoder_0 = grc_blks2.packet_demod_f(grc_blks2.packet_decoder(
        		access_code="",
        		threshold=-1,
        		callback=lambda ok, payload: self.blks2_packet_decoder_0.recv_pkt(ok, payload),
        	),
        )
        self.audio_sink_0 = audio.sink(44100, "", True)
        self.analog_sig_source_x_0 = analog.sig_source_f(samp_rate_send, analog.GR_COS_WAVE, 1000, 1, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_sig_source_x_0, 0), (self.blks2_packet_encoder_0, 0))    
        self.connect((self.blks2_packet_decoder_0, 0), (self.audio_sink_0, 0))    
        self.connect((self.blks2_packet_decoder_0, 0), (self.wxgui_scopesink2_0_0, 0))    
        self.connect((self.blks2_packet_encoder_0, 0), (self.digital_qam_mod_0, 0))    
        self.connect((self.digital_qam_demod_0, 0), (self.blks2_packet_decoder_0, 0))    
        self.connect((self.digital_qam_mod_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.digital_qam_mod_0, 0), (self.wxgui_scopesink2_0, 0))    
        self.connect((self.uhd_usrp_source_0, 0), (self.digital_qam_demod_0, 0))    


    def get_samp_rate_send(self):
        return self.samp_rate_send

    def set_samp_rate_send(self, samp_rate_send):
        self.samp_rate_send = samp_rate_send
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate_send)
        self.wxgui_scopesink2_0.set_sample_rate(self.samp_rate_send)
        self.wxgui_scopesink2_0_0.set_sample_rate(self.samp_rate_send)

    def get_samp_rate_rec(self):
        return self.samp_rate_rec

    def set_samp_rate_rec(self, samp_rate_rec):
        self.samp_rate_rec = samp_rate_rec
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate_rec)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate_rec)

    def get_audio_rate(self):
        return self.audio_rate

    def set_audio_rate(self, audio_rate):
        self.audio_rate = audio_rate

    def get_audio_interp(self):
        return self.audio_interp

    def set_audio_interp(self, audio_interp):
        self.audio_interp = audio_interp

    def get_Freq(self):
        return self.Freq

    def set_Freq(self, Freq):
        self.Freq = Freq
        self._Freq_slider.set_value(self.Freq)
        self._Freq_text_box.set_value(self.Freq)
        self.uhd_usrp_sink_0.set_center_freq(self.Freq, 0)
        self.uhd_usrp_source_0.set_center_freq(self.Freq, 0)


if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = top_block()
    tb.Start(True)
    tb.Wait()
