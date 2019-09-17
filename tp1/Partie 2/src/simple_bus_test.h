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


SC_MODULE(simple_bus_test)
{

	Packet packet_init1, packet_init2;
	char display_buffer[1024];

	// channels
	sc_clock C1;

	// Signaux entre Gen et Routeur
	sc_signal<Packet*> packet_GR;
	sc_signal<bool> packet_ready;
	sc_signal<bool> next_packet;

	// Signaux entre adaptateur de coprocesseur 1 et coprocesseur 1
	sc_signal<Packet*> packet_DS;
	sc_signal<bool> ack_DS;
	sc_signal<bool> ready_DS;

	// Signaux entre adaptateur de coprocesseur 2 et coprocesseur 2
	sc_fifo<Packet*> packet_DS2;

	// Signaux entre adaptateur de coprocesseur 3 et coprocesseur 3
	sc_buffer<Packet*> packet_DS3;
	sc_signal<bool> ack_DS3;

	// Signaux entre Display et coprocesseurs
	sc_signal<bool, sc_core::SC_MANY_WRITERS> msg_valid;
	sc_signal<char*, sc_core::SC_MANY_WRITERS> display_message;
	sc_signal<Packet*, sc_core::SC_MANY_WRITERS> display_packet;
	sc_signal<bool> display_ready;



	// module instances
	packet_gen						   *gen;
	packet_gen_adapt				   *packet_gen_adapt;
	simple_bus					       *bus;
	simple_bus_arbiter                 *arbiter;
	copro1							   *copro1;
	copro1_adapt					   *copro1_adapt;
	copro2                             *copro2;
	copro2_adapt					   *copro2_adapt;
	copro3                             *copro3;
	copro3_adapt					   *copro3_adapt;	
	display							   *dply;

	//A COMPLETER


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
		copro1 = new station("copro1");
		dply = new display("display");
		copro1_adapt = new copro1_adapt("copro1_adapt");
		packet_gen_adapt = new packet_gen_adapt("packet_gen_adapt");
		copro2 = new copro2("copro2");
		copro2_adapt = new copro2_adapt("copro2_adapt");
		copro3 = new copro3("copro3");
		copro3_adapt = new copro3_adapt("copro3_adapt");

		// connect instances
		
		bus->clock(C1);
		bus->arbiter_port(*arbiter);

		gen->packet_ready(packet_ready);
		gen->next_packet(next_packet);
		gen->packet_out(packet_GR);

		copro1->packet_out(packet_DS);
		copro1->ready(ready_DS);
		copro1->ack(ack_DS);
		copro1->msg_valid(msg_valid);
		copro1->output_msg(display_message);
		copro1->output_pkt(display_packet);
		copro1->display_ready(display_ready);
		
		dply->msg_valid(msg_valid);
		dply->input_message(display_message);
		dply->input_packet(display_packet);
		dply->display_ready(display_ready);

		copro1_adapt->packet_in(packet_DS);
		copro1_adapt->ack(ack_DS);
		copro1_adapt->ready(ready_DS);

		packet_gen_adapt->packet_ready(packet_ready);
		packet_gen_adapt->next_packet(next_packet);
		packet_gen_adapt->packet_in(packet_GR);

		//A COMPLETER

	}

	// destructor
	~simple_bus_test()
	{
		if (bus) { delete bus; bus = 0; }
		if (arbiter) { delete arbiter; arbiter = 0; }
		if(dply) { delete dply; dply = 0; }
		if (gen) { delete gen; gen = 0; }
		//A COMPLETER
		
	}

}; // end class simple_bus_test

#endif
