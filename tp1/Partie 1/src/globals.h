
#include <systemc.h>
//#include <iostream.h>

#ifndef GLOBALS_H
#define GLOBALS_H

// Un identificateur unique est associé à chaque terminaison de paquet, de plus une
// valeur est définie pour les paquets n'étant pas associés à une terminaison contenue
// dans le système de routage (out of range)
enum output_ports
{
	PACKET_OUT1 = 1,
	PACKET_OUT2 = 2,
	PACKET_OUT3 = 3,
	PACKET_DROP = 10
};

#endif
