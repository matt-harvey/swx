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

#include "day_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include "reporting_command.hpp"
#include "time_point.hpp"
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::ostream;
using std::ostringstream;
using std::string;
using std::vector;

namespace swx
{

namespace
{
    int const k_max_days_ago = 9;
    
    string days_ago_pluralized(int days)
    {
        if (days == 1)
        {
            return "yesterday";
        }
        ostringstream oss;
        oss << days << " days ago";
        return oss.str();
    }
}

DayCommand::DayCommand
(   string const& p_command_word,
    vector<string> const& p_aliases,
    TimeLog& p_time_log
):
    ReportingCommand
    (   p_command_word,
        p_aliases,
        "Print summary of a single day's activities",
        vector<HelpLine>
        {   HelpLine("Print summary of time spent on all activities today"),
            HelpLine
            (   "Print summary of time spent on ACTIVITY today",
                "<ACTIVITY>"
            )
        },
        p_time_log
    )
{
    string const s("Instead of today's activities, print the activities from ");
    for (int i = 1; i <= k_max_days_ago; ++i)
    {
        add_boolean_option
        (   static_cast<char>(i + '0'),
            s + days_ago_pluralized(i)
        );
    }
}

DayCommand::~DayCommand() = default;

Command::ErrorMessages
DayCommand::do_process
(   Config const& p_config,
    ParsedArguments const& p_args,
    ostream& p_ordinary_ostream
)
{
    ErrorMessages ret;
    TimePoint const n = now();

    int days_ago = 0;
    bool digit_flag_encountered = false;
    auto const& flags = p_args.flags();

    for (int i = 1; i <= k_max_days_ago; ++i)
    {
        if (flags.count(static_cast<char>(i + '0')))
        {
            if (digit_flag_encountered)
            {
                ret.push_back
                (   "Only a single digit numeric option can be passed "
                    "to this command"
                );
            }
            else
            {
                days_ago = i;
                digit_flag_encountered = true;
            }
        }
    }
    if (ret.empty())
    {
        auto const b = day_begin(n, -days_ago);
        auto const e = day_end(n, -days_ago);
        print_report
        (   p_ordinary_ostream,
            p_config,
            flags,
            p_args.ordinary_args(),
            &b,
            &e
        );
    }
    return ret;
}

}  // namespace swx
