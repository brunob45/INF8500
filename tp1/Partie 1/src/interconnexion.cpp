#include "interconnexion.h"
#include "globals.h"
#include <string.h>

void interconnexion::pkt_dispatch(void){
	
	int addr;

	while (true)
	{
		//Recupération du paquet
		next_packet = true; 
		cout << "INTER : Attente paquet pret" << endl;
		wait(packet_ready.posedge_event()); // Attendre ready == true
		cout << "INTER : paquet pret" << endl;
		cout << "INTER : Recuperation du paquet" << endl;
		pkt = *packet_in;
		cout << "INTER : Acquittement" << endl;
		next_packet = false;

		//Lecture de l'adresse du paquet
		addr = pkt.getAddress();

		//Utiliser la bonne méthode d'envoi selon l'adresse
		switch (addr){
		case PACKET_OUT1:
			pkt_send1();
			break;
		case PACKET_OUT2:
			pkt_send2();
			break;
		case PACKET_OUT3:
			pkt_send3();
			break;
		}
		// Message de sortie pour le module display
	}
}

void interconnexion::pkt_send1(void){
	packet_out = &pkt;
	ready_copro1 = true;
	wait(ack_copro1.posedge_event()); // Attendre ack == true
	ready_copro1 = false;
}

void interconnexion::pkt_send2(void){
	
	fifo_out.write(&pkt);

}

void interconnexion::pkt_send3(void){	

	buffer_out.write(&pkt);
	
	//Attendre que le copro3 soit pret
	wait(ack_copro3.posedge_event()); // Attendre ack == true

}
