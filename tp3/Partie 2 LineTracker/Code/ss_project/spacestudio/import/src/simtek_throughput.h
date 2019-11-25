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
#ifndef SIMTEK_FRAMETHROUGHPUT_H
#define SIMTEK_FRAMETHROUGHPUT_H

#include "generic_throughput.h"

class simtek_throughput: public throughput_if {
	
	public:
		simtek_throughput(timer_value_cb);
		virtual ~simtek_throughput();
		
		virtual throughput_stat get_throughput();
		virtual void reset() { m_throughput->reset(); }

	private:
		throughput_if* m_throughput;
};
#endif

