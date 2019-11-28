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
#include "display.h"

#include "SpaceDisplay.h"
#include "SpaceTypes.h"
#include "client_socket_helper.h"

#if defined(SPACE_SIMULATION_RELEASE) || defined(SPACE_SIMULATION_MONITORING) || defined(SPACE_SIMULATION_DEBUG)
static display* m_self;
static unsigned long long timer_value() {
	return m_self->get_timer_value();
}
#endif

display::display(sc_core::sc_module_name name, double period, sc_core::sc_time_unit unit, unsigned int id, unsigned char priority, bool verbose)
:SpaceBaseModule(name, period, unit, id, priority, verbose) {
	SC_THREAD(thread);
#if MAPPING == HW_MAPPING
	m_socket = new client_handler(this->name(), "localhost" , "5016");
#else
	m_socket = new client_handler(this->name(), "132.207.29.99" , "5016");
#endif
}

display::~display() {
	delete m_socket;
}

void display::thread(void) {
	unsigned int height = 0, width = 0, maximum_nb_lines = 0, command_status = 0, input_address = 0;
	m_head_in_buff = 0;
	m_tail_out_buff = 0;
	unsigned long m_cumulative = 0;

	double m_cumulative_througput = 0.0;
	throughput_if* m_throughput;
	#if defined(SPACE_ARCHITECTURE_IMPLEMENTATION)
		m_throughput = new linux_throughput();
	#else
		m_self = this;
		m_throughput = new simtek_throughput(&timer_value);
	#endif

	while(1) {
		ComputeFor(1);

		m_throughput->reset();

		// Update input FIFO tail value
		ModuleWrite(LINE_DETECTION0_ID, SPACE_BLOCKING, &m_tail_out_buff);

		// Before reading, verify if input FIFO is not empty
		is_input_fifo_empty();

		RegisterRead(REGISTERFILE0_ID, 12, &height);
		RegisterRead(REGISTERFILE0_ID, 13, &width);
		RegisterRead(REGISTERFILE0_ID, 14, &maximum_nb_lines);
		RegisterRead(REGISTERFILE0_ID, 15, &command_status);
		RegisterRead(REGISTERFILE0_ID, 16, &input_address);

		// Show image
		unsigned int address = input_address + (maximum_nb_lines * sizeof(line_points) * m_tail_out_buff);
		show_lines(address, width, height, maximum_nb_lines);

		// Changing Input FIFO tail value
		m_tail_out_buff = (m_tail_out_buff + 1) % FIFO_LENGTH;
		m_cumulative++;

		throughput_stat stat = m_throughput->get_throughput();
		double delta = stat.delta_time;
		m_cumulative_througput+=stat.throughput;

#if MAPPING == HW_MAPPING
		SpacePrint("%f frames/second\n", stat.throughput);
#endif
		send_duration(m_socket->get_socket(), stat.delta_time);
	}
}

void display::is_input_fifo_empty(){
	ModuleRead(LINE_DETECTION0_ID, SPACE_BLOCKING, &m_head_in_buff);

	if(m_head_in_buff == m_tail_out_buff) {
		SpaceMessageVerbose(GetVerbose(), "Display", "Input FIFO is empty  - head = %d tail = %d", m_head_in_buff, m_tail_out_buff);
		ModuleRead(LINE_DETECTION0_ID, SPACE_BLOCKING, &m_head_in_buff);
	}
}

void display::show_lines(unsigned int address, unsigned int width, unsigned int height, unsigned int maximum_nb_lines) {
	line_points* lines =  new line_points[maximum_nb_lines];

	DeviceRead(DDR_ID, address, &lines[0], maximum_nb_lines);

	if (!m_socket->has_error()) {
		bool is_ok = send_lines(m_socket->get_socket(), lines, maximum_nb_lines);
		if (!is_ok) {
			SpacePrint("%s error while sending image to socket", name());
			return;
		}
	}
	else {
		SpacePrint("%s socket not ready", name());
		return;
	}

	delete[] lines;
}

#if defined(SPACE_SIMULATION_RELEASE) || defined(SPACE_SIMULATION_MONITORING) || defined(SPACE_SIMULATION_DEBUG)
unsigned long long display::get_timer_value() {
#if MAPPING == HW_MAPPING
	return sc_core::sc_time_stamp().value();
#else
	unsigned long low, high, high2;
	unsigned long long llow, lhigh;

	do {
		DeviceRead(SIMULATION_TIMER_ID, SIMULATION_TIME_LOW, &low);
		DeviceRead(SIMULATION_TIMER_ID, SIMULATION_TIME_HIGH, &high);
		DeviceRead(SIMULATION_TIMER_ID, SIMULATION_TIME_HIGH, &high2);
	} while(high2 != high);

	llow = low;
	lhigh = high;
	return lhigh << 32 | llow;
#endif
}
#endif
