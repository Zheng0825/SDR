#!/usr/bin/env python2
##################################################
# ''GNU Radio Python Flow Graph
# Title: Nonbroadcastwithfreqandmac
# Generated: Mon Feb  1 16:46:29 2016
#
# The name of this file may be misleading if
# threading is not used in the end. I just needed
# to change the title to ensure that it would
# not be overwritten when I went to build it again
# later. 
#
# This makes me realize I need to create a way
# to ensure that changes to the flow graph can
# be made without requiring massive code rewrites
# as we add features.
#
##################################################
#from __future__ import print_function

async_mode = None

if async_mode is None:
    try:
        import eventlet
        async_mode = 'eventlet'
    except ImportError:
        pass

    if async_mode is None:
        try:
            from gevent import monkey
            async_mode = 'gevent'
        except ImportError:
            pass

    if async_mode is None:
        async_mode = 'threading'

    print('async_mode is ' + async_mode)

# monkey patching is necessary because this application uses a background
# thread
if async_mode == 'eventlet':
    import eventlet
    eventlet.monkey_patch()
elif async_mode == 'gevent':
    from gevent import monkey
    monkey.patch_all()

from threading import Thread
from optparse import OptionParser
from gnuradio.eng_option import eng_option
from gnuradio import eng_notation

from broadcastFandMNoGui import broadcastFandMNoGui
import mac
import pmt
import wx
import sys

from flask import Flask, render_template, request, jsonify,session
from flask_socketio import SocketIO, emit

from frequencytable import FrequencyTable

import os
from time import sleep
import subprocess

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, async_mode=async_mode)
thread = None

@app.route('/')
def index():
    return render_template('radiostats.html', rxFreq=tb.get_rx_freq(), txFreq=tb.get_tx_freq(), txGain=tb.get_tx_gain(), rxGain=tb.get_rx_gain())


# http://www.secdev.org/projects/scapy/
# Saving that link for later

# TX Freq
@socketio.on('inc tx freq', namespace='/test')
def inc_freq():
    newFreq = txfreqtable.increase_freq()
    os.system("echo " + str(newFreq) + " | sudo alfred -s 65")    
    #emit('confirm tx freq', {'txfreq':newFreq})

@socketio.on('dec tx freq', namespace='/test')
def dec_freq():
    newFreq = txfreqtable.decrease_freq()
    os.system("echo " + str(newFreq) + " | sudo alfred -s 65")
    #emit('confirm tx freq', {'txfreq':newFreq})

@socketio.on('alfred set freq')
def set_freq(freq):
    print("PRINT")  
    tb.set_tx_freq(freq[0])
    emit('confirm tx freq', {'txfreq':tb.get_tx_freq()})	
 
#RX Freq
@socketio.on('inc rx freq', namespace='/test')
def inc_freq():
    tb.set_rx_freq(rxfreqtable.increase_freq())
    os.system("echo " + str(tb.get_rx_freq()) + " | sudo alfred -s 66")
    emit('confirm rx freq', {'rxfreq':tb.get_rx_freq()})

@socketio.on('dec rx freq', namespace='/test')
def dec_freq():
    tb.set_rx_freq(rxfreqtable.decrease_freq())
    os.system("echo " + str(tb.get_rx_freq()) + " | sudo alfred -s 66")
    emit('confirm rx freq', {'rxfreq':tb.get_rx_freq()})

#TX Gain
@socketio.on('inc tx gain', namespace='/test')
def inc_freq():
    tb.set_tx_gain(tb.get_tx_gain() + 1)
    emit('confirm tx gain', {'txgain':tb.get_tx_gain()})

@socketio.on('dec tx gain', namespace='/test')
def dec_freq():
    tb.set_tx_gain(tb.get_tx_gain() - 1)
    emit('confirm tx gain', {'txgain':tb.get_tx_gain()})

#RX Gain
@socketio.on('inc rx gain', namespace='/test')
def inc_freq():
    tb.set_rx_gain(tb.get_rx_gain() + 1)
    emit('confirm rx gain', {'rxgain':tb.get_rx_gain()})

@socketio.on('dec rx gain', namespace='/test')
def dec_gain():
    tb.set_rx_gain(tb.get_rx_gain() - 1)
    emit('confirm rx gain', {'rxgain':tb.get_rx_gain()})

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
        "-A", "--rx-antenna", dest="rx_antenna", type="string", default="TX/RX",
        help="Set RX antenna [default=%default]")
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
        "", "--tx-gain", dest="tx_gain", type="eng_float", default=eng_notation.num_to_str(45),
        help="Set TX gain [default=%default]")
    parser.add_option(
        "", "--tx-lo-offset", dest="tx_lo_offset", type="eng_float", default=eng_notation.num_to_str(0),
        help="Set TX LO offset [default=%default]")
    parser.add_option(
        "-l", "--radio-addr", dest="radio_addr", type="intx", default=63,
        help="Set Local address [default=%default]")
    parser.add_option(
        "", "--rx-freq", dest="rx_freq", type="eng_float", default=eng_notation.num_to_str(915e6),
        help="Set RX freq [default=%default]")
    parser.add_option(
        "", "--tx-freq", dest="tx_freq", type="eng_float", default=eng_notation.num_to_str(915e6),
        help="Set TX freq [default=%default]")
    parser.add_option(
        "-r", "--rate", dest="rate", type="eng_float", default=eng_notation.num_to_str(1e6),
        help="Set Sample rate [default=%default]")
    return parser

def main(top_block_cls=broadcastFandMNoGui, options=None):
    print("Initializing GNU Radio")
    if options is None:
        options, _ = argument_parser().parse_args()
    global tb
    tb = top_block_cls(ampl=options.ampl, args=options.args, arq_timeout=options.arq_timeout, dest_addr=options.dest_addr, iface=options.iface, max_arq_attempts=options.max_arq_attempts, mtu=options.mtu, port=options.port, rx_antenna=options.rx_antenna, rx_gain=options.rx_gain, rx_lo_offset=options.rx_lo_offset, samps_per_sym=options.samps_per_sym, tx_gain=options.tx_gain, tx_lo_offset=options.tx_lo_offset, radio_addr=options.radio_addr, rx_freq=options.rx_freq, tx_freq=options.tx_freq, rate=options.rate)
    print("Starting GNU Radio")
    tb.start()
    setupEverythingElse()
    try:
        raw_input('Press Enter to quit: ')
    except EOFError:
        pass
    tb.stop()
    tb.wait()

def setupEverythingElse():
    global txfreqtable
    txfreqtable = FrequencyTable()

    global rxfreqtable
    rxfreqtable = FrequencyTable()

    print("Setting IP Address")
    os.system('sudo ifconfig tun0 192.168.200.2')
    print("Ip Address of tun0 is now 192.168.200.2")

    #print("setting up bat0")
    #os.system('sudo sh static/shell/raiseBatSignal.sh')
    #print("setup bat0")
    '''
    print("Setting up Alfred")
    sleep(1)
    print("...")
    sleep(1)
    print("...")
    sleep(1)
    print("...")
    os.system('sudo alfred -i bat0 -m -c echo "Master Wayne says Hello"' &)
    '''
    print("Starting rest of the server")


if __name__ == '__main__':

    thread = Thread(target=main, args=())
    #thread.daemon = True
    thread.start()
    
    socketio.run(app,debug=True, use_reloader=False)





