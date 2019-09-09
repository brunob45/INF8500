#include "display.h"
#include "packet.h"

// SC_THREAD(display_message)
// sensitive_pos(msg_valid)
void display::display_message( void )
{
	while(true)
	{
		cout << "DISPLAY : Attente d'un paquet a afficher" << endl;
		wait(msg_valid.posedge_event()); // attente posedge de paquet pret
		cout << "DISPLAY : recuperation paquet a afficher" << endl;
		display_ready = false; //on met display_ready à false pour générer un posedge
		pkt = *input_packet;
		cout << input_message << endl << pkt << endl ; //Affichage du paquet
		cout << "DISPLAY : Envoi de la réponse au coprocesseur" << endl;
		wait(50, SC_NS); //Attente pour que le posedge_event soit pris en compte
		display_ready = true;
		
	}
}
