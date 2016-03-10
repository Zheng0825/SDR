#!/bin/sh

mkdir PacketSize32Byte

cd PacketSize32Byte

echo "Four Hops 32byte Packet RX/TX 915/928 AVG Gain 47 " > 4Hop32bytePacketSizeTest1000.txt && ping -c 1000 -s 24 192.168.200.109 >> 4Hop32bytePacketSizeTest1000.txt

echo "Three Hops 32byte Packet RX/TX 915/928 AVG Gain 47 " > 3Hop32bytePacketSizeTest1000.txt && ping -c 1000 -s 24 192.168.200.102 >> 3Hop32bytePacketSizeTest1000.txt

echo "Two Hops 32byte Packet RX/TX 915/928 AVG Gain 47 " > 2Hop32bytePacketSizeTest1000.txt && ping -c 1000 -s 24 192.168.200.101 >> 2Hop32bytePacketSizeTest1000.txt

echo "One Hops 32byte Packet RX/TX 915/928 AVG Gain 47 " > 1Hop32bytePacketSizeTest1000.txt && ping -c 1000 -s 24 192.168.200.106 >> 1Hop32bytePacketSizeTest1000.txt

cd ..

mkdir PacketSize16Byte

cd PacketSize16Byte

echo "Four Hops 16byte Packet RX/TX 915/928 AVG Gain 47 " > 4Hop16bytePacketSizeTest1000.txt && ping -c 1000 -s 16 192.168.200.109 >> 4Hop16bytePacketSizeTest1000.txt

echo "Three Hops 16byte Packet RX/TX 915/928 AVG Gain 47 " > 3Hop16bytePacketSizeTest1000.txt && ping -c 1000 -s 16 192.168.200.102 >> 3Hop16bytePacketSizeTest1000.txt

echo "Two Hops 16byte Packet RX/TX 915/928 AVG Gain 47 " > 2Hop16bytePacketSizeTest1000.txt && ping -c 1000 -s 16 192.168.200.101 >> 2Hop16bytePacketSizeTest1000.txt

echo "One Hops 16byte Packet RX/TX 915/928 AVG Gain 47 " > 1Hop16bytePacketSizeTest1000.txt && ping -c 1000 -s 16 192.168.200.106 >> 1Hop16bytePacketSizeTest1000.txt

cd ..

