
#include <systemc.h>
#include "packet.h"		// importation du type Packet

#ifndef PACKETGEN_H
#define PACKETGEN_H

SC_MODULE(packet_gen)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	// signal indiquant que le routeur est prét à recevoir un nouveau paquet
	sc_in<bool> next_packet;
	// Sortie indiquant qu'un paquet est prêt pour le routeur
	sc_out<bool> packet_ready;
	// Sortie du paquet à transmettre au routeur
	sc_out<Packet*> packet_out;
	// Signal entrée indiquant que le monitor a recule paquet
	sc_in<bool> monitor_next;

	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	Packet *pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	SC_HAS_PROCESS(packet_gen);
	// Fonction qui genére les paquets (THREAD)
	void generate( void );

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	packet_gen(sc_module_name name) : sc_module(name)
	{
		// generate est un thread
		SC_THREAD(generate);
		// generate est sensible à next_packet
		sensitive(next_packet);
		// generate est sensitive à monitor_next
		//sensitive(monitor_next);		
	}
};

#endif
