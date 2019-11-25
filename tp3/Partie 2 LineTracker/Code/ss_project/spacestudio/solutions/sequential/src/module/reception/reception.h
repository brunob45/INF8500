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
#ifndef RECEPTION_H
#define RECEPTION_H

#include "SpaceBaseModule.h"
#include "client_handler.h"

#include "systemc"

class reception: public SpaceBaseModule {
	public:
		
	    SC_HAS_PROCESS(reception);
		
		reception(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose);
		~reception();

	    void thread(void);

	private:
	    // Output FIFO buffers
	    unsigned int m_head_out_buff;
	    unsigned int m_tail_in_buff;
	    client_handler* m_socket;

		void is_output_fifo_full();
};

#endif
