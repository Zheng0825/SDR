# Tools Folder

## Alfred-Ubuntu

This may get removed, currently its empty but I would like to include everything related to alfred here.

## gr-ieee802-11

This is an Out of Tree (OOT) module from GNU Radio. OOT just means that its not supported within the main confines of GNU Radio, but that the community generally suports it.

Specifically it lets you work with Wifi, although it mostly only works with 802.11p which is not a current
standard used by routers and is instead what is going to be the standard for car to car communication. 


## gr-foo

This is another OOT module. This module
was designed by a few different gnu-radio community 
members to give basic features that most OOT
modules rely on. The innerworkings are not really 
important
but if you are using an OOT module, expect that it
will require these features. 

## UDP

These are two small scripts I set up in python, useful for testing the network by sending and receiving unique data through UDP. Not super important, but may help.

## raiseBatSignal.sh

This is probably what you came here for. This sets up
batman-adv on the tun0 port. If you need to use this on
a different port, or even not on the SDR, just change
the words tun0 to the name of your port (ex. eth0) on all lines. This can be run with "sh raiseBatSignal.sh" and should need no input from the user. 

To confirm it is working run "ifconfig bat0". 

## tmux_start

This configures tmux to the proper settings for our test environment. It is really only useful on two of the machines in the lab, but I will hopefully
have time to expand this in the future. 


