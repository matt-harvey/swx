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
 *
 * @todo HIGH PRIORITY Reflect whether live or not when printing (vectors of)
 * Intervals.
 */
class Interval
{
// special member functions
public:

	explicit Interval
	(	TimePoint const& p_beginning,
		Seconds const& p_duration,
		bool p_is_live = false
	);

// ordinary member functions
public:
	Seconds duration() const;
	TimePoint beginning() const;
	TimePoint ending() const;
	bool is_live() const;

// member variables
private:
	bool m_is_live;
	TimePoint m_beginning;
	Seconds m_duration;

};  // class Interval

template <typename IntervalContainer>
IntervalContainer unstraddle_by_days(IntervalContainer const& container);

std::ostream& operator<<
(	std::ostream& os,
	std::vector<Interval> const& container
);

}  // namespace swx

#endif  // GUARD_interval_hpp_6955864893717628
