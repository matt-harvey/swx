/*
 * Copyright 2014, 2015 Matthew Harvey
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

#include "print_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include "reporting_command.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::ostream;
using std::runtime_error;
using std::string;
using std::unique_ptr;
using std::vector;

namespace swx
{

PrintCommand::PrintCommand
(   string const& p_command_word,
    vector<string> const& p_aliases,
    TimeLog& p_time_log
):
    ReportingCommand
    (   p_command_word,
        p_aliases,
        "Print summary of activities",
        vector<HelpLine>
        {   HelpLine("Print summary of time spent on all activities"),
            HelpLine("Print summary of time spent on ACTIVITY", "<ACTIVITY>")
        },
        p_time_log
    )
{
    add_option
    (   vector<string>{"f", "from"},
        HelpLine("Only count time spent on activities since TIMESTAMP", "<TIMESTAMP>"),
        nullptr,
        &m_since_str
    );
    add_option
    (   vector<string>{"t", "to"},
        HelpLine("Only count time spent on activities until TIMESTAMP", "<TIMESTAMP>"),
        nullptr,
        &m_until_str
    );
}

PrintCommand::~PrintCommand() = default;

Command::ErrorMessages
PrintCommand::do_process
(   Config const& p_config,
    vector<string> const& p_ordinary_args,
    ostream& p_ordinary_ostream
)
{
    ErrorMessages ret;
    unique_ptr<TimePoint> since_time_point_ptr;
    unique_ptr<TimePoint> until_time_point_ptr;
    auto const long_time_fmt = p_config.time_format();
    auto const short_time_fmt = p_config.short_time_format();
    if (!m_since_str.empty())
    {
        try
        {
            since_time_point_ptr.reset
            (   new TimePoint
                (   time_stamp_to_point(m_since_str, long_time_fmt, short_time_fmt)
                )
            );
        }
        catch (runtime_error&)
        {
            ret.push_back("Could not parse timestamp: " + m_since_str);
        }
    }
    if (!m_until_str.empty())
    {
        try
        {
            until_time_point_ptr.reset
            (   new TimePoint
                (   time_stamp_to_point(m_until_str, long_time_fmt, short_time_fmt)
                )
            );
        }
        catch (runtime_error&)
        {
            ret.push_back("Could not parse timestamp: " + m_until_str);
        }
    }
    if (ret.empty())
    {
        auto const error_messages = print_report
        (   p_ordinary_ostream,
            p_config,
            p_ordinary_args,
            since_time_point_ptr.get(),
            until_time_point_ptr.get()
        );
        for (auto const& message: error_messages) ret.push_back(message);
    }
    return ret;
}

}  // namespace swx
