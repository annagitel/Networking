#!/bin/bin/python
from scapy.all import *

a = IP()
a.src = '99.99.99.99'
a.dst = '192.168.0.100'		
b = ICMP()
p = a/b
send(p)
print ("done")
