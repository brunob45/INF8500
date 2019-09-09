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
		cout << "GEN : attente du bus pret" << endl;
		wait(); // Attendre l'assertion de next_packet
		
		wait(50, SC_NS);

		// G�n�rer un nombre al�atoire entre 0 et 255.
		nba =  (rand() % 63)*4;
		
		// G�n�rer un nouveau paquet et l'envoyer au coprocesseur
		// dont le num�ro a �t� g�n�r� al�atoirement
		pkt = new Packet(nba, 1 + i);
		//affichage du paquet envoy�
		cout << "GEN : Un paquet a ete envoye e l'adresse 0x" << hex << nba << endl;
		cout << *pkt;
		packet_out = pkt;
		cout << "GEN : Envoi du paquet au bus" << endl;
		packet_ready = true;
		
		cout << "GEN : Attente paquet recu" << endl;
		wait(); // Attendre la d�sassertion de next_packet
		packet_ready = false;
		
		delete pkt;
	i++;
	}
	
}
