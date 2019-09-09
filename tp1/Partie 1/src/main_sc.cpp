#include <systemc.h>

// Définitions globales
#include "globals.h"

// Définitions des modules
#include "display.h"
#include "packet_gen.h"
#include "copro1.h"
#include "copro2.h"
#include "copro3.h"
#include "interconnexion.h"


// Définitions des classes
#include "packet.h"

// Fonction principale
int sc_main( int argc, char* argv[] )
{
	
	Packet packet_init1,packet_init2;
	char display_buffer[1024];
	display_buffer[0] = 0;
	
	// Connexions entre packet_gen et interconnexion 
	sc_signal<Packet*> packet_GR;
	sc_signal<bool> packet_ready;
	sc_signal<bool> next_packet;

	// Connexions entre interconnexion et copro1 
	sc_signal<Packet*> packet_RS;
	sc_signal<bool> ack_RS;
	sc_signal<bool> ready_RS;

	// Connexions entre interconnexion et copro2 
	sc_fifo<Packet*> packet_RS2;

	// Connexions entre interconnexion et copro3 
	sc_buffer<Packet*> packet_RS3;
	sc_signal<bool> ack_RS3;

	// Connexions entre Display et coprocesseurs 
	sc_signal<bool,sc_core::SC_MANY_WRITERS> msg_valid;
	sc_signal<char*, sc_core::SC_MANY_WRITERS> display_message;
	sc_signal<Packet*, sc_core::SC_MANY_WRITERS> display_packet;
	sc_signal<bool> display_ready;


	// Espaces tampons (buffers) entre les modules
	display_packet = &packet_init2;
	display_message = display_buffer;

	//------------------------------------------
	// Instanciation
	//------------------------------------------

	packet_gen the_pkt_gen("the_pkt_gen");
	interconnexion interconnexion ("interconnexion");
	copro1 copro1("Coprocesseur1");
	copro2 copro2("Coprocesseur2");
	copro3 copro3("Coprocesseur3");
	display monitor("monitor");

	//------------------------------------------
	// Connexions entre les ports
	//------------------------------------------

	the_pkt_gen(next_packet, packet_ready, packet_GR);
	interconnexion(packet_GR, packet_ready, next_packet, packet_RS, ack_RS, ready_RS, packet_RS2, packet_RS3, ack_RS3);
	copro1(packet_RS, ready_RS, ack_RS, msg_valid, display_message, display_packet, display_ready);
	copro2(packet_RS2, msg_valid, display_message, display_packet, display_ready);
	copro3(packet_RS3, ack_RS3, msg_valid, display_message, display_packet, display_ready);
	monitor(msg_valid, display_message, display_packet, display_ready);

	//------------------------------------------
	// Démarrer la simulation
	//------------------------------------------
      
	sc_start(100, SC_SEC);


	printf("fin de la simulation");
	
	return 0;
}
