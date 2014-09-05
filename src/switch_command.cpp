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

namespace
{
	char const k_creation_flag = 'c';
	char const k_regex_flag = 'r';

}  // end anonymous namespace

SwitchCommand::SwitchCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	RecordingCommand
	(	p_command_word,
		p_aliases,
		"Switch between activities, or cease activity",
		vector<HelpLine>
		{	HelpLine
			(	"Stop accruing time to any activity"
			),
			HelpLine
			(	"Start accruing time to ACTIVITY, and stop accruing time to "
					"the current activity (if any); raise an error if ACTIVITY "
					"does not exist",
				"ACTIVITY"
			)
		},
		true,
		p_time_log
	)
{
	add_boolean_option
	(	k_creation_flag,
		"Create a new activity named ACTIVITY, and start accruing time to it; "
			"raise an error if an activity with this name already exists"
	);
	add_boolean_option
	(	k_regex_flag,
		"Switch to the most recent activity to match ACTIVITY, considered as "
			"a (POSIX extended) regular expression"
	);
}

SwitchCommand::~SwitchCommand()
{
}

Command::ErrorMessages
SwitchCommand::do_process
(	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	ErrorMessages error_messages;

	// interpret arguments
	bool const using_regex = p_args.has_flag(k_regex_flag);
	bool const creating_activity = p_args.has_flag(k_creation_flag);
	Arguments const args =
		expand_placeholders(p_args.ordinary_args(), time_log());

	// find the activity that we want to switch to
	string const activity_input(squish(args.begin(), args.end()));
	string activity;
	if (using_regex)
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
	if ((creating_activity != activity_exists) || activity.empty())
	{
		// we can switch
		time_log().append_entry(activity);
		if (activity.empty())
		{
			p_ordinary_ostream << "Activity ceased." << endl;
		}
		else if (creating_activity)
		{
			p_ordinary_ostream << "Created and switched to new activity: "
			                   << activity << endl;
		}
		else
		{
			assert (activity_exists);
			p_ordinary_ostream << "Switched to: " << activity << endl;
		}
	}
	else
	{
		// we can't switch

		// say why not
		ostringstream oss;
		enable_exceptions(oss);
		if (creating_activity)
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
