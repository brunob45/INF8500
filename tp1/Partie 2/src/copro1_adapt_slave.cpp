#include "copro1_adapt_slave.h"

copro1_adapt_slave::~copro1_adapt_slave()
{
	//A COMPLETER
}

void copro1_adapt_slave::access_time()
{
	//A COMPLETER
}

simple_bus_status copro1_adapt_slave::read(int *data, unsigned int address)
{
	//DO NOTHING (UNIMPLEMENTED)
	return SIMPLE_BUS_ERROR;
}
simple_bus_status copro1_adapt_slave::write(int *data, unsigned int address)
{
	packet_out = &data;
	ready_copro1 = true;
	wait(ack_copro1.posedge_event()); // Attendre ack == true
	ready_copro1 = false;
}
void copro1_adapt_slave::dispatch()
{
	//A COMPLETER
}
unsigned int  copro1_adapt_slave::start_address() const
{
	//A COMPLETER
}
unsigned int  copro1_adapt_slave::end_address() const
{
	//A COMPLETER
}
void copro1_adapt_slave::pkt_send1(void)
{
	//A COMPLETER
}
