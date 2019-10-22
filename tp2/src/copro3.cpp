
#include "copro3.h"
#include <string.h>


void copro3::pkt_display()
{
	while (true)
	{
		msg_valid = false; //Indique qu'il n'y a plus de paquets valides pour display et qu'il faut recommencer un cycle
		
		//Recupération du paquet généré par l'interconnexion
		ack = false; 
		cout << "COPRO3 : Attente paquet pret" << endl;
		wait(ready.posedge_event()); // Attendre ready == true
		cout << "COPRO3 : paquet pret" << endl;
		cout << "COPRO3 : Recuperation du paquet" << endl;
		pkt = *packet_in;
		cout << "COPRO3 : Acquittement" << endl;
		ack = true;

		cBubbleSort<unsigned int> cb(&pkt.getPacket()[3], 16);
		std::cout<<"Donnees avant tri: "<<std::endl;
		cb.print();
		cb.doSort(pkt.getDir());
		//  std::cout<<"Ascend(1)*/Desc(0)* Order "<<direction<<std::endl;
		std::cout<<"Donnees apres tri: "<<std::endl;
		cb.print();
		unsigned int* ptr_int_r;// = cb.get_pointer_to_sorted_array();
		ptr_int_r = cb.get_pointer_to_sorted_array();

		pkt.putPacket(ptr_int_r);

		
		// Envoi du paquet au display
		msg_valid = true; //Indique qu'un nouveau paquet est disponible pour le display
		char message[100];
		output_pkt = &pkt;
		strcpy( message, "DISPLAY : Paquet en Provenance : " );
		strcat( message, name() );
		output_msg = message;

		// Indiquer au module display qu'il faut afficher un message
		cout << "COPRO3 : Envoi au display" << endl;
		cout << "COPRO3 : attente du display ready" << endl;
		wait(display_ready.value_changed_event()); // attente du signal indiquant que le paquet a été lu par le display
		cout << "COPRO3 : Display a bien recu le message" << endl;

                // On retourne le résultat a l adapteur de copro 3
                packet_out.write(&pkt);

	}
}
