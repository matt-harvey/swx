// Copyright 2014 Matthew Harvey

#ifndef GUARD_interval_hpp_6955864893717628
#define GUARD_interval_hpp_6955864893717628

#include "day.hpp"
#include "seconds.hpp"
#include "time_point.hpp"
#include <chrono>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace swx
{

/**
 * Represent an interval of time. It may be open (still running) or
 * closed (has an end).
 */
class Interval
{
// special member functions
public:

	explicit Interval
	(	TimePoint const& p_beginning,
		Seconds const& p_duration
	);

// ordinary member functions
public:
	Seconds duration() const;
	TimePoint beginning() const;
	TimePoint ending() const;

// member variables
private:
	TimePoint m_beginning;
	Seconds m_duration;

};  // class Interval

template <typename IntervalContainer>
IntervalContainer unstraddle_by_days(IntervalContainer const& container);

std::ostream& operator<<(std::ostream& os, std::vector<Interval> const& container);

// IMPLEMENTATIONS

/*
template <typename IntervalContainer>
IntervalContainer
unstraddle_by_days(IntervalContainer const& container)
{
	// TODO ...
	throw std::runtime_error("Function not implemented.");

	IntervalContainer ret;
	for (auto const& interval: container)
	{
		TimePoint const beginning = interval.beginning();
		TimePoint const ending = interval.ending();
		auto const beg_time_t = std::chrono::system_clock::to_time_t(beginning);
		auto const end_time_t = std::chrono::system_clock::to_time_t(ending);

		// TODO may not be threadsafe
		auto const beg_tm_year = std::localtime(&beg_time_t)->tm_year;
		auto const beg_tm_yday = std::localtime(&beg_time_t)->tm_yday;
		auto const end_tm_year = std::localtime(&end_time_t)->tm_year;
		auto const end_tm_yday = std::localtime(&end_time_t)->tm_yday;

		Day const beg_day(beg_tm_year, beg_tm_yday);
		Day const end_day(end_tm_year, end_tm_yday);

		if (beg_day == end_day)
		{
			ret.push_back(interval);
			continue;
		}

		Day day = beg_day;
		while (day != end_day)
		{
			// TODO ...			
		}
		
		// TODO ...
		
	}
}
*/


}  // namespace swx

#endif  // GUARD_interval_hpp_6955864893717628
