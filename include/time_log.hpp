/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_time_log_hpp_6591341885082117
#define GUARD_time_log_hpp_6591341885082117

#include "interval.hpp"
#include <vector>

namespace swx
{

class TimeLog
{
// special member functions
public:
	TimeLog() = delete;
	TimeLog(TimeLog const& rhs) = delete;
	TimeLog(TimeLog&& rhs) = delete;
	TimeLog& operator=(TimeLog const& rhs) = delete;
	TimeLog& operator=(TimeLog&& rhs) = delete;
	~TimeLog() = default;

// member variables
private:
	std::vector<Interval> m_intervals;

};  // class TimeLog

}  // namespace swx

#endif  // GUARD_time_log_hpp_6591341885082117
