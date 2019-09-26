#ifndef COPRO1_ADAPT_SLAVE_H
#define COPRO1_ADAPT_SLAVE_H

#include <systemc.h>

#include "copro_adapt_slave.h"
#include "packet.h"

class copro1_adapt_slave
	: public copro_adapt_slave
{
public:
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */

	// Le paquet a destination du copro1
	sc_out<Packet*> packet_out;
	// Sortie a destination du copro1  et indiquant qu'il y a un paquet à traiter
	sc_out<bool> ready;
	// Signal provenant du copro1 et indiquant que le paquet a été traité
	sc_in<bool> ack;
	
	virtual void pkt_send(void);

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	copro1_adapt_slave(sc_module_name name_
		, unsigned int start_address
		, unsigned int end_address
		, unsigned int nr_wait_states)
		: copro_adapt_slave(name_, start_address, end_address, nr_wait_states)
	{
	}
	/* *******************************************************************
	// MODULE DESTRUCTOR
	******************************************************************** */
	virtual ~copro1_adapt_slave() {  }
};

#endif