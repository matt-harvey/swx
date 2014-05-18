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
	 * persisted to file, with a date of "now".
	 */
	void append_entry(std::string const& p_activity);

	/**
	 * Pass non-null pointers to filter by activity name and/or date
	 * range, or pass null pointers to ignore a particular filter.
	 * Caller retains ownership of pointed-to memory.
	 *
	 * If \e p_use_regex is passed \e true, then, rather than retrieving only
	 * stints with <em>*p_activity</em> as their activity, then function will
	 * retrieve stints for which their activity matches <em>*p_activity</em>,
	 * treated as a POSIX extended regular expression.
	 * 
	 */
	std::vector<Stint> get_stints
	(	std::string const* p_sought_activity,
		TimePoint const* p_begin,
		TimePoint const* p_end,
		bool p_use_regex = false
	);

	/**
	 * @return the most recent activity to match \e p_regex, considered as a
	 * regular expression of POSIX extended variety; or return the
	 * empty string if none match.
	 */
	std::string last_activity_to_match(std::string const& p_regex);

	/**
	 * @returns a vector containing the last \e p_num Activities in the log
	 * (not counting the empty, "inactive activity"). The last activity
	 * is listed at the front of the vector, then the penultimate activity,
	 * etc.. If there are consecutive entries in the log that have the same
	 * activity, these are treated as if there were a single combined entry
	 * for the purposes of the function. Thus the returned vector never
	 * contains consecutive identical activities.
	 *
	 * If there are fewer than \e p_num Activities to return, then a
	 * correspondingly shorter vector is returned. If the TimeLog is empty,
	 * then an empty vector will be returned.
	 */
	std::vector<std::string> last_activities(std::size_t p_num);

	/**
	 * @returns \e true if and only if there is an activity recorded in the log
	 * that is ongoing as at \e p_time_point.
	 */
	bool is_active_at(TimePoint const& p_time_point);

	/**
	 * @returns \e true if and only if the TimeLog is active at the most recent
	 * recorded point (which might possibly be in the future).
	 */
	bool is_active();

	/**
	 * @returns \e true if and only if p_activity has already been recorded
	 * at least once in the log.
	 */
	bool has_activity(std::string const& p_activity);

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
