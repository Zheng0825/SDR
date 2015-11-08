/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/hydra_anno.hh>
#include "logtcp.hh"

CLICK_DECLS

LogTCP::LogTCP()
  : _logfilename("stdout"), _fptr_log(stdout) 
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

LogTCP::~LogTCP()
{
}

int
LogTCP::configure(Vector<String> &conf, ErrorHandler *errh)
{

  if (cp_va_parse(conf, this, errh,
      cpFilename, "log file", &_logfilename,
		  cpOptional,
		  cpKeywords, 
 		  "LOGTYPE", cpUnsigned, "logtype", &_logtype,
 		  "VERBOSE", cpUnsigned, "verbose", &_verbose,
		  cpEnd) < 0) {
    return -1;
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

int
LogTCP::initialize(ErrorHandler *errh)
{
  hydra_chatter(9, _verbose, "%{element}::configure output file name %s\n", this, _logfilename.c_str());
  if (_logfilename != "stdout") 
  {
    _fptr_log = fopen(_logfilename.c_str(), "wt");
    if (!_fptr_log)
      return errh->error("%s: %s", _logfilename.c_str(), strerror(errno));
  } 
  
  printf("LogTCP: Opened log file %s\n", _logfilename.c_str());
  return 0;
}

void 
LogTCP::push(int port, Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);
  
  // log tcp statistics for experiments  
  process_data(p, _logtype);

  output(0).push(p);	
}

void LogTCP::process_data(Packet* pkt, int log_type)
{
  struct timeval now;
  click_gettimeofday(&now);
  int offset=0;

  Packet* cpkt = pkt->clone();

  if(log_type==0)
  {
    offset=116; // two byte tcp win field after 102 byte
    unsigned short win[2];

    win[0] = *((unsigned char *)(cpkt->data()+offset));
    win[1] = *((unsigned char *)(cpkt->data()+offset+1));

    unsigned short tcpwin = ((win[0]<<8)| win[1]);

    printf("TX tcpwin %06d \n", (int) tcpwin);
    fprintf(_fptr_log, "%d.%06d TX tcpwin  %06d \n", now.tv_sec, now.tv_usec, (int) tcpwin);
  }
  else if(log_type==1)
  {
    offset=115; // FLAG fields of TCP Header
    unsigned char ack_flag = *((unsigned char *)(cpkt->data()+offset));
    unsigned char mask=0x10;  

    int tcpack = (ack_flag && mask)>>5;

    if(tcpack)
    {
      // TCP acknowledgement number
      unsigned int num_ack[4];
      unsigned int ack_num; 
      offset=110; // ACK number start here

      num_ack[0] = *((unsigned char *)(cpkt->data()+offset));
      num_ack[1] = *((unsigned char *)(cpkt->data()+offset+1));
      num_ack[2] = *((unsigned char *)(cpkt->data()+offset+2));
      num_ack[3] = *((unsigned char *)(cpkt->data()+offset+3));
   
      ack_num =((num_ack[0]<<24)|(num_ack[1]<<16)|(num_ack[2]<<8)|(num_ack[3]));  
      
      printf("TCPACK seq %06d\n", ack_num);
      fprintf(_fptr_log, "%d.%06d TCPACK seq %06d\n", now.tv_sec, now.tv_usec, ack_num);
    }

  }

}

CLICK_ENDDECLS
EXPORT_ELEMENT(LogTCP)
