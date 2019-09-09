#include <systemc.h>
#include "packet.h"		// importation du type Packet

#ifndef COPRO1_H
#define COPRO1_H

SC_MODULE(copro1)
{
	/* *******************************************************************
	// MODULE PORTS 
	******************************************************************** */
	// Le paquet provenant de l'interconnexion 
	sc_in<Packet*> packet_out;
	// Entr�e provenant de l'interconnexion  et indiquant qu'il y a un paquet � traiter
	sc_in<bool> ready;
	// Signal indiquant au l'interconnexion que le paquet a �t� trait�
	sc_out<bool> ack;
	// Signal indiquant au display qu'il y a un message � traiter
	sc_out<bool> msg_valid;
	// Sortie d'une cha�ne de caract�res pour le display
	sc_out<char*> output_msg;
	// Sortie du paquet vers le display
	sc_out<Packet*> output_pkt;
	// Acquittement du display (Le display est pr�t � afficher)
	sc_in<bool> display_ready;

	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	// Paquet local au module copro1
	Packet pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Ce fonction re�oit un paquet de l'interconnexion  et envoit le paquet au display
	void pkt_display(void); //THREAD

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	SC_CTOR(copro1)
	{
		// pkt_display est un THREAD
		SC_THREAD(pkt_display);
		// pkt_display est sensible � ready et � display_ready
		sensitive << ready << display_ready;
	}

};

#endif

