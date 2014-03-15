// Copyright 2014 Matthew Harvey

#include "interval.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <jewel/assert.hpp>
#include <jewel/exception.hpp>

using boost::optional;

namespace posix_time = boost::posix_time;

namespace swx
{

Interval::Interval(posix_time::ptime const& p_beginning):
	m_beginning(p_beginning)
{
	JEWEL_ASSERT (!m_maybe_ending);
}

void
Interval::close(posix_time::ptime const& p_ending)
{
	if (is_closed(*this))
	{
		JEWEL_THROW(IntervalAlreadyClosedException, "Interval already closed.");
	}
	if (p_ending < beginning())
	{
		JEWEL_THROW
		(	InvalidIntervalException,
			"Interval cannot begin before it ends."
		);
	}
	m_maybe_ending = p_ending;
	return;
}

posix_time::ptime
Interval::beginning() const
{
	return m_beginning;
}

optional<posix_time::ptime>
Interval::maybe_ending() const
{
	return m_maybe_ending;
}

bool
is_open(Interval const& p_interval)
{
	return !is_closed(p_interval);
}

bool
is_closed(Interval const& p_interval)
{
	return static_cast<bool>(p_interval.maybe_ending());
}

}  // namespace swx
