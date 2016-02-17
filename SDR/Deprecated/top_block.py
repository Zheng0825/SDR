#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Wed Jan 27 16:41:40 2016
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

from gnuradio import audio
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.wxgui import forms
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import wx


class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 44100
        self.Switch = Switch = 0

        ##################################################
        # Blocks
        ##################################################
        self._Switch_chooser = forms.drop_down(
        	parent=self.GetWin(),
        	value=self.Switch,
        	callback=self.set_Switch,
        	label='Switch',
        	choices=[0,1],
        	labels=[],
        )
        self.Add(self._Switch_chooser)
        self.blocks_wavfile_source_0_0 = blocks.wavfile_source("/home/john/IRG/Music/John Prine - The Sins of Memphisto.wav", True)
        self.blocks_wavfile_source_0 = blocks.wavfile_source("/home/john/IRG/Music/Inspired But Too Tired Acoustic.wav", True)
        self.blks2_selector_0 = grc_blks2.selector(
        	item_size=gr.sizeof_float*1,
        	num_inputs=2,
        	num_outputs=1,
        	input_index=Switch,
        	output_index=0,
        )
        self.audio_sink_0 = audio.sink(samp_rate, "", True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blks2_selector_0, 0), (self.audio_sink_0, 0))    
        self.connect((self.blocks_wavfile_source_0, 0), (self.blks2_selector_0, 0))    
        self.connect((self.blocks_wavfile_source_0_0, 0), (self.blks2_selector_0, 1))    


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_Switch(self):
        return self.Switch

    def set_Switch(self, Switch):
        self.Switch = Switch
        self._Switch_chooser.set_value(self.Switch)
        self.blks2_selector_0.set_input_index(int(self.Switch))


if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = top_block()
    tb.Start(True)
    tb.Wait()
