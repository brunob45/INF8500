#include "copro2_adapt_slave.h"

copro2_adapt_slave::~copro2_adapt_slave()
{
	//A COMPLETER
}

void copro2_adapt_slave::access_time()
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
	static unsigned int write_cnt = 6;
	// accept a new call if m_wait_count < 0)
	if (m_wait_count < 0)
	{
		m_wait_count = m_nr_wait_states;
		return SIMPLE_BUS_WAIT;
	}
	if (m_wait_count == 0)
	{
		MEM[(address - m_start_address)/4] = *data;
		write_cnt--;
		if(write_cnt <= 0)
		{
			last_address = address;
			write_cnt = 6;
			start_dispatch.notify();
		}
		return SIMPLE_BUS_OK;
    }
  return SIMPLE_BUS_WAIT;

}
void copro2_adapt_slave::dispatch()
{
	while (true)
	{
		//Recupération du paquet
		packet_dispatched = true; 
		cout << "A_COPRO2 : Attente paquet pret" << endl;
		wait(start_dispatch.posedge_event()); // Attendre ready == true
		cout << "A_COPRO2 : paquet pret" << endl;
		cout << "A_COPRO2 : Recuperation du paquet" << endl;
		unsigned* pktmem = packet.getPacket();
		for (int i = 0; i < 6; i++)
			pktmem[i] = MEM[last_address-5+i];
		packet = MEM;
		cout << "A_COPRO2 : Acquittement" << endl;
		packet_dispatched = false;

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
	fifo_out.write(&pkt);
	wait(10, SC_NS);
}
