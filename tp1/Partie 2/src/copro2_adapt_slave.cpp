#include "copro2_adapt_slave.h"


void copro2_adapt_slave::pkt_send(void)
{
	cout << "COPRO2 send" << endl;
	fifo_out.write(packet);
	wait(10, SC_NS);
	cout << "COPRO2 send done" << endl;
}
