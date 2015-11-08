#!/bin/sh

#This is reference setting. Please copy this file and set your configurations.

#File name of .ccb file
FILENAME=frame-agg-emul

#Subnet of node
#SUBNET=192.168.255.0
SUBNET=192.168.100.0

#The last digit of IP address of this node 
#NODE=12
#NODE=13
NODE=14

#The port number that is used for MPI communication.
#Set the port that is used for sending packet from MAC to PHY,
# then port for receiving will be set by increasing this port by one
#PORT=52121
#PORT=52131
PORT=52141

#Static routing table composed of dest_node:next_hop_node:hop_count
#Each route entry should be seperated by comma
#ROUTES="13:13:1,14:13:2"
#ROUTES="12:12:1,14:14:1"
ROUTES="13:13:1,12:13:2"

./make-hydra-conf.py ${FILENAME}.ccb --subnet=${SUBNET} --node=${NODE} --mpi-port=${PORT} --routes=${ROUTES} > ${FILENAME}-${NODE}.click