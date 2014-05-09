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
#include "recording_command.hpp"
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
	char activity_creation_flag()
	{
		return 'c';
	}

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
		p_time_log
	)
{
	add_boolean_option
	(	activity_creation_flag(),
		"Create a new activity named ACTIVITY, and start accruing time to it; "
			"raise an error if an activity with this name already exists"
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
	// TODO LOW PRIORITY Tidy this.
	ErrorMessages error_messages;
	Arguments const args = p_args.ordinary_args();
	TimePoint const time_point = now();
	string const activity(squish(args.begin(), args.end()));
	if (activity.empty() && !time_log().is_active_at(time_point))
	{
		error_messages.push_back("Already inactive.");
		return error_messages;
	}
	if (!activity.empty() && time_log().is_active_at(time_point))
	{
		vector<string> const vec = time_log().last_activities(1);
		assert (!vec.empty());
		if (vec[0] == activity)
		{
			ostringstream oss;
			oss << "Already active: " << activity;
			error_messages.push_back(oss.str());
			return error_messages;
		}
	}
	char const acf = activity_creation_flag();
	if
	(	(p_args.has_flag(acf) && !time_log().has_activity(activity)) ||
		(!p_args.has_flag(acf) && time_log().has_activity(activity)) ||
		activity.empty()
	)
	{
		time_log().append_entry(activity, time_point);
		if (activity.empty())
		{
			p_ordinary_ostream << "Activity ceased." << endl;
		}
		else if (p_args.has_flag(acf))
		{
			p_ordinary_ostream << "Created and switched to new activity: "
			                   << activity << endl;
		}
		else
		{
			p_ordinary_ostream << "Switched to: " << activity << endl;
		}
		return error_messages;
	}
	ostringstream oss;
	if (p_args.has_flag(acf))
	{
		assert (time_log().has_activity(activity));
		oss << "Activity already exists: " << activity;
	}
	else
	{
		assert (!p_args.has_flag(acf));
		assert (!time_log().has_activity(activity));
		oss << "Non-existent activity: " << activity
		    << "\nUse -c option to create and switch to a new activity.";
	}
	if (time_log().is_active_at(time_point))
	{
		oss << "\nCurrent activity remains: "
		    << time_log().last_activities(1).at(0);
	}
	else
	{
		oss << "\nRemaining inactive." << endl;
	}
	error_messages.push_back(oss.str());
	return error_messages;
}

}  // namespace swx
