/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/packet_anno.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include <click/hydra_anno.hh>

#include "setpowerscen.hh"
#include "powercontrol.hh"

#include <math.h>

CLICK_DECLS

SetPowerScen::SetPowerScen()
  : _power_control(0), _verbose(0), 
    _packet_count(0), _cur_power(3000), _cur_target_snr(30.0), _c_map_power(0.1),
    _scen_num(0), _tx_amp(3000.0), _tx_pow(10.5), _tx_loss(22.0),
    _scen1_down(true)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

SetPowerScen::~SetPowerScen()
{
}

int
SetPowerScen::configure(Vector<String> &conf, ErrorHandler *errh)
{

  Element* e = 0;
  unsigned amp_tmp = 0;
  unsigned pow_tmp = 0;
  unsigned loss_tmp = 0;
  if (cp_va_parse(conf, this, errh,
  		  cpElement, "power control algorithm", &e,
		  cpOptional,
		  cpKeywords, 
		  "SCEN_NUM", cpUnsigned, "scenario number", &_scen_num, 
	          "TX_AMP", cpReal10, "Amplitude for calibration", 3, &amp_tmp,
	          "TX_POW", cpReal10, "Power for calibration [dBm]", 3, &pow_tmp,
	          "TX_LOSS", cpReal10, "Loss in the channel [dBm]", 3, &loss_tmp,
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }

  if (e) { 
    _power_control = (PowerControl *) e->cast("PowerControl");
    if(!_power_control) return errh->error("PowerControl must be set using PowerControl");
  } else {
    return errh->error("PowerControl must be defined");
  }


  _tx_amp = amp_tmp/1000.0;
  _tx_pow = pow_tmp/1000.0;
  _tx_loss = loss_tmp/1000.0;


  _c_map_power = _tx_pow/(_tx_amp*_tx_amp);
  printf("amp = %f, pow = %f, loss = %f, _c_map_power = %f\n", _tx_amp, _tx_pow, _tx_loss, _c_map_power);
  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

Packet *
SetPowerScen::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  _packet_count++;

  switch(_scen_num)
  {
    case 1:
    {
      scen_stairstep(); 
      _power_control->set_power(_cur_power); 
      break;
    }
    case 0:
    default:
      // Just use power set in PowerControl
      break;
  }


  WritablePacket* p_out = _power_control->get_power(p_in);

  hydra_chatter(9, _verbose, "%{element}::simple_action p_in=0x%x, p_out=0x%x", this, p_in, p_out);

  return p_out;
}


unsigned SetPowerScen::scen_stairstep()
{
  unsigned packet_per_power = 10; 
  float snr_step = 1.5;
  float highest_snr = 35;
  float lowest_snr = 14;

  if(_packet_count == 1)
  {
    _cur_target_snr = highest_snr;
    float exponent = (_cur_target_snr - _tx_loss) / 10.0;
    _cur_power = (unsigned) sqrt(pow(10.0, exponent)/_c_map_power); 
    return _cur_power;
  }
    
  if(_packet_count % packet_per_power)
    return _cur_power;

  if(_scen1_down)
  {
    _cur_target_snr -= snr_step;
    if(_cur_target_snr == lowest_snr)
      _scen1_down = false;
  }
  else
  {
    _cur_target_snr += snr_step;
    if(_cur_target_snr == highest_snr)
      _scen1_down = true;
  }

  float exponent = (_cur_target_snr - _tx_loss) / 10.0;
  _cur_power = (unsigned) sqrt(pow(10.0, exponent)/_c_map_power); 

  printf("_cur_target_snr = %f, _cur_power = %d\n", _cur_target_snr, _cur_power);
  return _cur_power;
}


enum {H_VERBOSE};

String
SetPowerScen::read_handler(Element *e, void *thunk)
{
  SetPowerScen *src = (SetPowerScen *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
SetPowerScen::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  SetPowerScen *src = (SetPowerScen *) e;
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
SetPowerScen::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetPowerScen)

