// Copyright 2014 Matthew Harvey

#include "interval.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <jewel/assert.hpp>

namespace posix_time = boost::posix_time;

namespace swx
{

Interval::Interval(posix_time::ptime const& p_begin): m_begin(p_begin)
{
	JEWEL_ASSERT (!m_end);
}

}  // namespace swx
