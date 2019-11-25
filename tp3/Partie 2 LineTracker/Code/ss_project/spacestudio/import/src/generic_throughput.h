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
#ifndef GENERIC_THROUGHPUT_H
#define GENERIC_THROUGHPUT_H
#include "throughput_if.h"

typedef void (*timer_cb)(void);
typedef unsigned long long (*timer_value_cb)(void);
typedef unsigned long (*count_cb)(void);

typedef struct {
	timer_cb start;
	timer_cb stop;
	timer_cb reset;
	timer_value_cb value;
} timer_callback;

class generic_throughput: public throughput_if {
	public:
		generic_throughput(timer_callback timer_callbacks, double timer_frequency, count_cb count);
		virtual throughput_stat get_throughput();
		virtual void reset();

	private:
		timer_callback m_timer_callbacks;
		count_cb m_count;
		double m_timer_frequency;
		unsigned long m_last_count;
		unsigned long long m_value;
		unsigned long long m_access_delay;
};
#endif

