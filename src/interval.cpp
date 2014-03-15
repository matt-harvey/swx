// Copyright 2014 Matthew Harvey

#include "interval.hpp"
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

}  // namespace swx
