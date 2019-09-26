#ifndef COPRO_ADAPT_SLAVE_H
#define COPRO_ADAPT_SLAVE_H

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_slave_if.h"
#include "packet.h"

class copro_adapt_slave
	: public simple_bus_slave_if,
	public sc_module
{
public:
	SC_HAS_PROCESS(copro_adapt_slave);

	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	sc_in_clk clock;
	

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Attente du communicateur pr�t (METHOD)
	void access_time(void); 
	// thread de dispatch r�veill�e lorsqu'un paquet est pr�t (THREAD)
	void dispatch(void);
	// Slave Interface (M�thodes de l'interface � impl�menter)
	simple_bus_status read(int *data, unsigned int address);
	simple_bus_status write(int *data, unsigned int address);
	bool direct_read(int *data, unsigned int address) override { return read(data, address) == SIMPLE_BUS_OK; }
	bool direct_write(int *data, unsigned int address) override { return write(data, address) == SIMPLE_BUS_OK; }
	unsigned int start_address() const;
	unsigned int end_address() const;

	virtual void pkt_send(void) = 0;


	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	copro_adapt_slave(sc_module_name name_
		, unsigned int start_address
		, unsigned int end_address
		, unsigned int nr_wait_states)
		: sc_module(name_)
		, m_start_address(start_address)
		, m_end_address(end_address)
		, m_nr_wait_states(nr_wait_states)
		, packet_dispatched(1)
		, m_wait_count(-1)
        , m_current_start_address(start_address)
	{
    	sc_assert(m_start_address <= m_end_address);
		sc_assert((m_end_address-m_start_address+1)%4 == 0);

		SC_THREAD(dispatch);
		sensitive << start_dispatch;
	}
	/* *******************************************************************
	// MODULE DESTRUCTOR
	******************************************************************** */
	virtual ~copro_adapt_slave();
	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
protected:
	Packet *packet;

private:
	sc_event start_dispatch;
	int packet_dispatched;
	unsigned int MEM[24];
	unsigned int m_start_address;
	unsigned int m_end_address;
	int m_wait_count;
	unsigned int m_nr_wait_states;
	unsigned int m_current_start_address;
};

#endif