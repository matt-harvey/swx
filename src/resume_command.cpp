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
		vector<HelpLine>
		{	HelpLine
			(	"If currently inactive, start accruing time to the most recent "
				"activity; or if already active, do nothing."
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
(	Arguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	auto const last_activities = m_time_log.last_activities(2);
	string const no_activity_message("No activity has yet been recorded.");
	if (last_activities.empty())
	{
		ret.push_back(no_activity_message);
	}
	else if (last_activities.front().empty())
	{
		switch (last_activities.size())
		{
		case 1:
			ret.push_back(no_activity_message);
			break;
		case 2:
			assert (!last_activities.back().empty());
			m_time_log.append_entry(last_activities.back(), now());
			p_ordinary_ostream << "Resumed: " << last_activities.back() << endl;
			break;
		default:
			assert (false);
		}
	}
	else
	{
		p_ordinary_ostream << "Already active: "
		                   << last_activities.front()
						   << endl;
	}
	return ret;
}

}  // namespace swx
