#include <systemc.h>
#include "packet.h"		// importation du type Packet

#ifndef INTERCONNEXION_H
#define INTERCOONEXION_H

SC_MODULE(interconnexion)
{
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	//////// Interface avec la g�n�ration de paquets ////////
	// Entr�e du paquet � transmettre au routeur
	sc_in<Packet*> packet_in;
	// Entr�e indiquant qu'un paquet est pr�t pour le routeur
	sc_in<bool> packet_ready;
	// signal indiquant que le routeur est pr�t � recevoir un nouveau paquet
	sc_out<bool> next_packet;

	//////// Interface avec le coprocesseur 1 ////////
	// Sortie du paquet � transmettre au coprocesseur 1
	sc_out<Packet*> packet_out;
	// Signal indiquant au l'interconnexion que le paquet a �t� trait�
	sc_in<bool> ack_copro1;
	// Entr�e provenant de l'interconnexion  et indiquant qu'il y a un paquet � traiter
	sc_out<bool> ready_copro1;
	

	//////// Interface avec le corocesseur 2 ////////
	// Sortie du paquet � transmettre au coprocesseur 2
	sc_fifo_out<Packet*> fifo_out;

	//////// Interface avec le coprocesseur 3 ////////
	// Sortie du paquet � transmettre au coprocesseur 3
	sc_out<Packet*> buffer_out;
	// Signal indiquant au l'interconnexion que le paquet a �t� trait�
	sc_in<bool> ack_copro3;


	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
	// Paquet local au module pkt_terminator
	Packet pkt;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Ce fonction re�oit un paquet du g�n�rateur et le dispatche � un des coprocesseurs
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

