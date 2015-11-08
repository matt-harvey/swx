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
#include "help_line.hpp"
#include "placeholder.hpp"
#include "recording_command.hpp"
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
    (   'c',
        "Create a new activity named ACTIVITY, and start accruing time to it; "
            "raise an error if an activity with this name already exists",
        [this]() { m_create_activity = true; }
    );
    add_option
    (   'r',
        "Switch to the most recent activity to match ACTIVITY, considered as "
            "a regular expression",
        [this]() { m_use_regex = true; }
    );
    add_option
    (   'a',
        "Instead of switching, retroactively amend the activity of the current "
            "stint to ACTIVITY. If ACTIVITY is not provided, this will cause the "
            "current stint to be erased (i.e. amended to become a period of "
            "inactivity).",
        [this]() { m_amend = true; }
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
    if (activity.empty())
    {
        if (!log_active)
        {
            return {"Already inactive."};
        }
    }
    else
    {
        if (log_active)
        {
            vector<string> const vec = time_log().last_activities(1);
            assert (!vec.empty());
            if (vec[0] == activity)
            {
                return {"Already active: " + activity};
            }
        }
    }

    // process switch
    if ((m_create_activity != activity_exists) || activity.empty())
    {
        string cease_message, create_message, existing_message;
        // we can switch or amend
        if (m_amend)
        {
            if (time_log().last_activities(1).empty())
            {
                cease_message = create_message = existing_message =
                    "There are no recorded stints to amend.";
            }
            else
            {
                auto const last = time_log().amend_last(activity);
                cease_message = "Current stint erased.\nWas: " + last;
                create_message = "Amended current stint\nWas: " + last + "\nNow: " + activity;
                existing_message = create_message;
            }
        }
        else
        {
            time_log().append_entry(activity);
            cease_message = "Activity ceased.";
            create_message = "Created and switched to new activity: "  + activity;
            existing_message = "Switched to: " + activity;
        }
        if (activity.empty()) p_ordinary_ostream << cease_message;
        else if (m_create_activity) p_ordinary_ostream << create_message;
        else p_ordinary_ostream << existing_message;
        p_ordinary_ostream << endl;
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
            oss << "Activity already exists: " << activity;
        }
        else
        {
            assert (!activity_exists);
            oss << "Non-existent activity: " << activity
                << "\nUse -c option to create and switch to a new activity.";
        }

        // report time log state
        if (log_active)
        {
            assert (!time_log().last_activities(1).empty());
            oss << "\nCurrent activity remains: "
                << time_log().last_activities(1).front();
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
