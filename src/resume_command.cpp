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
#include "help_line.hpp"
#include "parsed_arguments.hpp"
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

ResumeCommand::ResumeCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	Command
	(	p_command_word,
		p_aliases,
		"Resume previous activity",
		vector<HelpLine>
		{	HelpLine
			(	"If currently inactive, start accruing time to the most recent "
				"activity; or if already active, switch to the activity that "
				"was last active before the current one"
			)
		}
	),
	m_time_log(p_time_log)
{
}

ResumeCommand::~ResumeCommand()
{
}

Command::ErrorMessages
ResumeCommand::do_process
(	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	Arguments const args = p_args.ordinary_args();
	if (!args.empty())
	{
		ret.push_back("Too many arguments passed to this command.");
	}
	else
	{
		auto const last_activities = m_time_log.last_activities(2);
		auto const n = now();
		bool const is_active = m_time_log.is_active_at(n);
		if (last_activities.empty())
		{
			ret.push_back("No activity has yet been recorded.");
		}
		else if (!is_active)
		{
			auto const activity = last_activities[0];
			m_time_log.append_entry(activity, n);
			p_ordinary_ostream << "Resumed: " << activity << endl;
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
			m_time_log.append_entry(activity, n);
			p_ordinary_ostream << "Resumed: " << activity << endl;
		}
	}
	return ret;
}

}  // namespace swx
