#ifndef COPRO2_ADAPT_SLAVE_H
#define COPRO2_ADAPT_SLAVE_H

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_slave_if.h"
#include "packet.h"

class copro2_adapt_slave
	: public simple_bus_slave_if,
	public sc_module
{
public:
	SC_HAS_PROCESS(copro2_adapt_slave);
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	sc_in_clk clock;

	// La fifo paquet a destination du copro2 
	sc_fifo_out<Packet*> fifo_out;

	/* *******************************************************************
	// MODULE METHODS
	******************************************************************** */
	// Attente du communicateur prêt (METHOD)
	void access_time(void); 
	// thread de dispatch réveillée lorsqu'un paquet est prêt (THREAD)
	void dispatch(void);
	// Slave Interface (Méthodes de l'interface à implémenter)
	simple_bus_status read(int *data, unsigned int address);
	simple_bus_status write(int *data, unsigned int address);
	bool direct_read(int *data, unsigned int address) override { return read(data, address) == SIMPLE_BUS_OK; }
	bool direct_write(int *data, unsigned int address) override { return write(data, address) == SIMPLE_BUS_OK; }
	unsigned int start_address() const;
	unsigned int end_address() const;

	void pkt_send2(void);


	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	copro2_adapt_slave(sc_module_name name_
		, unsigned int start_address
		, unsigned int end_address
		,unsigned int nr_wait_states)
		: sc_module(name_)
		, m_start_address(start_address)
		, m_end_address(end_address)
		, m_nr_wait_states(nr_wait_states)
		, packet_dispatched(true)
		, m_wait_count(-1)
	{
		SC_THREAD(dispatch);

		SC_METHOD(access_time);
		dont_initialize();
		sensitive << clock.pos();
		
    	sc_assert(m_start_address <= m_end_address);
		sc_assert((m_end_address-m_start_address+1)%4 == 0);
    	unsigned int size = (m_end_address-m_start_address+1)/4;
    	MEM = new unsigned [size];
    	for (unsigned int i = 0; i < size; ++i)
      		MEM[i] = 0;

	}
	/* *******************************************************************
	// MODULE DESTRUCTOR
	******************************************************************** */
	~copro2_adapt_slave();
	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
private:
	sc_event start_dispatch;
	bool packet_dispatched;
	Packet *packet;
	unsigned int *MEM;
	unsigned int m_start_address;
	unsigned int m_end_address;
	int m_wait_count;
	unsigned int m_nr_wait_states;
	unsigned int last_address;
};

#endif