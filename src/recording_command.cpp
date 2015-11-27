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

#include "recording_command.hpp"
#include "command.hpp"
#include "help_line.hpp"
#include "result.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <stdexcept>
#include <string>
#include <vector>

using std::runtime_error;
using std::string;
using std::vector;

namespace swx
{

RecordingCommand::RecordingCommand
(   string const& p_command_word,
    vector<string> const& p_aliases,
    string const& p_usage_summary,
    vector<HelpLine> const& p_help_line,
    bool p_accept_ordinary_args,
    TimeLog& p_time_log
):
    Command
    (   p_command_word,
        p_aliases,
        p_usage_summary,
        p_help_line,
        p_accept_ordinary_args
    ),
    m_time_log(p_time_log)
{
}

RecordingCommand::~RecordingCommand() = default;

Result<TimePoint>
RecordingCommand::time_point
(   std::string const& p_time_stamp,
    std::string const& p_time_format,
    std::string const& p_short_time_format
)
{
    using ResultT = Result<TimePoint>;
    try
    {
        TimePoint const tp =
            time_stamp_to_point(p_time_stamp, p_time_format, p_short_time_format);
        if (tp > now())
        {
            return ResultT::make_invalid("Timestamp must not be future-dated.");
        }
        return ResultT::make_valid(tp);
    }
    catch (runtime_error&)
    {
        return ResultT::make_invalid("Could not parse timestamp: " + p_time_stamp);
    }
}

TimeLog&
RecordingCommand::time_log()
{
    return m_time_log;
}

}  // namespace swx
