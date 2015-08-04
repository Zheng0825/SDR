#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Narrow Fm
# Generated: Tue Aug  4 15:08:07 2015
##################################################

from gnuradio import analog
from gnuradio import audio
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import osmosdr
import wx

class narrow_fm(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Narrow Fm")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 2000000
        self.corr = corr = 0
        self.Freq = Freq = 107.9e6

        ##################################################
        # Blocks
        ##################################################
        _corr_sizer = wx.BoxSizer(wx.VERTICAL)
        self._corr_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_corr_sizer,
        	value=self.corr,
        	callback=self.set_corr,
        	label='corr',
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._corr_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_corr_sizer,
        	value=self.corr,
        	callback=self.set_corr,
        	minimum=-100,
        	maximum=100,
        	num_steps=200,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_corr_sizer)
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
        	sample_rate=samp_rate / 25,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.rational_resampler_xxx_0 = filter.rational_resampler_fff(
                interpolation=48,
                decimation=500,
                taps=None,
                fractional_bw=None,
        )
        self.osmosdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + "bladerf=0,xb200=auto,fpga=~/Downloads/hostedx40-latest.rbf" )
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(Freq, 0)
        self.osmosdr_source_0.set_freq_corr(corr, 0)
        self.osmosdr_source_0.set_dc_offset_mode(2, 0)
        self.osmosdr_source_0.set_iq_balance_mode(2, 0)
        self.osmosdr_source_0.set_gain_mode(1, 0)
        self.osmosdr_source_0.set_gain(25, 0)
        self.osmosdr_source_0.set_if_gain(25, 0)
        self.osmosdr_source_0.set_bb_gain(25, 0)
        self.osmosdr_source_0.set_antenna("", 0)
        self.osmosdr_source_0.set_bandwidth(0, 0)
          
        self.low_pass_filter_0 = filter.fir_filter_ccf(int(samp_rate/500e3), firdes.low_pass(
        	1, samp_rate, 100e3, 1e6, firdes.WIN_HAMMING, 6.76))
        self.audio_sink_0 = audio.sink(48000, "", True)
        self.analog_wfm_rcv_0 = analog.wfm_rcv(
        	quad_rate=500e3,
        	audio_decimation=1,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.low_pass_filter_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.low_pass_filter_0, 0), (self.analog_wfm_rcv_0, 0))
        self.connect((self.analog_wfm_rcv_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.audio_sink_0, 0))
        self.connect((self.osmosdr_source_0, 0), (self.low_pass_filter_0, 0))


# QT sink close method reimplementation

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.low_pass_filter_0.set_taps(firdes.low_pass(1, self.samp_rate, 100e3, 1e6, firdes.WIN_HAMMING, 6.76))
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate / 25)
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)

    def get_corr(self):
        return self.corr

    def set_corr(self, corr):
        self.corr = corr
        self._corr_slider.set_value(self.corr)
        self._corr_text_box.set_value(self.corr)
        self.osmosdr_source_0.set_freq_corr(self.corr, 0)

    def get_Freq(self):
        return self.Freq

    def set_Freq(self, Freq):
        self.Freq = Freq
        self.wxgui_fftsink2_0.set_baseband_freq(self.Freq)
        self._Freq_slider.set_value(self.Freq)
        self._Freq_text_box.set_value(self.Freq)
        self.osmosdr_source_0.set_center_freq(self.Freq, 0)

if __name__ == '__main__':
    import ctypes
    import os
    if os.name == 'posix':
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = narrow_fm()
    tb.Start(True)
    tb.Wait()

