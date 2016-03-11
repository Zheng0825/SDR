#!/bin/sh

mkdir NoBatman

cd NoBatman

mkdir 2400-2500

cd 2400-2500

mkdir Standard

cd Standard

echo "No Batman TTL 128 Packet RX/TX 2.4/2.5 G Gain 60 " > NoBatTTL128Test1000_1.txt && ping -c 1000 192.168.200.109 >> NoBat1000_1.txt

espeak "First Packet Test Complete"

echo "No Batman TTL 128 Packet RX/TX 2.4/2.5 G Gain 60 " > NoBatTTL128Test1000_2.txt && ping -c 1000 192.168.200.109 >> NoBat1000_2.txt

espeak "Second Packet Test Complete"

echo "No Batman TTL 128 Packet RX/TX 2.4/2.5 G Gain 60 " > NoBatTTL128Test1000_3.txt && ping -c 1000 192.168.200.109 >> NoBat1000_3.txt

espeak "Third Packet Test Complete"
espeak "Changing Tests"





cd ..

mkdir TTLChange

cd TTLChange

echo "No Batman TTL 128 Packet RX/TX 2.4/2.5 G Gain 60 " > NoBatTTL128Test1000.txt && ping -c 1000 -t 128 192.168.200.109 >> NoBatTTL128Test1000.txt

espeak "TTL 128 Complete"

echo "No Batman TTL 255 Packet RX/TX 2.4/2.5 G Gain 60 " > NoBatTTL255SizeTest1000.txt && ping -c 1000 -t 255 192.168.200.109 >> NoBatTTL255Test1000.txt

espeak "TTYL 255 Packet Complete"

espeak "Changing Tests"




cd ..

mkdir PacketSizeChange

cd PacketSizeChange

echo "No Batman 32byte Packet RX/TX 2.4/2.5 G Gain 60 " > NoBat32bytePacketSizeTest1000.txt && ping -c 1000 -s 24 192.168.200.109 >> NoBat32bytePacketSizeTest1000.txt

espeak "32 Byte Packet Test Complete"

echo "No Batman 16byte Packet RX/TX 2.4/2.5 G Gain 60 " > NoBat16bytePacketSizeTest1000.txt && ping -c 1000 -s 16 192.168.200.109 >> NoBat16bytePacketSizeTest1000.txt

espeak " 16 bye packet complete" 

espeak "All Current tests complete, please change frequency and run again"

cd ..
