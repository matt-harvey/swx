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

#include "today_command.hpp"
#include "command.hpp"
#include "help_line.hpp"
#include "stint.hpp"
#include "string_utilities.hpp"
#include "time_point.hpp"
#include <ostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace swx
{

TodayCommand::TodayCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	Command
	(	p_command_word,
		p_aliases,
		vector<HelpLine>
		{	HelpLine("Print summary of time spent on all activities today"),
			HelpLine
			(	"Print summary of time spent on ACTIVITY today",
				"ACTIVITY"
			)
		}
	),
	m_time_log(p_time_log)
{
}

TodayCommand::~TodayCommand()
{
}

Command::ErrorMessages
TodayCommand::do_process
(	Arguments const& p_args,
	std::ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	TimePoint const n = now();
	auto const b = day_begin(n);
	auto const e = day_end(n);
	if (p_args.empty())
	{
		p_ordinary_ostream << m_time_log.get_stints(nullptr, &b, &e);
	}
	else
	{
		auto const activity = squish(p_args.begin(), p_args.end());
		p_ordinary_ostream << m_time_log.get_stints(&activity, &b, &e);
	}
	return ret;
}

}  // namespace swx
