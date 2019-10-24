#include <systemc.h>
#include "packet_gen.h"
#include "my_random_obj.h"
#include "input_coverage.h"

// SC_THREAD(generate)
// sensitive(next_packet)
void packet_gen::generate( void )
{  
	int i=0;

	crave::init("crave.cfg");
	my_rand_obj obj;
	input_coverage obj2;
	TestBase tb;

	//while (i < 3) {
	while (obj2.get_inst_coverage() < 100) {  // le pourcentage de couverture que l'on veut obtenir : 100% sur toutes les contraintes + toutes les combinaisons possibles (copro, sort_order, methode_gen) 

		cout << "----------------------- GEN : Nouveau paquet --------------------------- " << endl;
		
		packet_ready = false;
		cout << "GEN : attente du bus pret" << endl;
		wait(); // Attendre l'assertion de next_packet
		
		wait(50, SC_NS);
		
    	CHECK(obj.next()); // Génération des attributs aléatoires du prochain paquet
    	tb.chk_testcase(obj.getTestCase());
		obj2.sample(obj.copro, obj.sort_dir, obj.data_order, obj.address); //indiquer à la couverture l'échantillon testé

		// Générer un nouveau paquet et l'envoyer au coprocesseur
		// dont le numéro a été généré aléatoirement
		pkt = new Packet(obj.address, tb.direction, (unsigned int*)tb.p);
		//affichage du paquet envoyé
		cout << "GEN : Un paquet a ete envoye a l'adresse 0x" << hex << obj.address << endl;
		//cout << *pkt;
		packet_out = pkt;
		cout << "GEN : Envoi du paquet au bus" << endl;
		packet_ready = true;
		
		cout << "GEN : Attente paquet recu" << endl;
		wait(); // Attendre la désassertion de next_packet
		packet_ready = false;
		
		delete pkt;
		i++;

		//Attendre le signal de monitor pour générer un nouveau paquet
		wait(monitor_next.value_changed_event());
		cout << "GEN : Signal recu de Monitor" << endl;

		std::cout << "coverage:"<<obj2.get_inst_coverage()<<std::endl;

	}


	cout << "----------------------- GEN : Fin de la generation --------------------------- " << endl;

	fc4sc::global::coverage_save("coverage_results.xml");
	std::cout << "nombre d'execution requise pour 100%: " << std::dec << (unsigned)obj2.nb_de_cov << std::endl;
}
