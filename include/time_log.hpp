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

#include "activity_filter_fwd.hpp"
#include "stint_fwd.hpp"
#include "time_point.hpp"
#include <string>
#include <memory>
#include <vector>

namespace swx
{

/**
 * Represents a record of time spent on various activities, persisted to a
 * plain text file.
 */
class TimeLog
{
// nested types
private:
    class Impl;

// special member functions
public:
    TimeLog
    (   std::string const& p_filepath,
        std::string const& p_time_format,
        unsigned int p_formatted_buf_len
    );
    TimeLog() = delete;
    TimeLog(TimeLog const& rhs) = delete;
    TimeLog(TimeLog&& rhs) = delete;
    TimeLog& operator=(TimeLog const& rhs) = delete;
    TimeLog& operator=(TimeLog&& rhs) = delete;
    ~TimeLog();

// ordinary member functions
public:

    /**
     * Push a new record onto the log. The new record will be immediately
     * persisted to file.
     *
     * It is the caller's reponsibility that this will not leave the log with 
     * entries that are out of time order.
     *
     * @exception std::runtime_error if p_time_point is future dated.
     */
    void append_entry(std::string const& p_activity, TimePoint const& p_time_point);

    /**
     * Amend the activity of the last entry in the log to \e p_activity, with
     * TimePoint \e p_time_point. If there are no entries in the log, this does
     * nothing. The change will be immediately persisted to file.
     *
     * It is the caller's reponsibility that this will not leave the log with 
     * entries that are out of time order.
     *
     * @return the previous activity, or an empty string if inactive (including if
     *   there are no entries in the log).
     * @exception std::runtime_error if p_time_point is future dated.
     */
    std::string amend_last(std::string const& p_activity, TimePoint const& p_time_point);

    /**
     * Apply <em>p_activity_filter.replace(activity, p_new)</em> to every
     * activity matched by \e p_activity_filter. The changes will be immediately
     * persisted to file.
     *
     * @return the number of stints for which a change was made.
     */
    std::vector<Stint>::size_type rename_activity
    (   ActivityFilter const& p_activity_filter,
        std::string const& p_new
    );

    /**
     * Provide \e p_activity_filter to filter by activity name.
     * Provide non-null pointers to TimePoints to filter by date range,
     * or null pointers to ignore one or both ends of the range.
     * 
     * The stints are ordered in ascending date order.
     */
    std::vector<Stint> get_stints
    (   ActivityFilter const& p_activity_filter,
        TimePoint const* p_begin,
        TimePoint const* p_end
    );

    /**
     * @return the most recent activity to match \e p_regex, considered as a
     * regular expression; or return the empty string if none match. (Modified
     * ECMAScript regex grammar is used, consistent with the default grammar
     * for C++ regular expressions.)
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
     * @param p_ago if passed non-zero, return the TimePoint of the entry \e p_ago
     *   entries before the last.
     * @return the TimePoint of the most recent entry in the log, or, if the log
     *   does not have a large enough number of entries to accommodate \e p_ago,
     *   the earliest possible TimePoint.
     */
    TimePoint last_entry_time(std::size_t p_ago = 0);

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

// member variables
private:
    std::unique_ptr<Impl> m_impl;

};  // class TimeLog

}  // namespace swx

#endif  // GUARD_time_log_hpp_6591341885082117
