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
#include "simulation_timer.h"
#include "PlatformDefinitions.h"
#include "ApplicationDefinitions.h"
#include "SpaceDisplay.h"

#include "tlm"

simulation_timer::simulation_timer(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, bool verbose)
:SpaceBaseDevice(name, sc_core::sc_time(period, unit), id, verbose) { }

void simulation_timer::SlaveRead(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
	unsigned long offset = GetRelativeAddress(trans);
	unsigned long* data = (unsigned long*)trans.get_data_ptr();
	unsigned long long time = sc_core::sc_time_stamp().value();

	if(offset == SIMULATION_TIME_LOW) {
		*data = (unsigned long)time;
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	} else if(offset == SIMULATION_TIME_HIGH) {
		*data = (unsigned long)(time >> 32);
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	} else {
		SpacePrint("Error. Simulation timer received an invalid read request to register address %#X\n", offset);
		trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
	}

	delay += GetClockPeriod(); // Simulate device access delay
}

void simulation_timer::SlaveWrite(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
	trans.set_response_status(tlm::TLM_OK_RESPONSE);	// don't forget to set the TLM 2.0 response status
	delay += GetClockPeriod();	// simulate device access delay
}
