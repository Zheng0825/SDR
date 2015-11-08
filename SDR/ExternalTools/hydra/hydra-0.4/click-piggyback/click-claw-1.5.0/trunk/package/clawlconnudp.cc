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

#include <clawadaptor.hh>

#include <claw/claw_data.h>
#include <claw/claw_event.h>
#include <claw/claw_format.h>

#include "clawlconnudp.hh"

using namespace std;

CLICK_DECLS

ClawLConnUDP::ClawLConnUDP() 
  : ClawLConn("Click", "ILIB"), 
    _lconn_port(0), _lconn_sk(0), 
    _gconn_ports(NUM_GCONNS, 0),
    _verbose(0) 
{
  _aliase_names.push_back("ILOOB");
  _aliase_names.push_back("INIB");
  _aliase_names.push_back("INOOB");

  hydra_chatter(9, 0, "%{element} constructor = 0x%x\n", this, (ClawLConnUDP *) this);
}

ClawLConnUDP::~ClawLConnUDP()
{
  terminate_sockets();
}


void* ClawLConnUDP::cast(const char *n)
{
  if( strcmp(n, "ClawLConnUDP") == 0 )
    return (ClawLConnUDP *) this;
  else if( strcmp(n, "ClawLConn") == 0 )
    return (ClawLConn *) this;

  return Element::cast(n);
}

int ClawLConnUDP::configure(Vector<String> &conf, ErrorHandler *errh)
{
  unsigned gilib_port;
  unsigned giloob_port;
  unsigned ginib_port;
  unsigned ginoob_port;

  int res = cp_va_parse(conf, this, errh, 
                    cpUnsigned, "Global ILIB Connector Port", &gilib_port,
                    cpUnsigned, "Global ILOOB Connector Port", &giloob_port,
                    cpUnsigned, "Global INIB Connector Port", &ginib_port,
                    cpUnsigned, "Global INOOB Connector Port", &ginoob_port,
                    cpUnsigned, "Local Connector Port", &_lconn_port,
    		    cpOptional, cpKeywords,
                    "VERBOSE", cpUnsigned, "noisy?", &_verbose, 
		    cpEnd);

  _gconn_ports[ILIB] = (unsigned short) gilib_port;
  _gconn_ports[ILOOB] = (unsigned short) giloob_port;
  _gconn_ports[INIB] = (unsigned short) ginib_port;
  _gconn_ports[INOOB] = (unsigned short) ginoob_port;

  hydra_chatter(9, _verbose, "configure got [%u %u %u %u] [%u]\n", gilib_port, giloob_port, ginib_port, ginoob_port, _lconn_port);

  init_sockets();
  
  if(_lconn_sk)
    add_select(_lconn_sk, SELECT_READ);
  
  return res;
}

void ClawLConnUDP::selected(int fd)
{
  //assert(fd == _lconn_sk);
  int conn_type = 0;

  int size = recv_from_gconn(&conn_type);
  unsigned char info_type = _format->get_type(_msg);

  hydra_chatter(9, _verbose, "ClawLConnUDP::selected: received\n");

  size = process_gconn_req(info_type, size);
  if(size)
    send_to_gconn(conn_type, size);
  else
    hydra_chatter(9, _verbose, "ClawLConnUDP::selected: not this case\n");
}

bool ClawLConnUDP::is_name(string& conn_name)
{
  //printf("ClawLConnUDP::is_name %s\n", conn_name.c_str());
  if(_name == conn_name)
    return true;

  for(int i=0; i<_aliase_names.size(); i++)
  {
    if(_aliase_names[i] == conn_name)
      return true;
  } 

/*
  switch(conn_name)
  {
    case "ILIB":
    case "ILOOB":
    case "INIB":
    case "INOOB":
      return true;
  }
*/
  //printf("ClawLConnUDP::is_name return false\n");
  return false;

}

bool ClawLConnUDP::is_same(ClawLConn* rhs) 
{
  if(this == rhs)
    return true;

  if(rhs->is_name(_name))
    return true;

  // FIXME:
  // if one of aliase name matches return true;

}

void ClawLConnUDP::register_adaptor(ClawAdaptor* adaptor)
{
  // FIXME: let caller know the confliction
  ClawAdaptor* prev = find_adaptor(adaptor);
  if(prev)
    return;  
  
  insert_adaptor(adaptor); 
}

// it should be block call that pause the adaptor
// It just send the event to general connector
void ClawLConnUDP::process_adaptor_event(ClawAdaptor* adaptor, ClawEvent* event)
{
  // first send this event to proper gconn
  unsigned char info_type = _format->EVENT_NOTIFY;
  int size = _format->marshall(_msg, _format->EVENT_NOTIFY, event);
  int conn_type = conn_type_int(event->conn_name());

  size = send_to_gconn(conn_type, size);
  hydra_chatter(9, _verbose, "ClawLConnUDP::process_adaptor_event: sent event\n");

  while(info_type != _format->EVENT_REPLY) 
  {

    size = recv_from_gconn(&conn_type);

    // Keep iterating until it gets event reply
    if((info_type = _format->get_type(_msg)) != ClawFormat::EVENT_REPLY)
    {
       size = process_gconn_req(info_type, size);
       if(size)
         send_to_gconn(conn_type, size);

       continue;
    }

  }

  _format->unmarshall(_msg, &info_type, event);

  hydra_chatter(9, _verbose, "ClawLConnUDP::process_adaptor_event: finish event\n");

}

int ClawLConnUDP::get_info(ClawData* data)
{
  int res = -1;

  hydra_chatter(9, _verbose, "ClawLConnUDP::get_info [%s %s]\n", data->adaptor_name().c_str(), data->info_name().c_str());
  ClawAdaptor* ca = find_adaptor(data->adaptor_name()); 
  if(!ca) return res;
  hydra_chatter(9, _verbose, "ClawLConnUDP::get_info found adaptor");

  return ca->claw_getinfo(data);
}

int ClawLConnUDP::set_info(ClawData* data)
{
  int res = -1;

  hydra_chatter(9, _verbose, "ClawLConnUDP::set_info [%s %s]\n", data->adaptor_name().c_str(), data->info_name().c_str());
  ClawAdaptor* ca = find_adaptor(data->adaptor_name()); 
  if(!ca) return res;

  hydra_chatter(9, _verbose, "ClawLConnUDP::set_info found adaptor");
  return ca->claw_setinfo(data);
}

int ClawLConnUDP::process_gconn_req(unsigned char info_type, int size)
{
  // FIXME: check if gconn send request other than data
  ClawData data;

  _format->unmarshall(_msg, &info_type, &data);

  if(info_type == ClawFormat::DATA_GET)
  {
    get_info(&data);
  }
  else if(info_type == ClawFormat::DATA_SET)
  {
    set_info(&data);
  }
    
  size = _format->marshall(_msg, _format->DATA_REPLY, &data);

  return size;
}

int ClawLConnUDP::send_to_gconn(int conn_type, int size)
{
  if(!_lconn_port)
    return 0;

  _gconn_sin.sin_port = htons(_gconn_ports[conn_type]);

  //hydra_chatter(9, _verbose, "ClawLConnUDP:send_to_gconn: type = %d port = %d size = %d\n", conn_type, _gconn_ports[conn_type], size);
  int send_size = sendto( _lconn_sk, _msg, size, 0, 
                          (struct sockaddr *)&_gconn_sin, sizeof(_gconn_sin) );

  return send_size;
}

int ClawLConnUDP::recv_from_gconn(int* conn_type)
{
  if(!_lconn_port)
    return 0;

  struct sockaddr_in from_addr;
  socklen_t addr_len = sizeof(from_addr); 

  int  size = _format->MAX_FORMAT_LEN;
  size = recvfrom( _lconn_sk, _msg, size, 0, 
                   (struct sockaddr*)&from_addr, &addr_len );

  // Find who sent 
  for(int i=0; i<_gconn_ports.size(); i++)
  {
    if(_gconn_ports[i] == htons(from_addr.sin_port))
    {
      *conn_type = i;
      break;
    } 

  }
  
  //hydra_chatter(9, _verbose, "ClawLConnUDP:recv_from_gconn: type = %d port = %d size = %d\n", *conn_type, htons(from_addr.sin_port), size);

  return size;
}

ClawAdaptor* ClawLConnUDP::find_adaptor(string& name)
{
  int num_adaptors = _adaptors.size();

  for(int i=0; i<num_adaptors; i++)
  {
    ClawAdaptor* ca = _adaptors[i];

    if(ca->is_name(name))
      return (ClawAdaptor *)_adaptors[i];
  }
 
  return 0; 
}

ClawAdaptor* ClawLConnUDP::find_adaptor(ClawAdaptor* adaptor)
{
  int num_adaptors = _adaptors.size();

  for(int i=0; i<num_adaptors; i++)
  {
    ClawAdaptor* ca = _adaptors[i];

    if(ca->is_same(adaptor))
      return (ClawAdaptor *)_adaptors[i];
  }
 
  return 0; 
}

void ClawLConnUDP::insert_adaptor(ClawAdaptor* adaptor)
{
  printf("ClawLConnUDP::insert_adaptor: %s\n", adaptor->adaptor_name().c_str());
  ClawAdaptor* prev = find_adaptor(adaptor->adaptor_name());
  if(prev)
    return;

  printf("ClawLConnUDP::insert_adaptor: %s inserted\n", adaptor->adaptor_name().c_str());
  _adaptors.push_back(adaptor);
 
  return; 
}

void ClawLConnUDP::init_sockets()
{ 
  if(!_lconn_port) 
    return;

  struct sockaddr_in sin;
  
  // recv socket binding 
  bzero((void*)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(0x7F000001);                    
  sin.sin_port = htons(_lconn_port);                           
                                                             
  _lconn_sk = socket(PF_INET, SOCK_DGRAM, 0);                 
  bind(_lconn_sk, (struct sockaddr *)&sin, sizeof(sin));      
                                                             
  // prepare dest addresses
  bzero((void*)&_gconn_sin, sizeof(sin));       
  _gconn_sin.sin_family = AF_INET;                        
  _gconn_sin.sin_addr.s_addr = htonl(0x7F000001);          

}                                                    

void ClawLConnUDP::terminate_sockets()
{
  if(!_lconn_sk)
    close(_lconn_sk);
}

int ClawLConnUDP::conn_type_int(string& conn_name)
{
  if( conn_name ==  "ILIB")
    return ILIB;
  if( conn_name ==  "ILOOB")
    return ILOOB;
  if( conn_name ==  "INIB")
    return INIB;
  if( conn_name ==  "INOOB")
    return INOOB;
  
  hydra_chatter(9, _verbose, "ClawLConnUDP:conn_type: never this case %s\n", conn_name.c_str());
  return -1;
}
         
CLICK_ENDDECLS
EXPORT_ELEMENT(ClawLConnUDP)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
