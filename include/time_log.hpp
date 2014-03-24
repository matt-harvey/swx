/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_time_log_hpp_6591341885082117
#define GUARD_time_log_hpp_6591341885082117

#include "activity.hpp"
#include "activity_id.hpp"
#include "time_point.hpp"
#include <string>
#include <map>
#include <vector>

namespace swx
{

class TimeLog
{
// nested types and typedefs
private:
	struct Entry
	{
		Entry(ActivityId p_activity_id, TimePoint const& p_time_point);
		ActivityId activity_id;
		TimePoint time_point;
	};
public:
	typedef std::vector<Activity> Activities;
private:
	typedef std::map<std::string, ActivityId> ActivityMap;

// special member functions
public:
	explicit TimeLog(std::string const& p_filepath);

	TimeLog() = delete;
	TimeLog(TimeLog const& rhs) = delete;
	TimeLog(TimeLog&& rhs) = delete;
	TimeLog& operator=(TimeLog const& rhs) = delete;
	TimeLog& operator=(TimeLog&& rhs) = delete;
	~TimeLog() = default;

/// ordinary member functions
public:
	void append_entry
	(	Activity const& p_activity,
		TimePoint const& p_time_point
	);

	/**
	 * @returns a non-owning pointer to the Activity named
	 * \e p_activity_name, or nullptr if there is no such Activity
	 * in the TimeLog.
	 *
	 * @todo This should probably be weak_ptr instead of a raw pointer.
	 */
	Activity* get_activity_by_name
	(	std::string const& p_activity_name
	);
private:
	void clear_cache();
	void mark_cache_as_stale();
	void load();
	ActivityId register_activity(std::string const& p_activity_name);
	void load_entry(std::string const& p_entry_string);

// member variables
private:
	bool m_is_loaded;
	std::string m_filepath;
	std::vector<Entry> m_entries;
	Activities m_activities;
	ActivityMap m_activity_map;

};  // class TimeLog

}  // namespace swx

#endif  // GUARD_time_log_hpp_6591341885082117
