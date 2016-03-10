#!/bin/sh

mkdir TTL128

cd TTL128

echo "Four Hops TTL 128 RX/TX 915/928 AVG Gain 47 " > 4HopTTL128Test1000.txt && ping -c 1000 -t 128 192.168.200.109 >> 4HopTTL128Test1000.txt

echo "Three Hops TTL 128 Packet RX/TX 915/928 AVG Gain 47 " > 3HopTTL128Test1000.txt && ping -c 1000 -t 128 192.168.200.102 >> 3HopTTL128Test1000.txt

echo "Two Hops TTL 128 Packet RX/TX 915/928 AVG Gain 47 " > 2HopTTL128Test1000.txt && ping -c 1000 -t 128 192.168.200.101 >> 2HopTTL128Test1000.txt

echo "One Hops TTL 128 Packet RX/TX 915/928 AVG Gain 47 " > 1HopTTL128Test1000.txt && ping -c 1000 -t 128 192.168.200.106 >> 1HopTTL128Test1000.txt

cd ..

mkdir TTL255

cd TTL255

echo "Four Hops TTL 255 RX/TX 915/928 AVG Gain 47 " > 4HopTTL255Test1000.txt && ping -c 1000 -t 255 192.168.200.109 >> 4HopTTL255Test1000.txt

echo "Three Hops TTL 255 RX/TX 915/928 AVG Gain 47 " > 3HopTTL255Test1000.txt && ping -c 1000 -t 255 192.168.200.102 >> 3HopTTL255Test1000.txt

echo "Two Hops TTL 255 Packet RX/TX 915/928 AVG Gain 47 " > 2HopTTL255Test1000.txt && ping -c 1000 -t 255 192.168.200.101 >> 2HopTTL255Test1000.txt

echo "One Hops TTL 255 Packet RX/TX 915/928 AVG Gain 47 " > 1HopTTL255SizeTest1000.txt && ping -c 1000 -t 255 192.168.200.106 >> 1HopTTL255Test1000.txt

cd ..