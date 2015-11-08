/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#include <stdio.h>
 
#include <click/config.h>
#include <click/glue.hh>
#include <clicknet/ether.h>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>
//#include <clicknet/80211.h>

#include "clawconnclick.hh"

CLICK_DECLS

ClawConnClick::ClawConnClick() : _verbose(0)
{
  init_my_sockets();
  init_conn_sockets();
  hydra_chatter(9, 0, "ClawConn constructor = 0x%x\n", this, (ClawConnProtModule *) this);
}

ClawConnClick::~ClawConnClick()
{
}

void* ClawConnClick::cast(const char *n)
{
  if( strcmp(n, "ClawConnClick") == 0 )
    return (ClawConnClick *) this;
  else if( strcmp(n, "ClawConnProtModule") == 0 )
    return (ClawConnProtModule *) this;

  return Element::cast(n);
}

int ClawConnClick::configure(Vector<String> &conf, ErrorHandler *errh)
{
  // FIXME: These two arguments should not be optional.
  int res = cp_va_parse(conf, this, errh, 
    		    cpOptional, cpKeywords,
                    "VERBOSE", cpUnsigned, "noisy?", &_verbose, 
		    cpEnd);
  
  return res;
}

Packet* ClawConnClick::simple_action(Packet *pkt)
{
  return pkt;
}

void ClawConnClick::init_my_sockets()
{ 
  unsigned int   recv_addr = INADDR_ANY; 
  unsigned short recv_port = 30002;
  unsigned int   send_addr = INADDR_ANY; 
  unsigned short send_port = 30004;

  struct sockaddr_in sin;
  
  // recv socket binding 
  bzero((void*)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(recv_addr);                    
  sin.sin_port = htons(recv_port);                           
                                                             
  _recv_sk = socket(PF_INET, SOCK_DGRAM, 0);                 
  bind(_recv_sk, (struct sockaddr *)&sin, sizeof(sin));      
                                                             
  // send socket binding                                     
  bzero((void*)&sin, sizeof(sin));                           
  sin.sin_family = AF_INET;                                  
  sin.sin_addr.s_addr = htonl(send_addr);                    
  sin.sin_port = htons(send_port);                           
                                                             
  _send_sk = socket(PF_INET, SOCK_DGRAM, 0);                 
  bind(_send_sk, (struct sockaddr *)&sin, sizeof(sin));      
                                                             
}                                                    

void ClawConnClick::init_conn_sockets()                    
{                                                            
  unsigned int   dest_addr = 0x7F000001;                     
  unsigned short dest_port = 30001;                          
                                                             
  // prepare dest address                                    
  bzero((void*)&_dest_address, sizeof(_dest_address));       
  _dest_address.sin_family = AF_INET;                        
  _dest_address.sin_addr.s_addr = htonl(dest_addr);          
  _dest_address.sin_port = htons(dest_port);                 
                                                             
}          

void ClawConnClick::send_to_conn(int* size, char* msg)
{
  socklen_t addr_len = sizeof(_dest_address); 

  hydra_chatter(9, _verbose, "%{element}:send_to_conn: %d\n", this, *size);
  //printf("ClawConnClick:send_to_conn: %d\n", *size);
  int send_size = sendto(_send_sk, msg, *size, 0, (struct sockaddr *)&_dest_address, addr_len);

  *size = send_size;
}

void ClawConnClick::recv_from_conn(int* size, char* msg)
{
  struct sockaddr_in from_addr;
  socklen_t addr_len = sizeof(_dest_address); 

  int recv_size = recvfrom(_recv_sk, msg, *size, 0, (struct sockaddr*)&from_addr, &addr_len);

  *size = recv_size;
  hydra_chatter(9, _verbose, "%{element}:recv_from_conn: %d\n", this, *size);
  //printf("ClawConnClick:recv_from_conn: %d\n", *size);

}


         
CLICK_ENDDECLS
EXPORT_ELEMENT(ClawConnClick)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
