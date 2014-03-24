// Copyright 2014 Matthew Harvey

#include "interval.hpp"
#include "round.hpp"
#include "seconds.hpp"
#include "time_conversion.hpp"
#include "time_point.hpp"
#include <cassert>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>

using std::endl;
using std::fixed;
using std::ios_base;
using std::ostream;
using std::right;
using std::setprecision;
using std::setw;

#define SWX_OUTPUT_ROUNDING_NUMERATOR 1
#define SWX_OUTPUT_ROUNDING_DENOMINATOR 4
#define SWX_OUTPUT_PRECISION 2
#define SWX_OUTPUT_WIDTH 6

namespace swx
{

namespace
{
	double round(double x)
	{
		return swx::round
		(	x,
			SWX_OUTPUT_ROUNDING_NUMERATOR,
			SWX_OUTPUT_ROUNDING_DENOMINATOR
		);
	}

}  // end anonymous namespace

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
	ios_base::fmtflags const orig_flags = os.flags();
	double accum_hours = 0.0;
	try
	{
		for (auto const& interval: container)
		{
			double hours = interval.duration().count() / 60.0 / 60.0;
			accum_hours += hours;

			os << time_point_to_stamp(interval.beginning()) << "    "
			   << time_point_to_stamp(interval.ending()) << "  ";

			os << fixed
			   << setprecision(SWX_OUTPUT_PRECISION)
			   << right;

			os << setw(SWX_OUTPUT_WIDTH) << round(hours) << "  "
			   << setw(SWX_OUTPUT_WIDTH) << round(accum_hours) << endl;

			os.flags(orig_flags);
		}
	}
	catch (...)
	{
		os.flags(orig_flags);	
		throw;
	}
	return os;
}

}  // namespace swx
