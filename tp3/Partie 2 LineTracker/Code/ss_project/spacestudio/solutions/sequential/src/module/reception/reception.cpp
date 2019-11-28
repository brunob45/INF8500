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
#include "reception.h"

#include "PlatformDefinitions.h"
#include "ApplicationDefinitions.h"
#include "SpaceDisplay.h"
#include "SpaceTypes.h"
#include "client_socket_helper.h"

reception::reception(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose)
:SpaceBaseModule(name, period, unit, id, priority, verbose) {
	SC_THREAD(thread);
#if MAPPING == HW_MAPPING
	m_socket = new client_handler(this->name(), "localhost", "5015");
#else
	//m_socket = new client_handler(this->name(), "192.168.2.104", "28015");
	m_socket = new client_handler(this->name(), "132.207.29.99", "5015");
#endif
}

reception::~reception() {
	delete m_socket;
}

void reception::thread(void) {
	unsigned char image[IWIDTH*IHEIGHT];
	unsigned int memory_output_offset = 0;
	unsigned int nb_frame = 0;
	m_head_out_buff = 0;
	m_tail_in_buff = 0;

	computeFor(1); // Wait reset cycle

	// Wait Master before starting
	unsigned int go = 0;
	ModuleRead(MASTER0_ID, SPACE_BLOCKING, &go);

	while(1) {
		//Update output FIFO head value
		ModuleWrite(LINE_DETECTION0_ID, SPACE_BLOCKING, &m_head_out_buff);
		memory_output_offset = IWIDTH * IHEIGHT * m_head_out_buff;

		/* read socket */
		if (!m_socket->has_error()) {
			bool is_ok = get_image_by_socket(m_socket->get_socket(), image, IWIDTH, IHEIGHT);
			if (!is_ok) {
				SpacePrint("%s error while getting image from socket", name());
				return;	//sc_stop();
			}
		}
		else {
			SpacePrint("%s socket not ready", name());
			return;	//sc_stop();
		}

		unsigned int address = DDR_BASE_ADDRESS + memory_output_offset;
		DeviceWrite(DDR_ID, address, (unsigned int*)&image[0], IWIDTH*IHEIGHT/4);

		//Before updating head, verify if FIFO is FULL?
		is_output_fifo_full();

		//Update output FIFO head value
		m_head_out_buff = (m_head_out_buff + 1) % FIFO_LENGTH;


		computeFor(1);
	}
}

// Verify if output memory FIFO is full, if so it will block until an image is read from the FIFO
void reception::is_output_fifo_full(){
	ModuleRead(LINE_DETECTION0_ID, SPACE_BLOCKING, &m_tail_in_buff);
	if((m_head_out_buff + 1)  % FIFO_LENGTH ==  m_tail_in_buff){
		ModuleRead(LINE_DETECTION0_ID, SPACE_BLOCKING, &m_tail_in_buff);
	}
}
