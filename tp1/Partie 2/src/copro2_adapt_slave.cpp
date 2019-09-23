#include "copro2_adapt_slave.h"

copro2_adapt_slave::~copro2_adapt_slave()
{
	//A COMPLETER
}

void copro2_adapt_slave::access_time()
{
	//DO NOTHING (UNIMPLEMENTED)
}

simple_bus_status copro2_adapt_slave::read(int *data, unsigned int address)
{
	//DO NOTHING (UNIMPLEMENTED)
	return SIMPLE_BUS_ERROR;
}
simple_bus_status copro2_adapt_slave::write(int *data, unsigned int address)
{
	static int cpt = 0;
	simple_bus_status status = SIMPLE_BUS_OK;

	MEM[(address - m_start_address)/4] = *data;

	if (cpt == 0)
	{
		status = SIMPLE_BUS_WAIT;
		m_current_start_address = address;
	}

	cpt++;
	if(cpt > 6)
	{
		cpt = 0;
		packet_dispatched = false;
		start_dispatch.notify();
	}

	return status;
}
void copro2_adapt_slave::dispatch()
{
	cout << "COPRO2 dispatch ready" << endl;
	while(1)
	{
		wait(start_dispatch);
		
		unsigned address = (m_current_start_address - m_start_address)/4;

		cout << "COPRO2 dispatch" << endl;

		packet = new Packet(&MEM[address]);
		pkt_send2();
		delete packet;

		packet_dispatched = true;
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
	fifo_out.write(packet);
	wait(10, SC_NS);
}
