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

#include "switch_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include "placeholder.hpp"
#include "recording_command.hpp"
#include "result.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::ostringstream;
using std::string;
using std::vector;

namespace swx
{

SwitchCommand::SwitchCommand
(   string const& p_command_word,
    vector<string> const& p_aliases,
    TimeLog& p_time_log
):
    RecordingCommand
    (   p_command_word,
        p_aliases,
        "Switch between activities, or cease activity",
        vector<HelpLine>
        {   HelpLine
            (   "Stop accruing time to any activity"
            ),
            HelpLine
            (   "Start accruing time to ACTIVITY, and stop accruing time to "
                    "the current activity (if any); raise an error if ACTIVITY "
                    "does not exist",
                "<ACTIVITY>"
            )
        },
        true,
        p_time_log
    )
{
    add_option
    (   vector<string>{"c", "create"},
        "Create a new activity named ACTIVITY, and start accruing time to it; "
            "raise an error if an activity with this name already exists",
        [this]() { m_create_activity = true; }
    );
    add_option
    (   vector<string>{"r", "regex"},
        "Switch to the most recent activity to match ACTIVITY, considered as "
            "a regular expression",
        [this]() { m_use_regex = true; }
    );
    add_option
    (   vector<string>{"a", "amend"},
        "Instead of switching, retroactively amend the activity of the current "
            "stint to ACTIVITY. If ACTIVITY is not provided, this will cause the "
            "current stint to be erased (i.e. amended to become a period of "
            "inactivity).",
        [this]() { m_amend = true; }
    );
    add_option
    (   vector<string>{"at"},
        HelpLine
        (   "Start accruing time to ACTIVITY (or cessation of activity, as the "
                "case may be) at the time indicated by TIMESTAMP",
            "<TIMESTAMP>"
        ),
        [this]() { m_time_stamp_provided = true; },
        &m_time_stamp
    );
}

SwitchCommand::~SwitchCommand() = default;

Command::ErrorMessages
SwitchCommand::do_process
(   Config const& p_config,
    vector<string> const& p_ordinary_args,
    ostream& p_ordinary_ostream
)
{
    (void)p_config;  // silence compiler re. unused param.

    ErrorMessages error_messages;

    auto const last_two_activities = time_log().last_activities(2);

    // interpret arguments
    vector<string> const args = expand_placeholders(p_ordinary_args, time_log());

    // find the activity that we want to switch to
    string const activity_input(squish(args.begin(), args.end()));
    string activity;
    if (m_use_regex)
    {
        if (activity_input.empty())
        {
            return {"Empty regex not allowed."};
        }
        activity = time_log().last_activity_to_match(activity_input);
        if (activity.empty())
        {
            return {"No activity matches regex: " + activity_input};
        }
    }
    else
    {
        activity = activity_input;
    }

    // capture state of time log
    bool const log_active = time_log().is_active();
    bool const activity_exists = time_log().has_activity(activity);

    // prevent pointless switch
    if (!m_time_stamp_provided)  // but assume not pointless if timestamp provided
    {
        if (activity.empty())
        {
            if (!log_active)
            {
                return {"Already inactive."};
            }
        }
        else if (log_active)
        {
            assert (!last_two_activities.empty());
            if (last_two_activities.front() == activity)
            {
                return {"\"" + activity + "\" is already the current activity."};
            }
        }
    }

    // process switch
    if ((m_create_activity != activity_exists) || activity.empty())
    {
        string cease_message, create_message, existing_message;
        // we can switch or amend
        TimePoint tp;
        if (m_time_stamp_provided)
        {
            auto const tp_result = time_point(m_time_stamp, p_config.time_format());
            if (tp_result.errors().empty())
            {
                tp = tp_result.get();
            }
            else
            {
                return tp_result.errors();
            }
        }
        else
        {
            tp = (m_amend ? time_log().last_entry_time() : now());
        }
        auto const confirmed_stamp = time_point_to_stamp
        (   tp,
            p_config.time_format(),
            p_config.formatted_buf_len()
        );
        if (m_amend)
        {
            auto const penultimate_entry_time = time_log().last_entry_time(1);
            if (tp < penultimate_entry_time)
            {
                return ErrorMessages
                {   "Timestamp must not be earlier than date of previous entry."
                };
            }
            if (last_two_activities.empty())
            {
                return ErrorMessages{"There are no recorded stints to amend."};
            }
            if (last_two_activities.size() >= 2)
            {
                auto const penultimate_activity = last_two_activities[1];
                if ((activity == penultimate_activity) && m_time_stamp_provided)
                {
                    return ErrorMessages
                    {   "Proposed amendment would cause the last stint to merge "
                            "with the one before it.\nTimestamp amendment would "
                            "therefore has had no effect.\nAborted."
                    };
                }
            }
            auto const last_entry_time = time_log().last_entry_time(0);
            auto const last_activity = time_log().amend_last(activity, tp);
            if (activity != last_activity)
            {
                auto const desc = [](string const& p_activity)
                {
                    return p_activity.empty() ? "inactive" : ("\"" + p_activity + "\"");
                };
                p_ordinary_ostream << "Current stint's activity amended from "
                                   << desc(last_activity) << " to "
                                   << desc(activity) << "." << endl;
            }
            if (m_time_stamp_provided)
            {
                auto const last_time_stamp = time_point_to_stamp
                (   last_entry_time,
                    p_config.time_format(),
                    p_config.formatted_buf_len()
                );
                p_ordinary_ostream << "Start time of current stint amended from "
                                   << last_time_stamp << " to "
                                   << confirmed_stamp << "." << endl;
            }
        }
        else
        {
            assert (!m_amend);
            if (tp < time_log().last_entry_time())
            {
                return ErrorMessages
                {   "Timestamp must not be earlier than date of last entry."
                };
            }
            time_log().append_entry(activity, tp);
            if (activity.empty())
            {
                p_ordinary_ostream << "Activity ceased at " << confirmed_stamp;
            }
            else if (m_create_activity)
            {
                p_ordinary_ostream << "Created and switched to \"" << activity
                                   << "\" at " << confirmed_stamp;
            }
            else
            {
                p_ordinary_ostream << "Switched to \"" << activity
                                   << "\" at " << confirmed_stamp;
            }
            p_ordinary_ostream << '.' << endl;
        }
    }
    else
    {
        // we can't switch

        // say why not
        ostringstream oss;
        enable_exceptions(oss);
        if (m_create_activity)
        {
            assert (activity_exists);
            oss << "Activity \"" << activity << "\" already exists.";
        }
        else
        {
            assert (!activity_exists);
            oss << "There is no activity named \"" << activity << "\".\n"
                << "Use -c option to create and switch to a new activity.";
        }

        // report time log state
        if (log_active)
        {
            assert (!time_log().last_activities(1).empty());
            oss << "\nCurrent activity remains \""
                << time_log().last_activities(1).front()
                << "\".";
        }
        else
        {
            oss << "\nRemaining inactive." << endl;
        }
        error_messages.push_back(oss.str());
    }
    return error_messages;
}

bool
SwitchCommand::does_support_placeholders() const
{
    return true;
}

}  // namespace swx
