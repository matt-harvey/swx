// Copyright 2014 Matthew Harvey

#include "interval.hpp"
#include "seconds.hpp"
#include "time_point.hpp"
#include <cassert>

namespace swx
{

Interval::Interval
(	TimePoint const& p_beginning,
	Seconds const& p_duration
):
	m_beginning(p_beginning),
	m_duration(p_duration)
{
}

Seconds
Interval::duration() const
{
	return m_duration;
}

TimePoint
Interval::beginning() const
{
	return m_beginning;
}

TimePoint
Interval::ending() const
{
	return beginning() + duration();
}

}  // namespace swx
