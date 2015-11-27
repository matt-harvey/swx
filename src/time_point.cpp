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

#include "time_point.hpp"
#include "config.hpp"
#include <chrono>
#include <cstring>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <string>

namespace chrono = std::chrono;

using std::localtime;
using std::memset;
using std::mktime;
using std::runtime_error;
using std::string;
using std::tm;
using std::time_t;
using std::unique_ptr;

namespace swx
{

namespace
{
    tm zeroed_tm()
    {
        tm tm;
        memset(&tm, 0, sizeof(tm));
        tm.tm_isdst = -1;
        return tm;
    }
}

TimePoint
now()
{
    return chrono::system_clock::now();
}

TimePoint
day_begin(TimePoint const& p_time_point, int p_days_diff)
{
    tm time_tm = time_point_to_tm(p_time_point);
    time_tm.tm_hour = p_days_diff * 24;
    time_tm.tm_min = time_tm.tm_sec = 0;
    time_tm.tm_isdst = -1;
    return tm_to_time_point(time_tm);
}

TimePoint
day_end(TimePoint const& p_time_point, int p_days_diff)
{
    tm time_tm = time_point_to_tm(p_time_point);
    time_tm.tm_hour = p_days_diff * 24;
    time_tm.tm_min = time_tm.tm_sec = 0;
    ++time_tm.tm_mday;
    return tm_to_time_point(time_tm);
}

tm
time_point_to_tm(TimePoint const& p_time_point)
{
    time_t const time_time_t = chrono::system_clock::to_time_t(p_time_point);
    tm* const time_tm_ptr = localtime(&time_time_t);
    return *time_tm_ptr;
}

TimePoint
tm_to_time_point(tm const& p_tm)
{
    tm time_tm = p_tm;
    time_t const time_time_t = mktime(&time_tm);
    return chrono::system_clock::from_time_t(time_time_t);
}

TimePoint
long_time_stamp_to_point(string const& p_time_stamp, string const& p_format)
{
    // note using a static for format string can cause odd bug with strptime (?)
    char const* format = p_format.c_str();
    auto tm = zeroed_tm();
    
    // non-portable
    if (strptime(p_time_stamp.c_str(), format, &tm) == nullptr)
    {
        string const errmsg = "Could not parse timestamp: " + p_time_stamp;
        throw runtime_error(errmsg);
    }

    return chrono::system_clock::from_time_t(mktime(&tm));
}

TimePoint
time_stamp_to_point
(   string const& p_time_stamp,
    string const& p_long_format,
    string const& p_short_format
)
{
    // note using a static for format string can cause odd bug with strptime (?)
    char const* long_format = p_long_format.c_str();
    auto tm = zeroed_tm();

    // non-portable

    // try long format first
    if (strptime(p_time_stamp.c_str(), long_format, &tm) == nullptr)
    {
        // try short format instead
        char const* short_format = p_short_format.c_str();
        tm = time_point_to_tm(day_begin(now()));

        if (strptime(p_time_stamp.c_str(), short_format, &tm) == nullptr)
        {
            string const errmsg = "Could not parse timestamp: " + p_time_stamp;
            throw runtime_error(errmsg);
        }
    }

    return chrono::system_clock::from_time_t(mktime(&tm));
}

string
time_point_to_stamp
(   TimePoint const& p_time_point,
    string const& p_format,
    unsigned int p_formatted_buf_len
)
{
    // don't make this static - caused odd bug with strptime (?)
    char const* format = p_format.c_str();
    tm const time_tmp = time_point_to_tm(p_time_point);
    unique_ptr<char[]> buf(new char[p_formatted_buf_len]);
    if (strftime(buf.get(), p_formatted_buf_len, format, &time_tmp) == 0)
    {
        throw runtime_error("Error formatting TimePoint.");
    }
    string const ret(buf.get());
    return ret;
}

}   // namespace swx
