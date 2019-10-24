/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

  *****************************************************************************/

/*****************************************************************************

  simple_bus_test.h : The test bench.

  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11

  *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

  Name, Affiliation, Date:
  Description of Modification:

  *****************************************************************************/

#ifndef __simple_bus_test_h
#define __simple_bus_test_h

#include <systemc.h>

#include "simple_bus.h"
#include "simple_bus_arbiter.h"
#include "packet.h"
#include "display.h"

#include "packet_gen.h"
#include "packet_gen_adapt_master.h"
#include "copro1.h"
#include "copro1_adapt_slave.h"
#include "copro2.h"
#include "copro2_adapt_slave.h"
#include "copro3.h"
#include "copro3_adapt_slave.h"
#include "Monitor.h"


SC_MODULE(simple_bus_test)
{

	Packet packet_init1, packet_init2;
	char display_buffer[1024];

	// channels
	sc_clock C1;

	// Signaux entre Gen et Routeur
	sc_signal<bool> next_GR;
	sc_signal<bool> ready_GR;
	sc_signal<Packet*> packet_GR;

	// Signaux entre adaptateur de coprocesseur 1 et coprocesseur 1
	sc_signal<bool> ready_CP1;
	sc_signal<bool> ack_CP1;
	sc_signal<Packet*> packet_CP1;

	// Signaux entre adaptateur de coprocesseur 2 et coprocesseur 2
	sc_signal<bool> ready_CP2;
	sc_signal<bool> ack_CP2;
	sc_signal<Packet*> packet_CP2;

	// Signaux entre adaptateur de coprocesseur 3 et coprocesseur 3
	sc_signal<bool> ready_CP3;
	sc_signal<bool> ack_CP3;
	sc_signal<Packet*> packet_CP3;

	// Signaux entre Display et coprocesseurs
	sc_signal<bool, sc_core::SC_MANY_WRITERS> msg_valid;
	sc_signal<char*, sc_core::SC_MANY_WRITERS> display_message;
	sc_signal<Packet*, sc_core::SC_MANY_WRITERS> display_packet;
	sc_signal<bool> display_ready;

	// Retour au moniteur
	sc_fifo<Packet*> packet_copro1_adapt_monitor;
	sc_fifo<Packet*> packet_copro2_adapt_monitor;
	sc_fifo<Packet*> packet_copro3_adapt_monitor;

	sc_signal<bool> monitor_ready;



	// module instances
	packet_gen		 *gen;
	packet_gen_adapt_master	 *packet_gen_adapt;
	simple_bus		 *bus;
	simple_bus_arbiter       *arbiter;
	copro1			 *copro_1;
	copro1_adapt_slave	 *copro1_adapt;
	copro2                   *copro_2;
	copro2_adapt_slave	 *copro2_adapt;
	copro3                   *copro_3;
	copro3_adapt_slave	 *copro3_adapt;	
	display			 *dply;
	Monitor			*monitor;


	// constructor
	SC_CTOR(simple_bus_test)
		: C1("C1")
	{
		// Espaces tampons (buffers) entre les modules
		display_packet = &packet_init2;
		display_message = display_buffer;
		display_buffer[0] = 0;

		// create instances
		//bus = new simple_bus("bus",true); // verbose output
		//arbiter = new simple_bus_arbiter("arbiter",true); // verbose output
		bus = new simple_bus("bus");
		arbiter = new simple_bus_arbiter("arbiter");
		gen = new packet_gen("packet_gen");
		dply = new display("display");
		monitor = new Monitor("monit");

		packet_gen_adapt = new packet_gen_adapt_master("packet_gen_adapt", 1, 0x00, false, 300);

		copro_1 = new copro1("copro1");
		copro1_adapt = new copro1_adapt_slave("copro1_adapt", 0x00, 0xFF, 1);

		copro_2 = new copro2("copro2");
		copro2_adapt = new copro2_adapt_slave("copro2_adapt", 0x100, 0x1FF, 1);

		copro_3 = new copro3("copro3");
		copro3_adapt = new copro3_adapt_slave("copro3_adapt", 0x200, 0x2FF, 1);


		// connect instances	
		bus->clock(C1);
		bus->arbiter_port(*arbiter);

		gen->packet_ready(ready_GR);
		gen->next_packet(next_GR);
		gen->packet_out(packet_GR);

		dply->msg_valid(msg_valid);
		dply->input_message(display_message);
		dply->input_packet(display_packet);
		dply->display_ready(display_ready);

		packet_gen_adapt->clock(C1);
		packet_gen_adapt->next(next_GR);
		packet_gen_adapt->ready(ready_GR);
		packet_gen_adapt->packet_in(packet_GR);
		packet_gen_adapt->bus_port(*bus);

		copro_1->packet_in(packet_CP1);
		copro_1->ready(ready_CP1);
		copro_1->ack(ack_CP1);
		copro_1->msg_valid(msg_valid);
		copro_1->output_msg(display_message);
		copro_1->output_pkt(display_packet);
		copro_1->display_ready(display_ready);
		copro_1->packet_out(packet_copro1_adapt_monitor);

		copro1_adapt->clock(C1);
		copro1_adapt->packet_out(packet_CP1);
		copro1_adapt->valid(ready_CP1);
		copro1_adapt->next(ack_CP1);
		copro1_adapt->packet_in(packet_copro1_adapt_monitor);

		bus->slave_port(*copro1_adapt);

		copro_2->packet_in(packet_CP2);
		copro_2->ready(ready_CP2);
		copro_2->ack(ack_CP2);
		copro_2->msg_valid(msg_valid);
		copro_2->output_msg(display_message);
		copro_2->output_pkt(display_packet);
		copro_2->display_ready(display_ready);
		copro_2->packet_out(packet_copro2_adapt_monitor);

		copro2_adapt->clock(C1);
		copro2_adapt->packet_out(packet_CP2);
		copro2_adapt->valid(ready_CP2);
		copro2_adapt->next(ack_CP2);
		copro2_adapt->packet_in(packet_copro2_adapt_monitor);
		copro2_adapt->bus_port(*bus);

		bus->slave_port(*copro2_adapt);

		copro_3->packet_in(packet_CP3);
		copro_3->ready(ready_CP3);
		copro_3->ack(ack_CP3);
		copro_3->msg_valid(msg_valid);
		copro_3->output_msg(display_message);
		copro_3->output_pkt(display_packet);
		copro_3->display_ready(display_ready);
		copro_3->packet_out(packet_copro3_adapt_monitor);

		copro3_adapt->clock(C1);
		copro3_adapt->packet_out(packet_CP3);
		copro3_adapt->valid(ready_CP3);
		copro3_adapt->next(ack_CP3);
		copro3_adapt->packet_in(packet_copro3_adapt_monitor);

		bus->slave_port(*copro3_adapt);

		monitor->clock(C1);
		monitor->packet_received(monitor_ready);

		bus->slave_port(*monitor);
	}

	// destructor
	~simple_bus_test()
	{
		if (bus) { delete bus; bus = 0; }
		if (arbiter) { delete arbiter; arbiter = 0; }
		if(dply) { delete dply; dply = 0; }
		if (gen) { delete gen; gen = 0; }
		//A COMPLETER
		if (packet_gen_adapt) { delete packet_gen_adapt; packet_gen_adapt = 0; }
		if (copro1_adapt) { delete copro1_adapt; copro1_adapt = 0; }
		if (copro_1) { delete copro_1; copro_1 = 0; }
		if (copro2_adapt) { delete copro2_adapt; copro2_adapt = 0; }
		if (copro_2) { delete copro_2; copro_2 = 0; }
		if (copro3_adapt) { delete copro3_adapt; copro3_adapt = 0; }
		if (copro_3) { delete copro_3; copro_3 = 0; }
	}

}; // end class simple_bus_test

#endif
