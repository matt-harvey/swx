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
#include "stint.hpp"
#include "time_point.hpp"
#include <ostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace swx
{

DayCommand::DayCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	Command(p_command_word, p_aliases),
	m_time_log(p_time_log)
{
}

DayCommand::~DayCommand()
{
}

Command::ErrorMessages
DayCommand::do_process
(	Arguments const& p_args,
	std::ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	if (p_args.empty())
	{
		TimePoint const n = now();
		auto const b = day_begin(n);
		auto const e = day_end(n);
		p_ordinary_ostream << m_time_log.get_stints(nullptr, &b, &e);
	}
	else
	{
		ret.push_back("Too many arguments passed to this subcommand.");
	}
	return ret;
}

vector<Command::HelpLine>
DayCommand::do_get_help_lines() const
{
	HelpLine const help_line("", "Print summary of today's activities");
	return vector<HelpLine>{help_line};
}

}  // namespace swx
