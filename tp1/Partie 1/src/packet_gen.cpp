#include <systemc.h>
#include "packet_gen.h"

// SC_THREAD(generate)
// sensitive(next_packet)
void packet_gen::generate( void )
{
	int i=0;
	int nba; 
	while (i < 8)
	{
 		packet_ready = false;
		cout << "GEN : attente du routeur pret" << endl;
		wait(next_packet.posedge_event()); // Attendre l'assertion de next_packet
		
		wait(50, SC_NS);

		// Générer un nombre aléatoire entre 1 et 3.
		nba = 1 + (rand() % (3));
		
		// Générer un nouveau paquet et l'envoyer à un coprocesseur
		// dont le numéro a été généré aléatoirement
		pkt = new Packet(nba, 1 + i);

		//affichage du paquet envoyé
		cout << "GEN : Un paquet a ete envoye au coprocesseur no #" << nba << endl;
		cout << "GEN : Message du paquet :" << 1+i << endl;
		
		packet_out =pkt;
		cout << "GEN : Envoi du paquet au routeur" << endl;
		packet_ready = true;
		
		cout << "GEN : Attente paquet recu" << endl;
		wait(next_packet.negedge_event()); // Attendre la désassertion de next_packet
		packet_ready = false;
		
		delete pkt;
		i++;
	}
	
}
