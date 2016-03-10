#!/bin/sh

mkdir 4HopTestBaseline

cd 4HopTestBaseline

mkdir Test1

cd Test1

echo "Four Hops Alternating RX/TX 915/928 AVG Gain 47 " > 4HopAltFreqTest.txt && ping -c 1000 192.168.200.109 >> 4HopAltFreqTest.txt

echo "Three Hops Alternating RX/TX 915/928 AVG Gain 47 " > 3HopAltFreqTest1000.txt && ping -c 1000 192.168.200.102 >> 3HopAltFreqTest1000.txt

echo "Two Hops Alternating RX/TX 915/928 AVG Gain 47 " > 2HopAltFreqTest1000.txt && ping -c 1000 192.168.200.101 >> 2HopAltFreqTest1000.txt

echo "One Hops Alternating RX/TX 915/928 AVG Gain 47 " > 1HopAltFreqTest1000.txt && ping -c 1000 192.168.200.106 >> 1HopAltFreqTest1000.txt

cd ..

mkdir Test2

cd Test2

echo "Four Hops Alternating RX/TX 915/928 AVG Gain 47 " > 4HopAltFreqTest.txt && ping -c 1000 192.168.200.109 >> 4HopAltFreqTest.txt

echo "Three Hops Alternating RX/TX 915/928 AVG Gain 47 " > 3HopAltFreqTest1000.txt && ping -c 1000 192.168.200.102 >> 3HopAltFreqTest1000.txt

echo "Two Hops Alternating RX/TX 915/928 AVG Gain 47 " > 2HopAltFreqTest1000.txt && ping -c 1000 192.168.200.101 >> 2HopAltFreqTest1000.txt

echo "One Hops Alternating RX/TX 915/928 AVG Gain 47 " > 1HopAltFreqTest1000.txt && ping -c 1000 192.168.200.106 >> 1HopAltFreqTest1000.txt

cd ..

mkdir Test3

cd Test3

echo "Four Hops Alternating RX/TX 915/928 AVG Gain 47 " > 4HopAltFreqTest.txt && ping -c 1000 192.168.200.109 >> 4HopAltFreqTest.txt

echo "Three Hops Alternating RX/TX 915/928 AVG Gain 47 " > 3HopAltFreqTest1000.txt && ping -c 1000 192.168.200.102 >> 3HopAltFreqTest1000.txt

echo "Two Hops Alternating RX/TX 915/928 AVG Gain 47 " > 2HopAltFreqTest1000.txt && ping -c 1000 192.168.200.101 >> 2HopAltFreqTest1000.txt

echo "One Hops Alternating RX/TX 915/928 AVG Gain 47 " > 1HopAltFreqTest1000.txt && ping -c 1000 192.168.200.106 >> 1HopAltFreqTest1000.txt

cd ../../

#echo "Four Hops Alternating RX/TX 915/928 AVG Gain 47 " > 4HopAltFreqTest10k.txt && ping -c 10000 192.168.200.109 >> 4HopAltFreqTest10k.txt

#echo "Three Hops Alternating RX/TX 915/928 AVG Gain 47 " > 3HopAltFreqTest10k.txt && ping -c 10000 192.168.200.102 >> 3HopAltFreqTest10k.txt

#echo "Two Hops Alternating RX/TX 915/928 AVG Gain 47 " > 2HopAltFreqTest10k.txt && ping -c 10000 192.168.200.101 >> 2HopAltFreqTest10k.txt

#echo "One Hops Alternating RX/TX 915/928 AVG Gain 47 " > 1HopAltFreqTest10k.txt && ping -c 10000 192.168.200.106 >> 1HopAltFreqTest10k.txt
