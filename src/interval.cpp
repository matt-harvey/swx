// Copyright 2014 Matthew Harvey

#include "interval.hpp"
#include "seconds.hpp"
#include "time_conversion.hpp"
#include "time_point.hpp"
#include <cassert>
#include <iostream>
#include <ostream>

using std::endl;
using std::ostream;

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

ostream&
operator<<(std::ostream& os, std::vector<Interval> const& container)
{
	double accum_hours = 0.0;
	for (auto const& interval: container)
	{
		double hours = interval.duration().count() / 60.0 / 60.0;
		accum_hours += hours;
		os << time_point_to_stamp(interval.beginning()) << "  "
		   << time_point_to_stamp(interval.ending()) << "  "
		   << hours << "  " << accum_hours << endl;
	}
	return os;
}
}  // namespace swx
