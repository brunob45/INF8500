#include "copro1_adapt_slave.h"


void copro1_adapt_slave::pkt_send(void)
{
	cout << "COPRO1 send" << endl;
	packet_out = packet;
	ready = true;
	wait(ack.posedge_event()); // Attendre ack == true
	ready = false;
	cout << "COPRO1 send done" << endl;
}
