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
#ifndef LINUX_FRAMETHROUGHPUT_H
#define LINUX_FRAMETHROUGHPUT_H

#include "throughput_if.h"

class linux_throughput: public throughput_if {
	
	public:
		linux_throughput();
		virtual ~linux_throughput();
		
		virtual throughput_stat get_throughput();
		virtual void reset() { m_throughput->reset(); }

	private:
		throughput_if* m_throughput;
};
#endif

