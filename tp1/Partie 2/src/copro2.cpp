
#include "copro2.h"
#include <string.h>


void copro2::pkt_display()
{
	while (true)
	{
		msg_valid = false; //Indique qu'il n'y a plus de paquets valides et qu'il faut recommencer un cycle
		
		//Recupération du paquet généré par l'interconnexion
		cout << "COPRO2 : Attente paquet pret" << endl;
		pkt = *fifo_in.read(); //Lecture bloquante
		cout << "COPRO2 : Paquet recu" << endl;
		
		// Envoi du paquet au display
		msg_valid = true; //Indique qu'un nouveau paquet est disponible pour le display
		char message[100];
		output_pkt = &pkt;
		strcpy( message, "DISPLAY2 : Paquet en Provenance : " );
		strcat( message, name() );
		output_msg = message;

		// Indiquer au module display qu'il faut afficher un message
		cout << "COPRO2 : Envoi au display" << endl;
		cout << "COPRO2 : attente du display ready" << endl;
		wait(display_ready.value_changed_event()); // attente du signal indiquant que le paquet a été lu par le display
		cout << "COPRO2 : Display a bien recu le message" << endl;
		
	}
}
