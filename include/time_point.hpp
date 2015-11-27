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

#ifndef GUARD_time_point_hpp_285827964211734
#define GUARD_time_point_hpp_285827964211734

#include <chrono>
#include <ctime>
#include <string>

namespace swx
{

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

TimePoint now();

/**
 * The first TimePoint of the day on which \e p_time_point falls; or, if
 * \e p_days_diff is passed a positive integer \e n, the first TimePoint of the
 * day \e n days after the day on which \e p_time_point falls; or, if \e
 * p_days_diff is passed a negative integer, \e n, the first TimePoint of the
 * day \e n days before the day on which \e p_time_point falls.
 */
TimePoint day_begin(TimePoint const& p_time_point, int p_days_diff = 0);

/**
 * One TimePoint past the last TimePoint of the day on which \e p_time_point
 * falls; or, if \e p_days_diff is passed a positive integer \e n, one
 * TimePoint past the last TimePoint of the day \e n days after the day on
 * which \e p_time_point falls; or, if \e p_days_diff is passed a negative
 * integer, \e n, one TimePoint past the last Time Point of the day \e n days
 * before the day on which \e p_time_point falls.
 */
TimePoint day_end(TimePoint const& p_time_point, int p_days_diff = 0);

std::tm time_point_to_tm(TimePoint const& p_time_point);

TimePoint tm_to_time_point(std::tm const& p_tm);

/**
 * Converts a timestamp that includes date information into a TimePoint.
 *
 * @exception std::runtime_error if \e p_time_stamp cannot be parsed using
 *   \e p_format
 */
TimePoint long_time_stamp_to_point
(   std::string const& p_time_stamp,
    std::string const& p_format
);

/**
 * Converts a timestamp into a TimePoint. The function first attempts
 * to use \e p_long_format, and if it cannot parse timestamp using
 * \e p_long_format, it attempts to use \e p_short_format.
 *
 * @param p_long_format a format string for a timestamp including date
 *   information
 * @param p_short_format a format string for a timestamp that does not
 *   include date information
 * @param p_time_stamp the timestamp to be parsed
 *
 * @exception std::runtime_error if \e p_time_stamp cannot be parsed using
 *   either format string
 */
TimePoint time_stamp_to_point
(   std::string const& p_time_stamp,
    std::string const& p_long_format,
    std::string const& p_short_format
);

std::string time_point_to_stamp
(   TimePoint const& p_time_point,
    std::string const& p_format,
    unsigned int p_formatted_buf_len
);

}  // namespace swx

#endif  // GUARD_time_point_hpp_285827964211734
