#ifndef COPRO3_ADAPT_SLAVE_H
#define COPRO3_ADAPT_SLAVE_H

#include <systemc.h>

#include "copro_adapt_slave.h"
#include "simple_bus_slave_if.h"
#include "packet.h"

class copro3_adapt_slave
    : public copro_adapt_slave
{
public:
    /* *******************************************************************
    // MODULE PORTS
    ******************************************************************** */

    // Le paquet a destination du copro3
    sc_out<Packet*> buffer_out;

    // Signal provenant du copro3 et indiquant que le paquet a été traité
    sc_in<bool> ack;

    virtual void pkt_send(void);

    /* *******************************************************************
    // MODULE CONSTRUCTOR
    ******************************************************************** */
    copro3_adapt_slave(sc_module_name name_
		, unsigned int start_address
		, unsigned int end_address
		, unsigned int nr_wait_states)
		: copro_adapt_slave(name_, start_address, end_address, nr_wait_states)
	{
	}
    /* *******************************************************************
    // MODULE DESTRUCTOR
    ******************************************************************** */
    virtual ~copro3_adapt_slave() {  }
};

#endif