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

#include "resume_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include "placeholder.hpp"
#include "recording_command.hpp"
#include "result.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::string;
using std::vector;

namespace swx
{

ResumeCommand::ResumeCommand
(   string const& p_command_word,
    vector<string> const& p_aliases,
    TimeLog& p_time_log
):
    RecordingCommand
    (   p_command_word,
        p_aliases,
        "Resume previous activity",
        vector<HelpLine>
        {   HelpLine
            (   "If currently inactive, start accruing time to the most recent "
                "activity; or if already active, switch to the activity that "
                "was last active before the current one"
            )
        },
        false,
        p_time_log
    )
{
    add_option
    (   vector<string>{"at"},
        HelpLine
        (   "Start accruing time to activity at the time indicated by TIMESTAMP",
            "<TIMESTAMP>"
        ),
        [this]() { m_time_stamp_provided = true; },
        &m_time_stamp
    );
}

ResumeCommand::~ResumeCommand() = default;

Command::ErrorMessages
ResumeCommand::do_process
(   Config const& p_config,
    vector<string> const& p_ordinary_args,
    ostream& p_ordinary_ostream
)
{
    (void)p_ordinary_args; (void)p_config;  // silence compiler re. unused params

    ErrorMessages ret;
    bool const is_active = time_log().is_active();
    auto const last_activities = time_log().last_activities(2);

    TimePoint tp;
    if (m_time_stamp_provided)
    {
        auto const tp_result = time_point(m_time_stamp, p_config.time_format());
        if (tp_result.errors().empty())
        {
            tp = tp_result.get();
            if (tp < time_log().last_entry_time())
            {
                return ErrorMessages
                {   "Timestamp must not be earlier than date of last entry."
                };
            }
        }
        else
        {
            return tp_result.errors();
        }
    }
    else
    {
        tp = now();
    }
    auto const confirmed_stamp = 
        time_point_to_stamp(tp, p_config.time_format(), p_config.formatted_buf_len());

    if (last_activities.empty())
    {
        ret.push_back("No activity has yet been recorded.");
    }
    else if (!is_active)
    {
        auto const activity = last_activities[0];
        time_log().append_entry(activity, tp);
        p_ordinary_ostream << "Resumed \"" << activity << "\" at "
                           << confirmed_stamp << '.' << endl;
    }
    else if (last_activities.size() == 1)
    {
        assert (is_active);
        ret.push_back("No prior activity to resume.");
    }
    else
    {
        assert (is_active);
        assert (last_activities.size() == 2);
        auto const activity = last_activities[1];
        time_log().append_entry(activity, tp);
        p_ordinary_ostream << "Resumed \"" << activity << "\" at "
                           << confirmed_stamp << '.' << endl;
    }
    return ret;
}

}  // namespace swx
