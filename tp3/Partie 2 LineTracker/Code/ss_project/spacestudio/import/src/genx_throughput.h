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
#ifndef GENX_FRAMETHROUGHPUT_H
#define GENX_FRAMETHROUGHPUT_H

#include "throughput_if.h"

class genx_throughput: public throughput_if {
	
	public:
		genx_throughput();
		virtual ~genx_throughput();
		
		virtual throughput_stat get_throughput();
		virtual void reset() { m_throughput->reset(); }

	private:
		throughput_if* m_throughput;
};
#endif

