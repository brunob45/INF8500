#include <systemc.h>
#include "packet.h"		// importation du type Packet

#ifndef INTERCONNEXION_H
#define INTERCOONEXION_H

SC_MODULE(interconnexion)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	//////// Interface avec la génération de paquets ////////
	// Entrée du paquet à transmettre au routeur
	sc_in<Packet*> packet_in;
	// Entrée indiquant qu'un paquet est prêt pour le routeur
	sc_in<bool> packet_ready;
	// signal indiquant que le routeur est prêt à recevoir un nouveau paquet
	sc_out<bool> next_packet;

	//////// Interface avec le coprocesseur 1 ////////
	// Sortie du paquet à transmettre au coprocesseur 1
	sc_out<Packet*> packet_out;
	// Signal indiquant au l'interconnexion que le paquet a été traité
	sc_in<bool> ack_copro1;
	// Entrée provenant de l'interconnexion  et indiquant qu'il y a un paquet à traiter
	sc_out<bool> ready_copro1;
	

	//////// Interface avec le corocesseur 2 ////////
	// Sortie du paquet à transmettre au coprocesseur 2
	sc_fifo_out<Packet*> fifo_out;

	//////// Interface avec le coprocesseur 3 ////////
	// Sortie du paquet à transmettre au coprocesseur 3
	sc_out<Packet*> buffer_out;
	// Signal indiquant au l'interconnexion que le paquet a été traité
	sc_in<bool> ack_copro3;


	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	// Paquet local au module pkt_terminator
	Packet pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Ce fonction reçoit un paquet du générateur et le dispatche à un des coprocesseurs
	void pkt_dispatch(void); //THREAD
	void pkt_send1(void);
	void pkt_send2(void);
	void pkt_send3(void);

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	SC_CTOR(interconnexion)
	{
		
		// pkt_dispatch est un THREAD
		SC_THREAD(pkt_dispatch);
		// pkt_dispatch est sensible aux signaux suivants
		sensitive << packet_ready << ack_copro1 << ack_copro3;
		
	}

};

#endif

