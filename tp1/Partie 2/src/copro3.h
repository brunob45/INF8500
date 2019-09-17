#include <systemc.h>
#include "packet.h"		// importation du type Packet

#ifndef COPRO3_H
#define COPRO3_H

SC_MODULE(copro3)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	// Le paquet provenant de l'interconnexion 
	sc_in<Packet*> buffer_in;

	sc_out<bool> ack;
	
	// Signal indiquant au display qu'il y a un message à traiter
	sc_out<bool> msg_valid;
	// Sortie d'une chaîne de caractères pour le display
	sc_out<char*> output_msg;
	// Sortie du paquet vers le display
	sc_out<Packet*> output_pkt;
	// Acquittement du display (Le display est prêt à afficher)
	sc_in<bool> display_ready;

	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	// Paquet local au module copro3
	Packet pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Ce fonction reçoit un paquet de l'interconnexion et envoit le paquet au display
	void pkt_display(void);

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	SC_CTOR(copro3)
	{

		// pkt_display est un thread
		SC_THREAD(pkt_display);
		// pkt_display est sensible à display_ready et à buffer_in
		sensitive << display_ready << buffer_in;

	}

};

#endif

