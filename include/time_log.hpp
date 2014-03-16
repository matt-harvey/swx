/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_time_log_hpp_6591341885082117
#define GUARD_time_log_hpp_6591341885082117

#include "activity.hpp"
#include "activity_id.hpp"
#include "time_log_entry.hpp"
#include <string>
#include <unordered_map>
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
	std::vector<TimeLogEntry> m_entries;
	std::vector<Activity> m_activities;
	std::unordered_map<std::string, ActivityId> m_activity_map;

};  // class TimeLog

}  // namespace swx

#endif  // GUARD_time_log_hpp_6591341885082117
