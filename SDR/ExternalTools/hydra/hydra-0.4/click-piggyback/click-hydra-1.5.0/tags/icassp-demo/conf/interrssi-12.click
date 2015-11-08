require(hydra);

AddressInfo(me 192.168.155.12 FF:0D:56:BE:19:12);
AddressInfo(mac_ip 0.0.0.0, phy_ip 0.0.0.0);
AddressInfo(broadcast 255.255.255.255 FF:FF:FF:FF:FF:FF);

// For static routing
AddressInfo(plan12 192.168.155.12 FF:0D:56:BE:19:12);
AddressInfo(plan13 192.168.155.13 FF:0D:56:BE:19:13);
AddressInfo(plan14 192.168.155.14 FF:0D:56:BE:19:14);

elementclass MPI {
  // declare
  to_socket::HydraToSocket(UDP, phy_ip, 51111, FRAME 0, SNAPLEN 4096);
  from_socket::HydraFromSocket(UDP, mac_ip, 51112, FRAME 0, SNAPLEN 4096);
  gr_phy::GrPhy(VERBOSE 0);

  // composing
  input [0] -> [0] gr_phy [0] -> to_socket;
  from_socket -> SetTimestamp -> [1] gr_phy [1] -> [0] output;
};


elementclass FromHydraMAC {
  // from phy
  //rxvector::RXVector(VERBOSE 0);
  rxvector::RXVectorGr(VERBOSE 0);

  // filters
  mpi_filter::PaintSwitch;
  frame_filter::Classifier(0/1c%3f, 0/1d%3f, 0/1b%3f, 0/20%3f, - );
  // extra change needs
  //address_filter::Classifier(4/FFFFFFFFFFFF, 4/FF0D56BE1912, - );
  address_filter::Classifier(6/FFFFFFFFFFFF, 6/FF0D56BE1912, - );
  unicast_filter::Classifier(0/20%3f, - );

  frame_tee::Tee;
  unicast_tee::Tee;

  // carriersense
  rx_state::RXState(VERBOSE 0);
  nav::VirtualCarrierSense(VERBOSE 0);
  rx_cs::CarrierSense(-100.0, RXSTATE rx_state, VERBOSE 0);

  // decap
  data_frame::MACFrameData(me:eth, MODE 0, VERBOSE 0);
  decap::HydraDecap(data_frame, VERBOSE 0);

  input[0] -> rxvector -> mpi_filter;

  mpi_filter [0] -> Print("R CS BUSY") -> [0] rx_cs -> [2] output;  // HYDRA_CCA_BUSY
  mpi_filter [1] -> Print("R CS IDLE") -> [0] rx_cs;  //  HYDRA_CCA_IDLE
  mpi_filter [2] -> Discard;     //  HYDRA_CCA_EIDLE
  mpi_filter [3] -> Discard;     //  HYDRA_TX_DATA
  mpi_filter [4] -> CheckCRC32 -> frame_tee [0] -> rx_state;    

  frame_tee [1] -> frame_filter [4] -> Print("R Invalid frame") -> Discard;
  frame_filter [0] -> Print("R CTS") -> address_filter; 
  frame_filter [1] -> Print("R ACK") -> address_filter; 
  frame_filter [2] -> Print("R RTS") -> address_filter; 
  frame_filter [3] -> Print("R DATA") -> address_filter;

  address_filter [0] -> Print("R BROADCAST") -> decap -> [0] output;
  address_filter [1] -> Print("R for me") -> unicast_filter;
  address_filter [2] -> Print("R for other") -> nav[0] -> Print("V CS") -> [1] rx_cs;

  nav [1] -> Discard;

  unicast_filter [0] -> Print("R UNICAST") -> unicast_tee;
  unicast_filter [1] -> [1] output ;

  unicast_tee [0] -> decap;
  unicast_tee [1] -> [1] output; 

};

// ToMAC
elementclass ToHydraMAC { $mpi |
  // frame formats
  rts_frame::MACFrameRTS(me:eth, VERBOSE 0);
  cts_frame::MACFrameCTSRSSI(me:eth, VERBOSE 0);
  data_frame::MACFrameData(me:eth, MODE 0, VERBOSE 0);
  ack_frame::MACFrameACK(me:eth, VERBOSE 0);

  // rates
  cts_filter::Classifier(0/1c%3f, - );
  rc_snr::RateControlRSSI(cts_frame, 6, 10, VERBOSE 9);
  data_rate::RateControlConst(54, VERBOSE 0);
  control_rate::RateControlConst(6, VERBOSE 0);

  // to phy
  //txv::TXVector(VERBOSE 0);
  txv::TXVectorGr(VERBOSE 0);

  // transmitter
  transmitter::DCFTransmitter(NOTIFIER mpi/gr_phy, SLOTTIME difs, VERBOSE 0);

  // tx scheduler
  cp::BEBPolicy(MAX_RETRIES 6, MIN_CW 8, VERBOSE 0);
  // for simulation
  sifs::SetDefer(0.003, 0.0000, VERBOSE 0);
  difs::SetDefer(0.050, 0.050, VERBOSE 0);
  phy_time::PHYProcessTime1(0.050, 0.100, 0.005, 10.0, 0.0 0.0, 0.0 0.0, VERBOSE 0);

  // for real
  //sifs::SetDefer(0.002, 0.0000, VERBOSE 0);
  //difs::SetDefer(0.025, 0.020, VERBOSE 0);
  //phy_time::PHYProcessTime1(0.020, 0.060, 0.002, 10.0, 0.0 0.0, 0.0 0.0, VERBOSE 0);

  set_nav::SetDuration1(SIFS sifs, PHYPROCESSTIME phy_time, VERBOSE 0);
  txscheduler::DCFScheduler1(cp, rts_frame, control_rate, rc_snr, set_nav, NOTIFIER mpi/gr_phy, VERBOSE 0);

  // data reception
  encap::HydraEncap(data_frame, VERBOSE 0);
  set_rts::SetRTSThreshold(0, VERBOSE 0);
  //set_rts::SetRTSThreshold(4096, VERBOSE 0);
  set_datarate::SetRateControl(data_rate, VERBOSE 0);
  set_controlrate::SetRateControl(control_rate, VERBOSE 0);

  // response reception (RTS, DATA)
  response_classifier::Classifier(0/1b%3f, 0/20%3f, - );
  tx_cs::CarrierSense(-100.0, VERBOSE 0);
  cs_filter::PaintSwitch;

  input [0] -> encap -> set_rts -> set_datarate ->  Print("GOT IP DATA", NBYTES 20, PRINTANNO false) -> [0] txscheduler;
  input [1] -> cts_filter [0] -> rc_snr -> [1] txscheduler;
  cts_filter [1] -> [1] txscheduler;
  input [2] -> [0] tx_cs -> cs_filter;

  txscheduler [0] -> Print("S TRIGGER", NBYTES 20, PRINTANNO true) -> cp -> difs -> Queue(1) -> [0] transmitter;
  txscheduler [1] -> Print("S DATA") -> sifs -> [1] transmitter;
  txscheduler [2] -> response_classifier;
  txscheduler [3] -> [1] tx_cs;

  cs_filter [0] -> Print("P CS BUSY") -> [2] transmitter // HYDRA_CCA_BUSY
  cs_filter [1] -> Print("P CS IDLE") -> [2] transmitter // HYDRA_CCA_IDLE

  response_classifier [0] -> cts_frame -> Print("S CTS") -> set_controlrate -> set_nav -> sifs;
  response_classifier [1] -> ack_frame -> Print("S ACK") -> set_controlrate;
  response_classifier [2] -> Discard;
   
  transmitter -> Print("S TR", NBYTES 20) -> SetCRC32 -> txv -> Print("S TXVECTOR", NBYTES 20, PRINTANNO false) -> [0] output; 

}; 

elementclass GridForwardChecker {
  // expects grid packets with MAC headers 
  //
  // push -> push 
  // 
  // output [0] passes the forwarding packets through the Grid MAC packets 
  // output [1] produces ICMP error packets to be passed back to me 
  // $me | 
  input -> cl :: Classifier(19/03, -);
  cl [0] -> MarkIPHeader(82) -> cl2 :: IPClassifier(src host != me, -);  //IP data
  cl [1] -> [0] output;              // don't try to dec ttl for non-IP packets...

  cl2 [0]-> dec :: DecIPTTL;         // only decrement ttl for packets we don't originate
  cl2 [1] -> [0] output;			 // do not decrease ttl of packet generated by me

  dec [0] -> [0] output;			 // forward packets
  dec [1] -> ICMPError(me, 11, 0) -> icmp_demux :: IPClassifier(dst host me, dst net me/24); 
  icmp_demux [0] -> [1] output;      // ICMP for me
  icmp_demux [1] -> GridEncap(me:eth, me:ip) -> [0] output;  // forward ICMP to net
};

elementclass GridTXChecker {
  // expects grid packets with MAC headers 
  //
  // push -> push 
  // 
  // output [0] passes the packets through the Grid MAC packets 
 
  input -> cl :: Classifier(19/03, -);
  cl [0] -> MarkIPHeader(82) -> output 
  cl [1] -> output;              
};

elementclass ToGridDev {
  // push -> pull
  input -> cl :: Classifier(19/02, 19/03, -);  // Grid(12/7fff): DSDV(19/02) & Data(19/03)
  prio :: PrioSched;
  cl [0] -> route_counter :: Counter -> route_q :: Queue(100) -> SetGridChecksum -> [1] prio;
  cl [1] -> data_counter :: Counter ->  data_q :: Queue(100)  
    -> data_counter_out :: Counter
    //-> tr :: TimeRange
    -> lr :: LookupLocalGridRoute2(me:eth, me:ip, nb, VERBOSE true) 
    -> SetGridChecksum
    -> data_counter_out2 :: Counter
    //-> tr2 :: TimeRange
    -> [0] prio;
  cl [2] -> Print("Probe packet") -> Discard; 

  prio -> dev_counter :: Counter -> t :: PullTee;
 
  t [0]  -> output; 
  t [1] -> SetTimestamp -> Discard;
};


elementclass FromGridDev {
  //  push -> push
  // `Grid' packets on first output

  input -> t :: Tee;
  t [0] -> HostEtherFilter(me:eth, DROP_OWN true) -> cl :: Classifier(12/7fff, -);
  t [1] -> Discard;

  cl [0] -> ck :: CheckGridHeader;            // `Grid' packets (Routing and Data)
  cl [1] -> [1] output;						  //  Everything else

  ck [0] -> [0] output;              		  // good 
  ck [1] -> Print('Bad Grid header received', TIMESTAMP true, NBYTES 166) -> Discard;
};

// Initialize Elements
metric :: HopcountMetric();

// MTU of 802.11 = 2312, so it should be less than 2312 -  8 (llc) = 2304
//nb :: DSDVRouteTable(3600000, 15000, 7500, 000, 
//nb :: DSDVRouteTable(3600000, 150000, 75000, 10000, 
//		 me:eth, me:ip, 
//		 MAX_HOPS 100, 
//		 METRIC metric,
//		 MTU 2300, 
//     		 VERBOSE true);

// destination next_hop_eth next_hop_ip hop_count
nb::StaticRouteTable(me:eth me:ip, 8, 
                     plan13:ip plan13:eth plan13:ip 1, 
                     plan14:ip plan13:eth plan13:ip 2);

grid_demux :: Classifier(19/03,    // encapsulated (data) packets
			 			 19/02);   // route advertisement packets


ip_demux :: IPClassifier(dst host me,     // raw ip for me
			 			 dst net me/24);  // raw ip for network

grid_data_demux :: IPClassifier(dst host me,    // grid ip for me
								dst net me/24);	// grid ip for network

// initialize IO
//UDP + IP + MPI + 802.11 + ll + nbr + grid 
//8   + 20 + 5   + 34     + 8  + 8   + 60     = 143
//tun :: KernelTun(me/24, HEADROOM 143, MTU 1357);
// MTU of 802.11 except header= 2312 
// MTU of tun dev = 2312 - 8 - 8 -60 = 2236 
//tun :: KernelTun(me/24, HEADROOM 143, MTU 2236, DEV_NAME "hydra1");
tun :: KernelTun(me/24, HEADROOM 143, MTU 1357, DEV_NAME "hydra1");
mpi::MPI;
from_mac :: FromHydraMAC;
to_mac :: ToHydraMAC(mpi);
from_grid :: FromGridDev; 
to_grid :: ToGridDev;
checker :: GridForwardChecker;

// MAC internal routine
from_mac [1] -> [1] to_mac;
from_mac [2] -> [2] to_mac;

// Sending routine
tun -> IPPrint("S IP:") -> ip_input :: CheckIPHeader(VERBOSE true) -> GetIPAddress(16) -> ip_demux;
ip_demux [0] -> IPPrint("R IP:") -> tun;  						  // loopback packet sent by us (only BSD)
ip_demux [1] -> GridEncap(me:eth, me:ip) -> checker; // forward packet sent by us

checker [0] -> to_grid;		// All data for others in network
checker [1] -> tun;             // ICMP error message for me

to_grid -> [0] to_mac -> [0] mpi;

// Receiving routine
mpi [0] -> from_mac [0] -> Queue(100) -> Unqueue -> from_grid;

from_grid [0] -> Paint(0) -> grid_demux;
from_grid [1] -> Discard;

grid_demux [0] -> CheckIPHeader( , 82) -> grid_data_demux;  // Data packet
grid_demux [1] -> nb -> to_grid;								// Routing Packet

grid_data_demux [0] -> Strip(82) -> IPPrint("R IP:") -> tun;  // receive packet from net for us  
grid_data_demux [1] -> checker;  // forward packet from net 




elementclass GridLoad {
  // push, no input 

  // DATASIZE should be the size of the desired UDP packet (including
  // ethernet, Grid, and IP headers), plus 2 for alignment.  It must
  // be at least 120.  Most of this is stripped off to be re-used
  // later, avoiding expensive pushes in the UDP/IP and Grid
  // encapsulation.
  src :: InfiniteSource(ACTIVE false, DATASIZE 120)
    -> Strip(112) // 14 + 60 + 8 + 20 + 8 + 2 = 112 
                  // (eth + grid + grid_encap + ip + udp + 2 for alignment)
    -> seq :: IncrementSeqNo(FIRST 0, OFFSET 0)
    -> SetIPAddress(me)
    -> StoreIPAddress(4)
    -> udp :: UDPIPEncap(me, 1111, 0.0.0.0, 8021)
    -> count :: Counter
    //-> tr :: TimeRange
    -> output;

  ph :: PokeHandlers;
}
// not needed in userlevel
arp_demux :: Classifier(12/0806 20/0001, // arp queries
			12/0800);        // IP packets
Idle -> arp_demux [0] -> Idle;		// arp
arp_demux [1] -> Idle;			// ip

// UDP packet generator
// load :: GridLoad -> ip_input;
// ControlSocket(tcp, 7777);
