/*
 * Copyright 2014 Matthew Harvey
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "interval.hpp"
#include "config.hpp"
#include "round.hpp"
#include "seconds.hpp"
#include "stream_flag_guard.hpp"
#include "time_point.hpp"
#include <cassert>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>

using std::endl;
using std::fixed;
using std::ios_base;
using std::left;
using std::ostream;
using std::right;
using std::setprecision;
using std::setw;

namespace swx
{

Interval::Interval
(	TimePoint const& p_beginning,
	Seconds const& p_duration,
	bool p_is_live
):
	m_is_live(p_is_live),
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

bool
Interval::is_live() const
{
	return m_is_live;
}

ostream&
operator<<(std::ostream& os, std::vector<Interval> const& container)
{
	double accum_hours = 0.0;
	auto const gap = "  ";
	auto const output_precision = Config::output_precision();
	auto const output_width = Config::output_width();
	auto const rounding_num = Config::output_rounding_numerator();
	auto const rounding_den = Config::output_rounding_denominator();
	for (auto const& interval: container)
	{
		StreamFlagGuard guard(os);
		double hours = interval.duration().count() / 60.0 / 60.0;
		accum_hours += hours;
		os << time_point_to_stamp(interval.beginning()) << gap
		   << time_point_to_stamp(interval.ending()) << gap;
		os << fixed
		   << setprecision(output_precision)
		   << right;
		os << setw(output_width)
		   << round(hours, rounding_num, rounding_den) << gap
		   << setw(output_width)
		   << round(accum_hours, rounding_num, rounding_den);
		if (interval.is_live())
		{
			os << left << gap << "ongoing";
		}
		os << endl;
	}
	return os;
}

}  // namespace swx
