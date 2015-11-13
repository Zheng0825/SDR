#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Fri Nov 13 16:51:19 2015
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
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import uhd
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
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
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=Freq,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate_rec / 25,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("addr=192.168.1.100", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(samp_rate_rec)
        self.uhd_usrp_source_0.set_center_freq(Freq, 0)
        self.uhd_usrp_source_0.set_gain(20, 0)
        self.uhd_usrp_source_0.set_antenna("RX2", 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("addr=192.168.1.100", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate_send)
        self.uhd_usrp_sink_0.set_center_freq(Freq, 0)
        self.uhd_usrp_sink_0.set_normalized_gain(0.5, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.rational_resampler_xxx_0_0 = filter.rational_resampler_fff(
                interpolation=48,
                decimation=250,
                taps=None,
                fractional_bw=None,
        )
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=int(samp_rate_send * 1.0),
                decimation=audio_rate * audio_interp,
                taps=None,
                fractional_bw=None,
        )
        self.low_pass_filter_0 = filter.fir_filter_ccf(20, firdes.low_pass(
        	1, samp_rate_rec, 100e3, 10e3, firdes.WIN_HAMMING, 6.76))
        self.blocks_wavfile_source_0 = blocks.wavfile_source("/home/john/Downloads/documents-export-2015-09-15/Inspired But Too Tired Acoustic.wav", True)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((25, ))
        self.audio_sink_0 = audio.sink(48000, "", True)
        self.analog_wfm_tx_0 = analog.wfm_tx(
        	audio_rate=audio_rate,
        	quad_rate=audio_rate * audio_interp,
        	tau=75e-6,
        	max_dev=5e3,
        )
        self.analog_wfm_rcv_0 = analog.wfm_rcv(
        	quad_rate=250e3,
        	audio_decimation=1,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_wfm_rcv_0, 0), (self.rational_resampler_xxx_0_0, 0))    
        self.connect((self.analog_wfm_tx_0, 0), (self.rational_resampler_xxx_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.analog_wfm_tx_0, 0))    
        self.connect((self.blocks_wavfile_source_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.low_pass_filter_0, 0), (self.analog_wfm_rcv_0, 0))    
        self.connect((self.low_pass_filter_0, 0), (self.wxgui_fftsink2_0, 0))    
        self.connect((self.rational_resampler_xxx_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.audio_sink_0, 0))    
        self.connect((self.uhd_usrp_source_0, 0), (self.low_pass_filter_0, 0))    


    def get_samp_rate_send(self):
        return self.samp_rate_send

    def set_samp_rate_send(self, samp_rate_send):
        self.samp_rate_send = samp_rate_send
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate_send)

    def get_samp_rate_rec(self):
        return self.samp_rate_rec

    def set_samp_rate_rec(self, samp_rate_rec):
        self.samp_rate_rec = samp_rate_rec
        self.low_pass_filter_0.set_taps(firdes.low_pass(1, self.samp_rate_rec, 100e3, 10e3, firdes.WIN_HAMMING, 6.76))
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate_rec)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate_rec / 25)

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
        self.wxgui_fftsink2_0.set_baseband_freq(self.Freq)


if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = top_block()
    tb.Start(True)
    tb.Wait()
