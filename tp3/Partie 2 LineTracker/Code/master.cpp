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
#include "master.h"

#include "PlatformDefinitions.h"
#include "ApplicationDefinitions.h"
#include "SpaceDisplay.h"
#include "SpaceTypes.h"


master::master(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose)
:SpaceBaseModule(name, period, unit, id, priority, verbose) {
	SC_THREAD(thread);
}

void master::thread(void) {
	unsigned int  command_status = 0x0000FFFF;
	unsigned int  width = IWIDTH;
	unsigned int  height = IHEIGHT;
	unsigned int  resizing_scale = RESIZING_SCALE;

	unsigned int  width_scaled = width / resizing_scale;
	unsigned int  height_scaled = height / resizing_scale;

	unsigned int  low_thres = LOW_THRESH;
	unsigned int  high_thres = HIGH_THRESH;

	unsigned int  threshold = HOUGH_THRESHOLD;
	unsigned int maximum_nb_lines = MAXIMUM_NB_LINES;

	unsigned int  input_address, output_address;

	computeFor(1); // Wait reset cycle

	// Canny equalizer module registers - Canny need 3 Image buffers to be used during canny operation
	input_address = DDR_BASE_ADDRESS;
	output_address = input_address +  (width * height * FIFO_LENGTH);
	RegisterWrite(REGISTERFILE0_ID, 0, &height);
	RegisterWrite(REGISTERFILE0_ID, 1, &width);
	RegisterWrite(REGISTERFILE0_ID, 2, &resizing_scale);
	RegisterWrite(REGISTERFILE0_ID, 3, &height_scaled);
	RegisterWrite(REGISTERFILE0_ID, 4, &width_scaled);
	RegisterWrite(REGISTERFILE0_ID, 5, &input_address);
	RegisterWrite(REGISTERFILE0_ID, 6, &output_address);
	RegisterWrite(REGISTERFILE0_ID, 7, &low_thres);
	RegisterWrite(REGISTERFILE0_ID, 8, &high_thres);
	RegisterWrite(REGISTERFILE0_ID, 9, &threshold);
	RegisterWrite(REGISTERFILE0_ID, 10, &maximum_nb_lines);
	RegisterWrite(REGISTERFILE0_ID, 11, &command_status);

	input_address = output_address;
	RegisterWrite(REGISTERFILE0_ID, 12, &height_scaled);
	RegisterWrite(REGISTERFILE0_ID, 13, &width_scaled);
	RegisterWrite(REGISTERFILE0_ID, 14, &maximum_nb_lines);
	RegisterWrite(REGISTERFILE0_ID, 15, &command_status);
	RegisterWrite(REGISTERFILE0_ID, 16, &input_address);


	unsigned int go = 1;
	ModuleWrite(RECEPTION0_ID, SPACE_BLOCKING, &go);

	while(1) {
		//The parameters dont change during the execution ... wait forever
		sc_core::wait(1);
	}
}

