#ifndef COPRO1_ADAPT_SLAVE_H
#define COPRO1_ADAPT_SLAVE_H

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_slave_if.h"
#include "packet.h"

class copro1_adapt_slave
	: public simple_bus_slave_if,
	public sc_module
{
public:
	SC_HAS_PROCESS(copro1_adapt_slave);
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */
	sc_in_clk clock;

	// Le paquet a destination du copro1
	sc_out<Packet*> packet_out;
	// Sortie a destination du copro1  et indiquant qu'il y a un paquet à traiter
	sc_out<bool> ready;
	// Signal provenant du copro1 et indiquant que le paquet a été traité
	sc_in<bool> ack;
	

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

	void pkt_send1(void);


	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	copro1_adapt_slave(sc_module_name name_
		, unsigned int start_address
		, unsigned int end_address
		, unsigned int nr_wait_states)
		: sc_module(name_)
		, m_start_address(start_address)
		, m_end_address(end_address)
		, m_nr_wait_states(nr_wait_states)
		, packet_dispatched(1)
		, m_wait_count(-1)
	{
		SC_THREAD(dispatch);
		//A COMPLETER
		SC_METHOD(access_time);
		dont_initialize();
		//A COMPLETER


	}
	/* *******************************************************************
	// MODULE DESTRUCTOR
	******************************************************************** */
	~copro1_adapt_slave();
	/* *******************************************************************
	// LOCAL VARIABLES
	******************************************************************** */
private:
	sc_event start_dispatch;
	int packet_dispatched;
	Packet *packet;
	unsigned int *MEM;
	unsigned int m_start_address;
	unsigned int m_end_address;
	int m_wait_count;
	unsigned int m_nr_wait_states;
};

#endif