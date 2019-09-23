#include <systemc.h>
#include "packet_gen.h"

// SC_THREAD(generate)
// sensitive(next_packet)
void packet_gen::generate( void )
{
	srand(time(0));
	for (int i = 0; i < 8; i++)
	{
 		packet_ready = false;
		cout << "GEN : attente du bus pret" << endl;
		wait(); // Attendre l'assertion de next_packet
		
		wait(50, SC_NS);

		// Générer un nombre aléatoire entre 0 et 255.
		int address_nb = (rand() % (72/4))*4;
		int copro_nb =   2;//(rand() % 3);
		int nba = address_nb + copro_nb * 96;
		
		// Générer un nouveau paquet et l'envoyer au coprocesseur
		// dont le numéro a été généré aléatoirement
		Packet pkt(nba, 1 + i);

		//affichage du paquet envoyé
		cout << "GEN : Un paquet a ete envoye e l'adresse 0x" << hex << nba << endl;
		packet_out = &pkt;
		cout << "GEN : Envoi du paquet au bus" << endl;
		packet_ready = true;
		
		cout << "GEN : Attente paquet recu" << endl;
		wait(); // Attendre la désassertion de next_packet
		packet_ready = false;
	}
	
}
