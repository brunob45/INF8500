#include "packet_gen_adapt_master.h"
#include "globals.h"
#include <string.h>

void packet_gen_adapt_master::pkt_dispatch(void){

	unsigned int addr;
	simple_bus_status status;

	// Rajout Julien :
	unsigned int packet_size = 19;

	while (true)
	{
		// Réception d'un packet du générateur :
		next.write(true);
		cout << "PKT_GEN_ADAPTER : attente packet disponible" << endl;
		wait(ready.posedge_event());
		next.write(false);
		pkt = *packet_in.read();
		addr = pkt.getAddress();

		// Attente d'un front montant d'horloge
		wait();

		cout << "PKT_GEN_ADAPTER : envoi du packet sur le bus" << endl;
		status = bus_port->burst_write(m_unique_priority, (int*)pkt.getPacket(), addr, packet_size, m_lock);

		if (status == SIMPLE_BUS_ERROR) {
			sb_fprintf(stdout, "%s %s : blocking-write failed at address %x\n",
		  					sc_time_stamp().to_string().c_str(), name(), m_address);
		}

		wait(m_timeout, SC_NS);
	}
}
