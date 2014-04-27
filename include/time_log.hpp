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

#ifndef GUARD_time_log_hpp_6591341885082117
#define GUARD_time_log_hpp_6591341885082117

#include "interval.hpp"
#include "stint.hpp"
#include "time_point.hpp"
#include <cstddef>
#include <string>
#include <map>
#include <vector>

namespace swx
{

/**
 * Represents a record of time spent on various activities, persisted to a
 * plain text file.
 */
class TimeLog
{
// nested types and typedefs
private:

	typedef std::vector<std::string> Activities;
	typedef Activities::size_type ActivityId;
	typedef std::map<std::string, ActivityId> ActivityMap;

	/**
	 * Represents a single record in the log.
	 */
	struct Entry
	{
		Entry(ActivityId p_activity_id, TimePoint const& p_time_point);
		ActivityId activity_id;
		TimePoint time_point;
	};

// special member functions
public:
	explicit TimeLog(std::string const& p_filepath);

	TimeLog() = delete;
	TimeLog(TimeLog const& rhs) = delete;
	TimeLog(TimeLog&& rhs) = delete;
	TimeLog& operator=(TimeLog const& rhs) = delete;
	TimeLog& operator=(TimeLog&& rhs) = delete;
	~TimeLog() = default;

// ordinary member functions
public:

	/**
	 * Push a new record onto the log. The new record will be immediately
	 * persisted to file.
	 */
	void append_entry
	(	std::string const& p_activity,
		TimePoint const& p_time_point
	);

	/**
	 * Pass non-null pointers to filter by activity name and/or date
	 * range, or pass null pointers to ignore a particular filter.
	 * Caller retains ownership of pointed-to memory.
	 */
	std::vector<Stint> get_stints
	(	std::string const* p_activity = nullptr,
		TimePoint const* p_begin = nullptr,
		TimePoint const* p_end = nullptr
	);

private:
	void clear_cache();
	void mark_cache_as_stale();
	void load();
	ActivityId register_activity(std::string const& p_activity);

	void load_entry
	(	std::string const& p_entry_string,
		std::size_t p_line_number
	);

	std::string const& id_to_activity(ActivityId p_activity_id);

	std::vector<Entry>::const_iterator find_entry_just_before
	(	TimePoint const& p_time_point
	);

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
