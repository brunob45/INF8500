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
#include "genx_throughput.h"
#include "generic_throughput.h"
#include <cstddef>
#include <cstdio>

extern "C" {
	#include "gt.h"
}

static unsigned long generic_count = 0;

static unsigned long static_generic_count() {
	return generic_count++;
}

static void timer_start() {
	start_gt();
}

static void timer_stop() {
	stop_gt();
}

static void timer_reset() {
	reset_gt();
}

static unsigned long long timer_value() {
	return get_time_gt();
}

genx_throughput::genx_throughput() {
	map_gt();
	unsigned long long frequency = 333000000.0;
	timer_callback callbacks = { &timer_start, &timer_stop, &timer_reset, &timer_value };
	m_throughput = new generic_throughput(callbacks, frequency, &static_generic_count);
}

genx_throughput::~genx_throughput() {
	delete m_throughput;
	unmap_gt();
}

throughput_stat genx_throughput::get_throughput() {
	return m_throughput->get_throughput();
}
