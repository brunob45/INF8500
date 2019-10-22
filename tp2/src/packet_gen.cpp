#include <systemc.h>
#include "packet_gen.h"
#include "my_random_obj.h"

// SC_THREAD(generate)
// sensitive(next_packet)
void packet_gen::generate( void )
{  
	int i=0;
 	int nba; 
	int copro_numero;
	int offset_copro;

	crave::init("crave.cfg");
	my_rand_obj obj;
	TestBase tb;

	while (i < 8)
	{
 		packet_ready = false;
		cout << "GEN : attente du bus pret" << endl;
		wait(); // Attendre l'assertion de next_packet
		
		wait(50, SC_NS);
		
    	CHECK(obj.next()); // G�n�ration des attributs al�atoires du prochain paquet

		// G�n�rer un nouveau paquet et l'envoyer au coprocesseur
		// dont le num�ro a �t� g�n�r� al�atoirement
		pkt = new Packet(obj.address, 1 + i);
		//affichage du paquet envoy�
		cout << "GEN : Un paquet a ete envoye a l'adresse 0x" << hex << nba << endl;
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
