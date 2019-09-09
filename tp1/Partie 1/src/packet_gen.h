
#include <systemc.h>
#include "packet.h"		// importation du type Packet

#ifndef PACKETGEN_H
#define PACKETGEN_H

SC_MODULE(packet_gen)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	// signal indiquant que le routeur est prêt à recevoir un nouveau paquet
	sc_in<bool> next_packet;
	// Sortie indiquant qu'un paquet est prêt pour le routeur
	sc_out<bool> packet_ready;
	// Sortie du paquet à transmettre au routeur
    sc_out<Packet*> packet_out;

	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	Packet *pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Fonction qui genère les paquets (THREAD)
	void generate( void );

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	SC_CTOR(packet_gen)
	{
		// generate est un thread
		SC_THREAD(generate);
		// generate est sensible à next_packet
		sensitive << next_packet;
	}
};

#endif
