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

#include "print_command.hpp"
#include "command.hpp"
#include "help_line.hpp"
#include "reporting_command.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

namespace swx
{

PrintCommand::PrintCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	ReportingCommand
	(	p_command_word,
		p_aliases,
		"Print summary of activities",
		vector<HelpLine>
		{	HelpLine("Print summary of time spent on all activities."),
			HelpLine("Print summary of time spent on ACTIVITY", "ACTIVITY")
		},
		p_time_log
	)
{
}

PrintCommand::~PrintCommand()
{
}

Command::ErrorMessages
PrintCommand::do_process
(	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	Arguments const args = p_args.ordinary_args();
	if (args.empty())
	{
		print_report(p_ordinary_ostream, p_args.single_character_flags());
	}
	else
	{
		string const activity = squish(args.begin(), args.end());
		print_report
		(	p_ordinary_ostream,
			p_args.single_character_flags(),
			&activity
		);
	}
	return ret;
}

}  // namespace swx
