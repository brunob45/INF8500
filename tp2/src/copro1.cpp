
#include "copro1.h"
#include <string.h>


void copro1::pkt_display()
{
	while(true)
	{
		msg_valid = false; //Indique qu'il n'y a plus de paquets valides pour display et qu'il faut recommencer un cycle
		
		//Recup�ration du paquet g�n�r� par l'interconnexion
		ack = false; 
		cout << "COPRO1 : Attente paquet pret" << endl;
		wait(ready.posedge_event()); // Attendre ready == true
		cout << "COPRO1 : paquet pret" << endl;
		cout << "COPRO1 : Recuperation du paquet" << endl;
		pkt = *packet_in;
		cout << "COPRO1 : Acquittement" << endl;
		ack = true;


  unsigned int* ptr_int = pkt.getPacket();
  unsigned  int iarray[16];
  for(int i = 0;i<16;i++)
   {
     iarray[i] = ptr_int[i+3]; //*(ptr_int+i*sizeof(unsigned int);
   };

  cBubbleSort<unsigned int> cb(iarray, 16);
  std::cout<<"Donnees avant tri: "<<std::endl;
  cb.print();
  cb.doSort(true);
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
		cout << "COPRO1 : Envoi au display" << endl;
		cout << "COPRO1 : attente du display ready" << endl;
		wait(display_ready.value_changed_event()); // attente du signal indiquant que le paquet a �t� lu par le display
		cout << "COPRO1 : Display a bien recu le message" << endl;

		// On retourne le résultat a l adapteur de copro 1
		packet_out.write(&pkt);

	}
}
