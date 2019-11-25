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
#ifndef MASTER_H
#define MASTER_H

#include "SpaceBaseModule.h"

#include "systemc"

class master: public SpaceBaseModule {
	public:
		
	    SC_HAS_PROCESS(master);
		
		master(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose);
		void write_2_register(unsigned int file_id, unsigned int offset, unsigned long value);

	    void thread(void);
};

#endif
