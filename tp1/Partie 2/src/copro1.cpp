
#include "copro1.h"
#include <string.h>


void copro1::pkt_display()
{
	while(true)
	{
		msg_valid = false; //Indique qu'il n'y a plus de paquets valides pour display et qu'il faut recommencer un cycle
		
		//Recupération du paquet généré par l'interconnexion
		ack = false; 
		cout << "COPRO1 : Attente paquet pret" << endl;
		wait(ready.posedge_event()); // Attendre ready == true
		cout << "COPRO1 : paquet pret" << endl;
		cout << "COPRO1 : Recuperation du paquet" << endl;
		pkt = *packet_in;
		cout << "COPRO1 : Acquittement" << endl;
		ack = true;
		
		// Envoi du paquet au display
		msg_valid = true; //Indique qu'un nouveau paquet est disponible pour le display
		char message[100];
		output_pkt = &pkt;
		strcpy( message, "DISPLAY : Paquet en Provenance : " );
		strcat( message, name() );
		output_msg = message;

		// Indiquer au module display qu'il faut afficher un message
		cout << "COPRO1 : Envoi au display" << endl;
		cout << "COPRO1 : attente du display ready" << endl;
		wait(display_ready.value_changed_event()); // attente du signal indiquant que le paquet a été lu par le display
		cout << "COPRO1 : Display a bien recu le message" << endl;

	}
}
