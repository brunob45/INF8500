#include "copro3_adapt_slave.h"


void copro3_adapt_slave::pkt_send(void)
{
	cout << "COPRO3 send" << endl;
	buffer_out.write(packet);
	wait(ack.posedge_event()); // Attendre ack == true
	cout << "COPRO3 send done" << endl;
}
