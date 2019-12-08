ifconfig eth0 down
ifconfig eth0 hw ether 00:18:3E:00:00:02
ifconfig eth0 132.207.89.102 broadcast 132.207.89.127 netmask 255.255.255.128 up
route add default gw 132.207.89.1
ifconfig eth0 up

ping 8.8.8.8

./zynq0.core0.arm_a9.elf
