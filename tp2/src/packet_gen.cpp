#include <systemc.h>
#include "packet_gen.h"
#include "my_random_obj.h"
#include "input_coverage.h"

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
	input_coverage obj2;
	TestBase tb;

	//while (i < 8)
	while (obj2.get_inst_coverage() < 50) {  // le pourcentage de couverture que l'on veut obtenir --> à calculer 
	{
 		packet_ready = false;
		cout << "GEN : attente du bus pret" << endl;
		wait(); // Attendre l'assertion de next_packet
		
		wait(50, SC_NS);
		
    	CHECK(obj.next()); // Génération des attributs aléatoires du prochain paquet
		obj2.sample(obj.copro_value(), obj.sort_dir_value(), obj.data_order_value()/*, obj.address_value()*/); //indiquer à la couverture l'échantillon testé

		// Générer un nouveau paquet et l'envoyer au coprocesseur
		// dont le numéro a été généré aléatoirement
		pkt = new Packet(obj.address, 1 + i);
		//affichage du paquet envoyé
		cout << "GEN : Un paquet a ete envoye a l'adresse 0x" << hex << nba << endl;
		cout << *pkt;
		packet_out = pkt;
		cout << "GEN : Envoi du paquet au bus" << endl;
		packet_ready = true;
		
		cout << "GEN : Attente paquet recu" << endl;
		wait(); // Attendre la désassertion de next_packet
		packet_ready = false;
		
		delete pkt;
		i++;
	}

	fc4sc::global::coverage_save("coverage_results.xml");
	std::cout << "nombre d'execution requise pour 81.25%: " << obj2.nb_de_cov << std::endl;
	
}
