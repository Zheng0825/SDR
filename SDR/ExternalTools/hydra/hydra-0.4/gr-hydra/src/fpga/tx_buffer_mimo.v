/* 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
module tx_buffer_mimo
  ( //System
    input wire [31:0] timestamp_clock, input wire usbclk, input wire bus_reset, input wire reset, input wire clock_reset,
    input wire [15:0] usbdata, output wire have_space, input wire [3:0] channels, 
    //output transmit signals
    output wire [15:0] tx_i_0, output wire [15:0] tx_q_0, 
    output wire [15:0] tx_i_1, output wire [15:0] tx_q_1,
    output wire [15:0] tx_i_2, output wire [15:0] tx_q_2, 
    output wire [15:0] tx_i_3, output wire [15:0] tx_q_3, 
    input wire txclk, input wire txstrobe, input wire WR,
    input wire clear_status, output wire tx_empty, output wire [15:0] debugbus, 
    //command reader io
    output wire [15:0] rx_databus, output wire rx_WR, output wire rx_WR_done, 
    input wire rx_WR_enabled,
    //register io 
    output wire [1:0] reg_io_enable, output wire [31:0] reg_data_in, output wire [6:0] reg_addr,
    input wire [31:0] reg_data_out,  
    //input characteristic signals
    input wire [31:0] rssi_0, input wire [31:0] rssi_1, input wire [31:0] rssi_2, 
    input wire [31:0] rssi_3, input wire [31:0] rssi_wait, input wire [31:0] threshhold, 
    output wire [1:0] tx_underrun, 
    //system stop
    output wire stop, output wire [15:0] stop_time);
	
   parameter NUM_CHAN	 =      1 ;
    
   /* These will eventually be external register */
   wire                 [7:0]  txstrobe_rate [NUM_CHAN-1:0] ;
   wire			        [31:0] rssi [3:0];
   assign rssi[0] = rssi_0;
   assign rssi[1] = rssi_1;
   assign rssi[2] = rssi_2;
   assign rssi[3] = rssi_3;
   
    /* Connections between tx_usb_fifo_reader and
       cnannel/command processing blocks */
   wire                  [31:0] tx_data_bus ;
   wire            [NUM_CHAN:0] chan_WR ;
   wire            [NUM_CHAN:0] chan_done ;
    
    /* Connections between data block and the
       FX2/TX chains */
   wire            [NUM_CHAN:0] chan_underrun;
   wire            [NUM_CHAN-1:0] chan_txempty;
   
   /* Conections between tx_data_packet_fifo and
       its reader + strobe generator */
   wire                 [31:0] chan_fifodata [NUM_CHAN:0] ;
   wire                        chan_pkt_waiting [NUM_CHAN:0] ;
   wire                        chan_rdreq [NUM_CHAN:0] ;
   wire                        chan_skip [NUM_CHAN:0] ;
   wire                        chan_have_space [NUM_CHAN:0] ;

   wire		            [14:0] debug [NUM_CHAN:0];
    
   /* Outputs to transmit chains */
   wire                 [15:0] tx_i [NUM_CHAN:0] ;
   wire                 [15:0] tx_q [NUM_CHAN:0] ;

   /*assign tx_i[NUM_CHAN] = 0;
   assign tx_q[NUM_CHAN] = 0;*/
    
   assign have_space = chan_have_space[0] & chan_have_space[1];
   assign tx_empty = chan_txempty;//[0];// & chan_txempty[1] ;

   assign tx_i_0 = chan_txempty ? 16'b0 : tx_i[0] ;
   assign tx_q_0 = chan_txempty ? 16'b0 : tx_q[0] ;
   assign tx_i_1 = chan_txempty ? 16'b0 : tx_i[1] ;
   assign tx_q_1 = chan_txempty ? 16'b0 : tx_q[1] ;
        
   assign tx_q_2 = 16'b0 ;
   assign tx_i_2 = 16'b0 ;
   assign tx_q_3 = 16'b0 ;
   assign tx_i_3 = 16'b0 ;
   assign tx_i_3 = 16'b0 ;
	
   wire		WR_final;
   assign debugbus = {have_space, txclk, WR, WR_final, chan_WR, chan_done, 
                      chan_pkt_waiting[0], chan_pkt_waiting[1],
                      chan_rdreq[0], chan_rdreq[1], chan_txempty, chan_txempty};

   wire [31:0] usbdata_final;

   tx_packer tx_usb_packer
   (.bus_reset(bus_reset), .usbclk(usbclk), .WR_fx2(WR),
    .usbdata(usbdata), .reset(reset), .txclk(txclk),
    .usbdata_final(usbdata_final), .WR_final(WR_final));
	
   channel_demux #(NUM_CHAN) channel_demuxer
   (.usbdata_final(usbdata_final), .WR_final(WR_final),
    .reset(reset), .txclk(txclk), .WR_channel(chan_WR),
    .WR_done_channel(chan_done), .ram_data(tx_data_bus));
	
   assign tx_underrun[0] = chan_underrun[0];

   channel_ram tx_data_packet_fifo 
   (.reset(reset), .txclk(txclk), .datain(tx_data_bus),
    .WR(chan_WR[0]), .WR_done(chan_done[0]),
    .have_space(chan_have_space[0]), .dataout(chan_fifodata[0]),
    .packet_waiting(chan_pkt_waiting[0]), .RD(chan_rdreq[0]),
    .RD_done(chan_skip[0]));

   mimo_fifo_reader tx_chan_reader 
   (.reset(reset), .tx_clock(txclk), .tx_strobe(txstrobe),
    .timestamp_clock(timestamp_clock), .samples_format(4'b0),          
    .tx_q_0(tx_q[0]), .tx_i_0(tx_i[0]), .tx_q_1(tx_q[1]), .tx_i_1(tx_i[1]), .underrun(chan_underrun[0]),
    .skip(chan_skip[0]), .rdreq(chan_rdreq[0]),
    .fifodata(chan_fifodata[0]), .pkt_waiting(chan_pkt_waiting[0]),
    .tx_empty(chan_txempty), .rssi(rssi[0]), .debug(debug[0]),
    .threshhold(threshhold), .rssi_wait(rssi_wait));	         

   channel_ram tx_cmd_packet_fifo 
   (.reset(reset), .txclk(txclk), .datain(tx_data_bus), .WR(chan_WR[NUM_CHAN]),
    .WR_done(chan_done[NUM_CHAN]), .have_space(chan_have_space[NUM_CHAN]),
    .dataout(chan_fifodata[NUM_CHAN]), .packet_waiting(chan_pkt_waiting[NUM_CHAN]),
    .RD(chan_rdreq[NUM_CHAN]), .RD_done(chan_skip[NUM_CHAN]));

   cmd_reader tx_cmd_reader
   (.reset(reset), .txclk(txclk), .timestamp_clock(timestamp_clock), .skip(chan_skip[NUM_CHAN]),
    .rdreq(chan_rdreq[NUM_CHAN]), .fifodata(chan_fifodata[NUM_CHAN]),
    .pkt_waiting(chan_pkt_waiting[NUM_CHAN]), .rx_databus(rx_databus),
    .rx_WR(rx_WR), .rx_WR_done(rx_WR_done), .rx_WR_enabled(rx_WR_enabled),
    .reg_data_in(reg_data_in), .reg_data_out(reg_data_out), .reg_addr(reg_addr),
    .reg_io_enable(reg_io_enable), .debug(debug[NUM_CHAN]), .stop(stop), .stop_time(stop_time));
				   
endmodule // tx_buffer

