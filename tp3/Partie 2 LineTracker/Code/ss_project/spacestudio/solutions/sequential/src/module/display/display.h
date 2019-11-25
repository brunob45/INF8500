///////////////////////////////////////////////////////////////////////////////
///
///         IRT Antoine de Saint-Exupéry - (http://http://www.irt-saintexupery.com/)
///         (c) All Rights Reserved. 2016
///
///         No authorization to modify or use this file for
///         commercial purposes without prior written consentement
///         of its author(s)
///
///////////////////////////////////////////////////////////////////////////////
#ifndef DISPLAY_H
#define DISPLAY_H

#include "ApplicationDefinitions.h"
#include "PlatformDefinitions.h"
#include "SpaceBaseModule.h"
#include "linux_throughput.h"
#include "simtek_throughput.h"

#include "systemc"
#include "client_handler.h"

class display: public SpaceBaseModule {
	public:
		
	    SC_HAS_PROCESS(display);
		
		display(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose);
	   	~display();
	   	
	    void thread(void);
	    void is_input_fifo_empty();

#if THROUGHPUT_MONITORING == 1
#if defined(SPACE_SIMULATION_RELEASE) || defined(SPACE_SIMULATION_MONITORING) || defined(SPACE_SIMULATION_DEBUG)
unsigned long long get_timer_value();
#endif
#endif

	private:
	    unsigned int m_head_in_buff;
	    unsigned int m_tail_out_buff;
	    client_handler* m_socket;

	    void show_lines(unsigned int address, unsigned int width, unsigned int height, unsigned int maximum_nb_lines);
};

#endif
