// Copyright 2014 Matthew Harvey

#ifndef GUARD_interval_hpp_6955864893717628
#define GUARD_interval_hpp_6955864893717628

#include "seconds.hpp"
#include "time_point.hpp"
#include <chrono>

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

	/**
	 * @param p_beginning Time at which the interval begins. Defaults
	 * to NOW.
	 */
	explicit Interval
	(	TimePoint const& p_beginning,
		Seconds const& p_duration
	);

private:

	TimePoint m_beginning;
	Seconds m_duration;

};  // class Interval

}  // namespace swx

#endif  // GUARD_interval_hpp_6955864893717628
