/*
 * Copyright 2014 Matthew Harvey
 */

#include "stream_flag_guard.hpp"
#include <ios>
#include <ostream>

using std::ostream;

namespace swx
{

StreamFlagGuard::StreamFlagGuard(ostream& p_os):
	m_os(p_os),
	m_original_flags(p_os.flags())
{
}

StreamFlagGuard::~StreamFlagGuard()
{
	reset();
}

void
StreamFlagGuard::reset()
{
	m_os.flags(m_original_flags);		
	return;
}

}  // namespace swx
