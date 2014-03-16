/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_time_log_entry_hpp_3198050499520288
#define GUARD_time_log_entry_hpp_3198050499520288

#include "activity_id.hpp"
#include "time_point.hpp"

namespace swx
{

// begin forward declarations

class TimeLog;

// end forward declarations

class TimeLogEntry
{
// special member functions
	TimeLogEntry
	(	TimeLog& p_time_log,
		ActivityId p_activity_id,
		TimePoint p_time_point
	);

// member variables
private:
	TimeLog& m_time_log;
	ActivityId m_activity_id;
	TimePoint m_time_point;

};  // class TimeLogEntry

}  // namespace swx

#endif  // GUARD_time_log_entry_hpp_3198050499520288
