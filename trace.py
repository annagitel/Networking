#coding: utf-8
from scapy.all import*
for i in range (100):
	a=IP()
	a.dst="8.8.8.8"
	a.ttl=i
	b=ICMP()
	p=a/b
	send(p)
