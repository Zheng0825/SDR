// 
// Copyright (C) 2009  The University of Texas at Austin.
// 
// This file is part of Hydra: A wireless multihop testbed.
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, subject to the conditions
// listed in the Click LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the Click LICENSE file; the license in that file is
// legally binding.
// 
require(hydra);

// channel port number = phy number
// phy port number = 0:related mac 1:related channel

// tx_range = 200m, cs_range = 400m
//channel :: SimChannel(0 0, 0 150, 0 300);
channel :: SimChannel(0 0, 0 100);
// phy time
// SimGrPhy only use tx encoding and rx deconding time
// slope and interception
phy_time::PHYProcessTime1(0.01, 0.01, 0.001, 10.0, 0.0 0.0, 0.0 0.0, VERBOSE 0);

// phy 0
//phy0 :: SimGrPhyNew(phy_time, VERBOSE 0)
//from_mac0 :: HydraFromSocket(UDP, 0.0.0.0, 52121, FRAME 0, SNAPLEN 2500, VERBOSE true);
//to_mac0 :: HydraToSocket(UDP, 192.168.0.12, 52122, FRAME 0, SNAPLEN 2500, VERBOSE true);
//delay0 :: DelayUnqueue(0.001);
//delay0 :: Unqueue;

// phy 1
phy1 :: SimGrPhyNew(phy_time, VERBOSE 0);
from_mac1 :: HydraFromSocket(UDP, 0.0.0.0, 52131, FRAME 0, SNAPLEN 2500, VERBOSE true);
to_mac1 :: HydraToSocket(UDP, 192.168.0.13, 52132, FRAME 0, SNAPLEN 2500, VERBOSE true);
//delay1 :: DelayUnqueue(0.001);
//delay1 :: Unqueue;

// phy 2
phy2 :: SimGrPhyNew(phy_time, VERBOSE 0);
from_mac2 :: HydraFromSocket(UDP, 0.0.0.0, 52141, FRAME 0, SNAPLEN 2500, VERBOSE true);
to_mac2 :: HydraToSocket(UDP, 192.168.0.14, 52142, FRAME 0, SNAPLEN 2500, VERBOSE true);
//delay2 :: DelayUnqueue(0.001);
//delay2 :: Unqueue;

// Connection Configuration

// phy 0
//from_mac0 -> [0] phy0; 
//channel [0] -> [1] phy0;
//phy0 [0] -> Queue(100) -> to_mac0;
//phy0 [1] -> [0] channel;

// phy 1
from_mac1 -> [0] phy1; 
channel [0] -> [1] phy1;
phy1 [0] -> Queue(100) -> to_mac1;
phy1 [1] -> [0] channel;

// phy 2
from_mac2 -> [0] phy2; 
channel [1] -> [1] phy2;
phy2 [0] ->  Queue(100) -> to_mac2;
phy2 [1] -> [1] channel;
