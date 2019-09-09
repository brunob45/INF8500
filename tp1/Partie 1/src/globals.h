
#include <systemc.h>
//#include <iostream.h>

#ifndef GLOBALS_H
#define GLOBALS_H

// Un identificateur unique est associ� � chaque terminaison de paquet, de plus une
// valeur est d�finie pour les paquets n'�tant pas associ�s � une terminaison contenue
// dans le syst�me de routage (out of range)
enum output_ports
{
	PACKET_OUT1 = 1,
	PACKET_OUT2 = 2,
	PACKET_OUT3 = 3,
	PACKET_DROP = 10
};

#endif
