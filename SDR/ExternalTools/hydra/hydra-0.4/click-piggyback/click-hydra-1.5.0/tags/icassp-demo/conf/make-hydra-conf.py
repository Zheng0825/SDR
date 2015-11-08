#!/usr/bin/env python

import sys
import string
from string import Template
from optparse import OptionParser

class click_parser(object):
  def __init__(self, subnet, node, port, routes):
    self.ip = self.prepare_ip(subnet, node)
    (self.mac, self.mac_cl) = self.prepare_mac(subnet, node)
    (self.to_port, self.from_port) = self.prepare_port(port)
    (self.all_addr, self.static_routes) = self.prepare_routes(subnet, routes)
    self.hydra_if = self.prepare_if(subnet)

  # Prepare IP string
  def prepare_ip(self, subnet, node):
    ip_digit = subnet.split('.')
    ip = ip_digit[0] + '.' + ip_digit[1] + '.'  + ip_digit[2] + '.' + node 
    return ip

  # Prepare MAC strings
  def prepare_mac(self, subnet, node):
    mac_digit = subnet.split('.')
    for i in range(3):
      mac_digit[i] = hex(int(mac_digit[i])).replace('0x', '')
    mac_digit[3] = hex(int(node)).replace('0x', '')

    for i in range(4):
      if len(mac_digit[i]) == 1:
        mac_digit[i] = '0' + mac_digit[i]

    mac = 'ff' + ':' + '0d' + ':' + mac_digit[0] + ':' + mac_digit[1] + ':' + mac_digit[2] + ':' + mac_digit[3]
    mac_cl = 'ff' + '0d' + mac_digit[0] + mac_digit[1] + mac_digit[2] + mac_digit[3] 
    return mac, mac_cl
    
  # Prepare PORT strings
  def prepare_port(self, port):
    to_port = port 
    from_port = str(int(port) + 1)
    return to_port, from_port

  # Prepare ROUTE strings
  def prepare_routes(self, subnet, routes):
    route_arr = routes.split(",")

    for i in range(len(route_arr)):
      route_arr[i] = route_arr[i].lstrip().rstrip()
  
    all_addr = '' 
    static_routes = 'nb::StaticRouteTable(me:eth me:ip, 8'
    for i in range(len(route_arr)):
      (str_addr, str_route) = self.prepare_route(subnet, route_arr[i]) 
      all_addr += 'AddressInfo(' + str_addr + ');\n'
      static_routes += ',\n ' + str_route 
    static_routes += ');\n'

    return all_addr, static_routes
    

  # rarr = dest_node, next node, hop_count 
  def prepare_route(self, subnet, route):
    rarr = route.split(':')
    if len(rarr) != 3:
      sys.stderr.write("Wrong syntax for static route, it should be dest:next:hop_count")
      sys.exit(1)

    dest_ip = self.prepare_ip(subnet, rarr[0])
    (dest_mac, tmp) = self.prepare_mac(subnet, rarr[0])

    addr_info = 'plan' + rarr[0] + ' ' + dest_ip + ' ' + dest_mac 
    route_info = 'plan' + rarr[0] + ':ip plan' + rarr[1] + ':eth plan' + rarr[1] + ':ip ' + rarr[2]
    return addr_info, route_info


  def prepare_if(self, subnet):
    subnet_arr = subnet.split('.')
    str_if = 'hydra' + subnet_arr[2]
    return str_if 

     
  def parse_line(self, input_line):
    s = Template(input_line)
    output_line = s.substitute(IP = self.ip, MAC = self.mac, MAC_CL = self.mac_cl, 
                               TO_PORT = self.to_port, FROM_PORT = self.from_port,
                               ALL_ADDR = self.all_addr, 
                               STATIC_ROUTES = self.static_routes,
                               HYDRA_IF = self.hydra_if)
    return output_line

    


def main():
  parser = OptionParser()
  parser.add_option("", "--subnet", type = "string", default="192.168.255.0",
                    help="set subnet of IP(MAC) addr [default=%default]")
  parser.add_option("", "--node", type = "string", default="11",
                    help="set the last digit of IP(MAC) addr [default=%default]")
  parser.add_option("", "--mpi-port", type = "string", default="60001",
                    help="set the UDP port for MPI [default=%default]")
  parser.add_option("", "--routes", type = "string", default="",
                    help="set static routes [default=%default]")

  (options, args) = parser.parse_args()

  if len(args) != 1:
    parser.print_help()
    sys.exit(1)

  my_parser = click_parser(options.subnet, options.node, options.mpi_port, options.routes)

  ccb_file = open(args[0], "r")
  line = 'init'
  while line != '':
    line = ccb_file.readline()
    line = my_parser.parse_line(line)
    sys.stdout.write("%s" % (line))
    


if __name__ == '__main__':
  try:
    main()
  except KeyboardInterrupt:
    pass
