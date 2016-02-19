# Flowgraphs Folder

## deprecated

Dont go in there. DO NOT GO IN THERE.

## webinterfaces

This is probably where you want to be, this has
all the files related to creating the web
interface and flask server. More details there

## BroadcastMAC.py

This will setup the bare minimum you need to have
a working instance of GNU Radio that will allow
for data transmission. 

This will broadcast at 915 MHz ONLY. The Gains
will be adjustable. 

Please Note, you will have to manually add
an IP address, and this will not setup batman-adv
or alfred. This is a good first step if you just want
to learn what is going on, or confirm that your
setup is correct. This also will not allow
for frequency hopping. 

If this is your first time running GNU Radio, it would
probably be better to start with broadcastwithFreqAndMac.grc

## broadcastwithFreqAndMac.grc

This is my advised place to start. It will allow you
to transmist and recieve similar to BroadcastMAC.py,
however it will be running inside GNU Radio-Companion
and not in a python instance where you cannot really
see what is going on. 

To start, run:

sudo gnuradio-companion broadcastwithFreqAndMac.grc

This will also let you change the MAC address of the
flowgraph. When operating with different radios, each
needs a unique MAC address or there will be collisions
and you will not get data to transmit well. 

When running this file, be aware that it will change
the python files in this folder, so if you are just
experimenting, please copy this file to a new
folder with your name on it, using the command:

mkdir <your_name>
cp broadcastwithFreqAndMac.grc <yourname>

## broadcastwithFreqAndMac.py and broadcastwithFreqAndMac2.py

These should be two python instances of the same flow
graph, but with different MAC address. If you are
working and see that the MAC addresses have become
the same, please either edit it or let John know. 

## simple_trx.py

This is left as a sanity check. It is the basic program
created by gr-mac, the file that these
flowgraphs are based off of. 

