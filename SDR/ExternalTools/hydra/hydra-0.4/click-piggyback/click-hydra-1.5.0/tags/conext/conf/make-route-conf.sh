#!/bin/sh

./dsdv-config.pl --userlevel --interface eth0 --address 192.168.100.13 > dsdv-13.click
./dsr-config.pl --userlevel --interface eth0 --address 192.168.100.13 > dsr-13.click
