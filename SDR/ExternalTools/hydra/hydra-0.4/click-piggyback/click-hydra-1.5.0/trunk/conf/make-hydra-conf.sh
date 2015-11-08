#!/bin/sh
# 
# Copyright (C) 2009  The University of Texas at Austin.
# 
# This file is part of Hydra: A wireless multihop testbed.
# 
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, subject to the conditions
# listed in the Click LICENSE file. These conditions include: you must
# preserve this copyright notice, and you cannot mention the copyright
# holders in advertising related to the Software without their permission.
# The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
# notice is a summary of the Click LICENSE file; the license in that file is
# legally binding.
# 

#This is reference setting. Please copy this file and set your configurations.

#File name of .ccb file
FILENAME=frame-agg

#Subnet of node
SUBNET=192.168.255.0

#The last digit of IP address of this node 
NODE=14

#The port number that is used for MPI communication.
#Set the port that is used for sending packet from MAC to PHY,
# then port for receiving will be set by increasing this port by one
PORT=60000

#Static routing table composed of dest_node:next_hop_node:hop_count
#Each route entry should be seperated by comma
#ROUTES="13:13:1,14:13:2"
#ROUTES="12:12:1,14:14:1"
ROUTES="12:13:2,13:13:1"

./make-hydra-conf.py ${FILENAME}.ccb --subnet=${SUBNET} --node=${NODE} --mpi-port=${PORT} --routes=${ROUTES} > ${FILENAME}-${NODE}.click
