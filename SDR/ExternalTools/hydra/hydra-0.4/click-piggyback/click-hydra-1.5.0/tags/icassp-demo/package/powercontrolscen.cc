/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
//#include <click/packet_anno.hh>
//#include <clicknet/ether.h>
//#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "powercontrolscen.hh"

#include <math.h>

CLICK_DECLS

PowerControlScen::PowerControlScen()
  : _verbose(0), _power(3000), _scen_num(0),
    _scen1_pkt_count(0), _scen1_cur_target_snr(30.0), _scen1_map_power(0.1),
    _scen1_tx_amp(3000.0), _scen1_tx_pow(10.5), _scen1_tx_loss(22.0),
    _scen1_highest_snr(35.0), _scen1_lowest_snr(10.0), _scen1_snr_step(0.1),
    _scen1_num_packets(1), _scen1_down(true)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

PowerControlScen::~PowerControlScen()
{
}

void* PowerControlScen::cast(const char *n)
{ 
  if( strcmp(n, "PowerControlScen") == 0 )
    return (PowerControlScen *) this;
  else
    return PowerControl::cast(n);                             
}


int PowerControlScen::configure(Vector<String> &conf, ErrorHandler *errh)
{

  unsigned amp_tmp = 0;
  unsigned pow_tmp = 0;
  unsigned loss_tmp = 0;
  unsigned highest_snr_tmp = 0;
  unsigned lowest_snr_tmp = 0;
  unsigned snr_step_tmp = 0;
  if (cp_va_parse(conf, this, errh,
	  cpUnsigned, "scenario number", &_scen_num, 
	  cpOptional,
	  cpKeywords, 
          "TX_AMP", cpReal10, "Amplitude for calibration", 3, &amp_tmp,
          "TX_POW", cpReal10, "Power for calib [dBm]", 3, &pow_tmp,
          "TX_LOSS", cpReal10, "Loss in the channel [dB]", 3, &loss_tmp,
          "TX_HSNR", cpReal10, "highest snr", 3, &highest_snr_tmp,
          "TX_LSNR", cpReal10, "lowest snr", 3, &lowest_snr_tmp,
          "TX_SSNR", cpReal10, "snr step", 5, &snr_step_tmp,
          "TX_NUM_SSNR", cpUnsigned, "packets per step", &_scen1_num_packets,
	  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
	  cpEnd) < 0) {
    return -1;
  }

  if(!_scen_num)
    return errh->error("Scenario number should be larger than 0");

  if(amp_tmp) _scen1_tx_amp = amp_tmp/1000.0;
  if(pow_tmp)  _scen1_tx_pow = pow_tmp/1000.0;
  if(loss_tmp) _scen1_tx_loss = loss_tmp/1000.0;
  if(highest_snr_tmp) _scen1_highest_snr = highest_snr_tmp/1000.0;
  if(lowest_snr_tmp) _scen1_lowest_snr = lowest_snr_tmp/1000.0;
  if(snr_step_tmp) _scen1_snr_step = snr_step_tmp/100000.0;

  _scen1_map_power = _scen1_tx_pow/(_scen1_tx_amp*_scen1_tx_amp);

  if(_scen_num == 1)
  {
    hydra_chatter(8, _verbose, "%{element}::scenario %d, \
                  amp = %f, pow = %f, loss = %f, _scen1_map_power = %f\n", \
                  this, _scen_num, \
                  _scen1_tx_amp, _scen1_tx_pow, _scen1_tx_loss, _scen1_map_power);
  }
  return 0;
}

Packet* PowerControlScen::simple_action(Packet *p_in)                
{                                                             
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  set_power(_power);
                                                              
  return p_in;                                                
}                


WritablePacket* PowerControlScen::get_power(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::get_power", this);

  WritablePacket* p_out = p_in->uniqueify();                  
                                                              
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
                                                              
  cha->power = (u_int16_t) _power;                            

  return p_out;
}


void PowerControlScen::set_power(unsigned power)             
{                                                             
  hydra_chatter(8, _verbose, "%{element}::set_power", this);
  switch(_scen_num)
  {
    case 1:
    {
      // FIXME: Scen_num = 1 should be an inner class
      _power = scen1_stairstep(power); 
      break;
    }
    case 0:
    default:
      // Just use power set in PowerControl
      break;
  }

}             


unsigned PowerControlScen::scen1_stairstep(unsigned cur_power)
{
  unsigned power = cur_power;

  _scen1_pkt_count++;
  if(_scen1_pkt_count == 1)
  {
    _scen1_cur_target_snr = _scen1_highest_snr;
    float exponent = (_scen1_cur_target_snr - _scen1_tx_loss) / 10.0;
    power = (unsigned) sqrt(pow(10.0, exponent)/_scen1_map_power); 
    return power;
  }
    
  if(_scen1_pkt_count % _scen1_num_packets)
    return power;

  if(_scen1_down)
  {
    _scen1_cur_target_snr -= _scen1_snr_step;
    if(_scen1_cur_target_snr == _scen1_lowest_snr)
      _scen1_down = false;
  }
  else
  {
    _scen1_cur_target_snr += _scen1_snr_step;
    if(_scen1_cur_target_snr == _scen1_highest_snr)
      _scen1_down = true;
  }

  float exponent = (_scen1_cur_target_snr - _scen1_tx_loss) / 10.0;
  power = (unsigned) sqrt(pow(10.0, exponent)/_scen1_map_power); 

  hydra_chatter(8, _verbose, "%{element}:Scen1: _scen1_cur_target_snr = %f, power = %d\n", this, _scen1_cur_target_snr, power);

  return power;
}


enum {H_VERBOSE};

String
PowerControlScen::read_handler(Element *e, void *thunk)
{
  PowerControlScen *src = (PowerControlScen *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
PowerControlScen::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  PowerControlScen *src = (PowerControlScen *) e;
  String s = cp_uncomment(arg);
  switch((int)vparam) {
  case H_VERBOSE: {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("verbose parameter must be unsigned");
    src->_verbose = m;
    break;
  }
  }
  return 0;
}

void
PowerControlScen::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PowerControlScen)

