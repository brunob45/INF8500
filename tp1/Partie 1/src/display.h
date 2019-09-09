#include <systemc.h>
#include "packet.h"		// importation d'un type Packet

#ifndef DISPLAY_H
#define DISPLAY_H

SC_MODULE(display)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	// Entr�e indiquant qu'il y a des infos � afficher
	sc_in<bool> msg_valid;
	// Chaine de caract�res � afficher
	sc_in<char*> input_message;
	// Paquet � afficher
	sc_in<Packet*> input_packet;
	//signal indiquant que le display est pret � afficher un message
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
		// display_message est sensible � msg_valid (positif)
		sensitive_pos(msg_valid);
	}
};

#endif
