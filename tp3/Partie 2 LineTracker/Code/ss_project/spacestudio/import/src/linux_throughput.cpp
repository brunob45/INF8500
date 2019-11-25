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
#include "linux_throughput.h"
#include "generic_throughput.h"
#include <chrono>

extern "C" {
	#include "gt.h"
}

static unsigned long generic_count = 0;

static unsigned long static_generic_count() {
	return generic_count++;
}

static void timer_start() { }

static void timer_stop() { }

static void timer_reset() { }

static unsigned long long timer_value() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

linux_throughput::linux_throughput() {
	unsigned long long frequency = 1000.0;
	timer_callback callbacks = { &timer_start, &timer_stop, &timer_reset, &timer_value };
	m_throughput = new generic_throughput(callbacks, frequency, &static_generic_count);
}

linux_throughput::~linux_throughput() {
	delete m_throughput;
}

throughput_stat linux_throughput::get_throughput() {
	return m_throughput->get_throughput();
}
