///////////////////////////////////////////////////////////////////////////////
//
// Filename : filter.h
//
// Creation date : Mon Nov 25 10:23:56 EST 2019
//
///////////////////////////////////////////////////////////////////////////////
#ifndef FILTER_H
#define FILTER_H

#include "ApplicationDefinitions.h"
#include "PlatformDefinitions.h"
#include "SpaceBaseModule.h"

#include "systemc"

class filter: public SpaceBaseModule {
	private:
		buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> m_pixels_buffer_out_resize;
		buffer_pixel<LINE_DETECTION_OUTPUT_BUFFER_SIZE> m_pixels_buffer_out_filter;

	public:
		SC_HAS_PROCESS(filter);
		
		filter(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose);
		void convolution_filter(int height, int width, int kernel_size);
	   	
		void thread();
};

#endif
