///////////////////////////////////////////////////////////////////////////////
///
///         Space Codesign System Inc. - (http://www.spacecodesign.com)
///         (c) All Rights Reserved. 2012
///
///         No authorization to modify or use this file for
///         commercial purposes without prior written consentement
///         of its author(s)
///
///////////////////////////////////////////////////////////////////////////////
#include "generic_throughput.h"
#include "SpaceDisplay.h"
#include <cstdio>

generic_throughput::generic_throughput(timer_callback timer_callbacks, double timer_frequency, count_cb count)
:m_timer_callbacks(timer_callbacks), m_timer_frequency(timer_frequency), m_count(count) {
	unsigned long long value0 = m_timer_callbacks.value();
	unsigned long long value1 = m_timer_callbacks.value();
	m_access_delay = 0;
	if (value0 > value1)
		m_access_delay = value0-value1;
	else
		m_access_delay = value1-value0;
    reset();
}

void generic_throughput::reset() {
	m_timer_callbacks.stop();
	m_timer_callbacks.reset();
	m_timer_callbacks.start();
	m_last_count = m_count();
	m_value = m_timer_callbacks.value();
}

throughput_stat generic_throughput::get_throughput() {
	double throughput = 0.0;
	double delta_time = 0.0;
	unsigned long delta_count = 0;
	unsigned long current_count = m_count();
	unsigned long long current_value = m_timer_callbacks.value();
	
	if (m_value > current_value || m_last_count > current_count) {
		reset();
	}
	else {
		delta_count = current_count-m_last_count;
		double delta_value = current_value-m_value-2*m_access_delay;
		if (delta_value > 0.0) {
			delta_time = delta_value/m_timer_frequency;
			throughput = delta_count/delta_time;
		}
	}

	if (delta_count > 0) {
		m_value = current_value;
		m_last_count = current_count;
	}

	return { throughput, delta_time };
}
