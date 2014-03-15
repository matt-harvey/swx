// Copyright 2014 Matthew Harvey

#include "interval.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <jewel/assert.hpp>
#include <jewel/exception.hpp>

namespace posix_time = boost::posix_time;

namespace swx
{

Interval::Interval(posix_time::ptime const& p_begin): m_begin(p_begin)
{
	JEWEL_ASSERT (!m_end);
}

void
Interval::close(posix_time::ptime const& p_end)
{
	if (is_closed())
	{
		JEWEL_THROW(IntervalException, "Interval already closed.");
	}
	m_end = p_end;
	return;
}

bool
Interval::is_open() const
{
	return !is_closed();
}

bool
Interval::is_closed() const
{
	return static_cast<bool>(m_end);
}

}  // namespace swx
