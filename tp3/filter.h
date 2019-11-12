///////////////////////////////////////////////////////////////////////////////
//
// Filename : filter.h
//
// Creation date : Tue Nov 12 16:03:56 EST 2019
//
///////////////////////////////////////////////////////////////////////////////
#ifndef FILTER_H
#define FILTER_H

#include "ApplicationDefinitions.h"
#include "PlatformDefinitions.h"
#include "SpaceBaseModule.h"

#include "systemc"

class filter: public SpaceBaseModule {
	public:
		
		SC_HAS_PROCESS(filter);
		
		filter(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose);
	   	
		void thread();
};

#endif
