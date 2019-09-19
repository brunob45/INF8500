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
#include "packet_gen.h"
#include "packet_gen_adapt_master.h"
#include "copro1.h"
#include "copro1_adapt_slave.h"
#include "copro2.h"
#include "copro2_adapt_slave.h"
#include "copro3.h"
#include "copro3_adapt_slave.h"
#include "display.h"


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
    packet_gen						   *p_gen;
    packet_gen_adapt_master			   *p_packet_gen_adapt;
    simple_bus					       *p_bus;
    simple_bus_arbiter                 *p_arbiter;
    copro1							   *p_copro1;
    copro1_adapt_slave				   *p_copro1_adapt;
    copro2                             *p_copro2;
    copro2_adapt_slave				   *p_copro2_adapt;
    copro3                             *p_copro3;
    copro3_adapt_slave				   *p_copro3_adapt;
    display							   *p_dply;

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
        p_bus = new simple_bus("bus");
        p_arbiter = new simple_bus_arbiter("arbiter");
        p_gen = new packet_gen("packet_gen");
        p_packet_gen_adapt = new packet_gen_adapt_master("packet_gen_adapt", 0, 256, false, 2);
        p_copro1_adapt = new copro1_adapt_slave("copro1_adapt", 0x00, 0x0f, 2);
        p_copro1 = new copro1("copro1");
        p_copro2_adapt = new copro2_adapt_slave("copro2_adapt", 0x10, 0x1f, 2);
        p_copro2 = new copro2("copro2");
        p_copro3_adapt = new copro3_adapt_slave("copro3_adapt", 0x20, 0x2f, 2);
        p_copro3 = new copro3("copro3");
        p_dply = new display("display");

        // connect instances
        
        p_bus->clock(C1);
        p_packet_gen_adapt->clock(C1);
        p_copro1_adapt->clock(C1);
        p_copro2_adapt->clock(C1);
        p_copro3_adapt->clock(C1);

        p_bus->arbiter_port(*p_arbiter);

        // connect packet_gen and adapt
        p_gen->packet_ready(packet_ready);
        p_packet_gen_adapt->packet_ready(packet_ready);

        p_gen->next_packet(next_packet);
        p_packet_gen_adapt->next_packet(next_packet);

        p_gen->packet_out(packet_GR);
        p_packet_gen_adapt->packet_in(packet_GR);

        // connect adapt and copro1
        p_copro1_adapt->packet_out(packet_DS);
        p_copro1->packet_in(packet_DS);

        p_copro1_adapt->ack(ack_DS);
        p_copro1->ack(ack_DS);

        p_copro1_adapt->ready(ready_DS);
        p_copro1->ready(ready_DS);

        // connect adapt and copro2
        p_copro2_adapt->fifo_out(packet_DS2);
        p_copro2->fifo_in(packet_DS2);

        // connect adapt and copro3
        p_copro3_adapt->buffer_out(packet_DS3);
        p_copro3->buffer_in(packet_DS3);

        p_copro3_adapt->ack(ack_DS3);
        p_copro3->ack(ack_DS3);

        // connect copros and display
        p_copro1->msg_valid(msg_valid);
        p_copro2->msg_valid(msg_valid);
        p_copro3->msg_valid(msg_valid);
        p_dply->msg_valid(msg_valid);

        p_copro1->output_msg(display_message);
        p_copro2->output_msg(display_message);
        p_copro3->output_msg(display_message);
        p_dply->input_message(display_message);

        p_copro1->output_pkt(display_packet);
        p_copro2->output_pkt(display_packet);
        p_copro3->output_pkt(display_packet);
        p_dply->input_packet(display_packet);

        p_copro1->display_ready(display_ready);
        p_copro2->display_ready(display_ready);
        p_copro3->display_ready(display_ready);
        p_dply->display_ready(display_ready);
    }

    // destructor
    ~simple_bus_test()
    {
        delete p_bus; p_bus = nullptr;
        delete p_arbiter; p_arbiter = nullptr;

        delete p_gen; p_gen = nullptr;
        delete p_packet_gen_adapt; p_copro1_adapt = nullptr;

        delete p_copro1_adapt; p_copro1_adapt = nullptr;
        delete p_copro1; p_copro1 = nullptr;

        delete p_copro2_adapt; p_copro2_adapt = nullptr;
        delete p_copro2; p_copro2 = nullptr;

        delete p_copro3_adapt; p_copro3_adapt = nullptr;
        delete p_copro3; p_copro3 = nullptr;

        delete p_dply; p_dply = nullptr;
    }

}; // end class simple_bus_test

#endif
