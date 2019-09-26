#ifndef COPRO2_ADAPT_SLAVE_H
#define COPRO2_ADAPT_SLAVE_H

#include <systemc.h>

#include "copro_adapt_slave.h"
#include "packet.h"

class copro2_adapt_slave
	: public copro_adapt_slave
{
public:
	/* *******************************************************************
	// MODULE PORTS
	******************************************************************** */

	// La fifo paquet a destination du copro2 
	sc_fifo_out<Packet*> fifo_out;
	
	virtual void pkt_send(void);

	/* *******************************************************************
	// MODULE CONSTRUCTOR
	******************************************************************** */
	copro2_adapt_slave(sc_module_name name_
		, unsigned int start_address
		, unsigned int end_address
		, unsigned int nr_wait_states)
		: copro_adapt_slave(name_, start_address, end_address, nr_wait_states)
	{
	}
	/* *******************************************************************
	// MODULE DESTRUCTOR
	******************************************************************** */
	virtual ~copro2_adapt_slave() {  }
};

#endif