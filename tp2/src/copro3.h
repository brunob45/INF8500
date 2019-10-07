#include <systemc.h>
#include "packet.h"		// importation du type Packet
#include "BubbleSort.h"

#ifndef COPRO3_H
#define COPRO3_H

SC_MODULE(copro3)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	// Le paquet provenant de l'interconnexion 
	sc_in<Packet*> packet_in;
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

	//Retour du copro3 a son adapteur
        sc_fifo_out<Packet*>  packet_out;


	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	// Paquet local au module copro3
	Packet pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Ce fonction re�oit un paquet de l'interconnexion et envoit le paquet au display
	void pkt_display(void);

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	SC_CTOR(copro3)
	{
		// pkt_display est un THREAD
		SC_THREAD(pkt_display);
	}

};

#endif

