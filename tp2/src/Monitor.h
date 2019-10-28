#pragma once

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_slave_if.h"
#include "packet.h"

SC_MODULE(Monitor), public simple_bus_slave_if
{
	SC_HAS_PROCESS(Monitor);

	sc_in_clk clock;
	sc_out<bool> packet_received;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// thread de dispatch réveillée lorsqu'un paquet est prét (THREAD)
	void dispatch(void);
	// Slave Interface (Méthodes de l'interface é implémenter)
	simple_bus_status read(int *data, unsigned int address);
	simple_bus_status write(int *data, unsigned int address);
	unsigned int start_address() const {return 768;}
	unsigned int end_address() const {return 768+(19*4-1);}

	bool direct_read(int *data, unsigned int address) {return (read(data, address) == SIMPLE_BUS_OK);}
	bool direct_write(int *data, unsigned int address) {return (write(data, address) == SIMPLE_BUS_OK);}

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	Monitor(sc_module_name name_)
		: sc_module(name_)
	{
		SC_THREAD(dispatch);
		sensitive << start_check;
	}
	/* *******************************************************************
	// MODULE DESTRUCTOR
	******************************************************************** */
	~Monitor() { /*Nothing*/ }
	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
private:
	Packet packet;
	int m_wait_count;
    sc_event start_check;

};