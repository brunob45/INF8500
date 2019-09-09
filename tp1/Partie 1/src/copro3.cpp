
#include "copro3.h"
#include <string.h>


void copro3::pkt_display()
{
	while (true)
	{
		msg_valid = false; //Indique qu'il n'y a plus de paquets valides et qu'il faut recommencer un cycle
		
		//Recupération du paquet généré par l'interconnexion
		ack = false;
		cout << "COPRO3 : Attente paquet pret" << endl;
		wait(buffer_in.value_changed_event()); //sync ready
		cout << "COPRO3 : paquet pret" << endl;
		cout << "COPRO3 : Recuperation du paquet" << endl;
		pkt = *buffer_in.read(); //Lecture
		cout << "COPRO3 : Acquittement" << endl;
		ack = true;
		
		// Envoi du paquet au display
		msg_valid = true; //Indique qu'un nouveau paquet est disponible pour le display
		char message[100];
		output_pkt = &pkt;
		strcpy( message, "DISPLAY3 : Paquet en Provenance : " );
		strcat( message, name() );
		output_msg = message;

		// Indiquer au module display qu'il faut afficher un message
		cout << "COPRO3 : Envoi au display" << endl;
		cout << "COPRO3 : attente du display ready" << endl;
		wait(display_ready.value_changed_event()); // attente du signal indiquant que le paquet a été lu par le display
		cout << "COPRO3 : Display a bien recu le message" << endl;
		

	}
}
