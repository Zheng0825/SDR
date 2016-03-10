#!/bin/sh

#echo "Four Hops Alternating RX/TX 915/928 AVG Gain 47 " > 4HopAltFreqTest.txt && ping -c 1000 192.168.200.109 >> 4HopAltFreqTest.txt

echo "Three Hops Alternating RX/TX 915/928 AVG Gain 47 " > 3HopAltFreqTest.txt && ping -c 5 192.168.200.102 >> 3HopAltFreqTest.txt

echo "Two Hops Alternating RX/TX 915/928 AVG Gain 47 " > 2HopAltFreqTest.txt && ping -c 5 192.168.200.101 >> 2HopAltFreqTest.txt

echo "One Hops Alternating RX/TX 915/928 AVG Gain 47 " > 1HopAltFreqTest.txt && ping -c 5 192.168.200.106 >> 1HopAltFreqTest.txt