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
using std::stringstream;
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
    add_option
    (   'a',
        "Instead of today's activities, print the activities of N days ago",
        nullptr,
        &m_days_ago_str,
        "<N>"
    );
}

DayCommand::~DayCommand() = default;

Command::ErrorMessages
DayCommand::do_process
(   Config const& p_config,
    std::vector<std::string> const& p_ordinary_args,
    ostream& p_ordinary_ostream
)
{
    ErrorMessages ret;
    TimePoint const n = now();

    int days_ago = 0;
    stringstream ss(m_days_ago_str);
    ss >> days_ago;
    if (!ss)
    {
        ret.push_back("Could not parse \"" + m_days_ago_str + "\" as numeric argument.");
    }
    if (ret.empty())
    {
        auto const b = day_begin(n, -days_ago);
        auto const e = day_end(n, -days_ago);
        print_report
        (   p_ordinary_ostream,
            p_config,
            p_ordinary_args,
            &b,
            &e
        );
    }
    return ret;
}

}  // namespace swx
