#include "copro3_adapt_slave.h"

copro3_adapt_slave::~copro3_adapt_slave()
{
	//A COMPLETER
}

void copro3_adapt_slave::access_time()
{
	//A COMPLETER
}

simple_bus_status copro3_adapt_slave::read(int *data, unsigned int address)
{
	//DO NOTHING (UNIMPLEMENTED)
	return SIMPLE_BUS_ERROR;
}
simple_bus_status copro3_adapt_slave::write(int *data, unsigned int address)
{
	return SIMPLE_BUS_OK;
}
void copro3_adapt_slave::dispatch()
{
	//A COMPLETER
}
unsigned int  copro3_adapt_slave::start_address() const
{
	return m_start_address;
}
unsigned int  copro3_adapt_slave::end_address() const
{
	return m_end_address;
}
void copro3_adapt_slave::pkt_send3(void){
	//A COMPLETER
}
