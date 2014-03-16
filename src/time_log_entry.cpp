/*
 * Copyright 2014 Matthew Harvey
 */

#include "time_log_entry.hpp"

namespace swx
{

TimeLogEntry::TimeLogEntry
(	TimeLog& p_time_log,
	ActivityId p_activity_id,
	TimePoint p_time_point
):
	m_time_log(p_time_log),
	m_activity_id(p_activity_id),
	m_time_point(p_time_point)
{
}

}  // namespace swx
