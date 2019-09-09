#include <systemc.h>
#include "packet.h"		// importation d'un type Packet

#ifndef DISPLAY_H
#define DISPLAY_H

SC_MODULE(display)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	// Entrée indiquant qu'il y a des infos à afficher
	sc_in<bool> msg_valid;
	// Chaine de caractères à afficher
	sc_in<char*> input_message;
	// Paquet à afficher
	sc_in<Packet*> input_packet;
	//signal indiquant que le display est pret à afficher un message
	sc_out<bool> display_ready;

	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	// Paquet local au module
	Packet pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Fonction d'affichage des messages (THREAD)
	void display_message( void );	

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	// CTOR SystemC
	SC_CTOR(display)
	{
		// display_message est un thread
		SC_THREAD(display_message);
		// display_message est sensible à msg_valid (positif)
		sensitive_pos(msg_valid);
	}
};

#endif
