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
#include "string_utilities.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <cassert>
#include <iostream>
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

SwitchCommand::SwitchCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	Command
	(	p_command_word,
		p_aliases,
		"Switch between activities, or cease activity",
		vector<HelpLine>
		{	HelpLine
			(	"Stop accruing time to any activity"
			),
			HelpLine
			(	"Start accruing time to ACTIVITY, and stop accruing time to "
					"the current activity (if any)",
				"ACTIVITY"
			)
		}
	),
	m_time_log(p_time_log)
{
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
	(void)p_ordinary_ostream;  // ignore param.
	ErrorMessages error_messages;
	Arguments const args = p_args.ordinary_args();
	TimePoint const time_point = now();
	string const activity(squish(args.begin(), args.end()));
	if (activity.empty() && !m_time_log.is_active_at(time_point))
	{
		error_messages.push_back("Already inactive.");
	}
	else if (!activity.empty() && m_time_log.is_active_at(time_point))
	{
		vector<string> const vec = m_time_log.last_activities(1);
		assert (!vec.empty());
		if (vec[0] == activity)
		{
			ostringstream oss;
			oss << "Already active: " << activity;
			error_messages.push_back(oss.str());
		}
		else
		{
			m_time_log.append_entry(activity, time_point);
		}
	}
	else
	{
		m_time_log.append_entry(activity, time_point);
	}
	return error_messages;
}

}  // namespace swx
