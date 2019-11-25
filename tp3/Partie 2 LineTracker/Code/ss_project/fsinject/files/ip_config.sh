#!/bin/sh

ifconfig eth0 down
# INF8500-01
#ifconfig eth0 132.207.89.101 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-02
#ifconfig eth0 132.207.89.102 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-03
#ifconfig eth0 132.207.89.103 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-04
#ifconfig eth0 132.207.89.104 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-05
#ifconfig eth0 132.207.89.105 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-06
#ifconfig eth0 132.207.89.106 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-07
#ifconfig eth0 132.207.89.107 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-08
#ifconfig eth0 132.207.89.108 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-09
#ifconfig eth0 132.207.89.109 broadcast 132.207.89.127 netmask 255.255.255.128
# INF8500-10
#ifconfig eth0 132.207.89.110 broadcast 132.207.89.127 netmask 255.255.255.128
route add default gw 132.207.89.1
#echo nameserver 192.168.0.254 > /etc/resolv.conf