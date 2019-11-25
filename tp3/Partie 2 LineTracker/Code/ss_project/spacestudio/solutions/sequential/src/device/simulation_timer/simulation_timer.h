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
#ifndef SIMULATION_TIMER_H_
#define SIMULATION_TIMER_H_

#include "SpaceBaseDevice.h"

#include "systemc"

class simulation_timer: public SpaceBaseDevice {
	public:

		simulation_timer(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, bool verbose);
		~simulation_timer() { }
		
		virtual void SlaveRead(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
		virtual void SlaveWrite(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};

#endif // SIMULATION_TIMER_H_
