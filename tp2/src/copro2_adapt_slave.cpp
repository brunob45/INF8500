#include "copro2_adapt_slave.h"

copro2_adapt_slave::~copro2_adapt_slave()
{
	if (MEM) delete [] MEM;
  	MEM = (unsigned int *)0;
}

void copro2_adapt_slave::wait_loop()
{
	if (m_wait_count >= 0) m_wait_count--;
}

simple_bus_status copro2_adapt_slave::read(int *data, unsigned int address)
{
	//DO NOTHING (UNIMPLEMENTED)
	return SIMPLE_BUS_ERROR;
}

simple_bus_status copro2_adapt_slave::write(int *data, unsigned int address)
{
	static int cpt_passage = 1;

  if (m_wait_count < 0) {
      
		m_wait_count = m_nr_wait_states;
		return SIMPLE_BUS_WAIT;
  }
  if (m_wait_count == 0) {
		
		MEM[(address - m_start_address)/4] = *data;
		
		if (cpt_passage == 19) {
			// Packet reçu au complet
			cpt_passage = 1;
			start_dispatch.notify();
		}
		else {
			if (cpt_passage == 1)
				m_current_packet_start_address = address;
			
			cpt_passage++;
		}
		
		return SIMPLE_BUS_OK;
	}

  return SIMPLE_BUS_WAIT;

}

void copro2_adapt_slave::dispatch()
{
	while(1) 
	{	
		wait(); // Attente évènement start_dispatch
		packet = new Packet(&MEM[((m_current_packet_start_address - m_start_address) / 4)]);
		pkt_send2();
	}
}

unsigned int  copro2_adapt_slave::start_address() const
{
	return m_start_address;
}

unsigned int  copro2_adapt_slave::end_address() const
{
	return m_end_address;
}

void copro2_adapt_slave::pkt_send2(void)
{
	packet_out.write(packet);
	valid.write(true);
	wait(next.posedge_event());
	valid.write(false);	
}

void copro2_adapt_slave::to_monitor(void)
{
	static bool lock = false;
	static unsigned priority = 10;

	const unsigned int addr = 768;
	const unsigned int packet_size = 19;

	simple_bus_status status;

	while(1)
	{
		cout << "COPRO2_ADAPT : Attente paquet trie" << endl;
		pkt = *packet_in.read(); // Attendre la lecture bloquante
		cout << "COPRO2_ADAPT : Recuperation du paquet trie" << endl;
		// write du paquet au moniteur
		cout << "COPRO2_ADAPT : envoi du packet sur le bus" << endl;
		status = bus_port->burst_write(priority, (int*)pkt.getPacket(), addr, packet_size, lock);

		if (status == SIMPLE_BUS_ERROR) {
			sb_fprintf(stdout, "%s %s : blocking-write failed at address %x\n",
					sc_time_stamp().to_string().c_str(), name(), addr);
		}
	}
}

// Ajout Julien, sinon erreur à la compilation (comme dans l'exemple) :

bool copro2_adapt_slave::direct_read(int *data, unsigned int address)
{
  return (read(data, address) == SIMPLE_BUS_OK);
}

bool copro2_adapt_slave::direct_write(int *data, unsigned int address)
{
  return (write(data, address) == SIMPLE_BUS_OK);
}
