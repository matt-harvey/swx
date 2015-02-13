/*
 * Copyright 2015 Matthew Harvey
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

#include "current_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include "time_log.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::string;
using std::vector;

namespace swx
{

namespace
{
	char const k_suppress_newline_flag = 's';

}  // end anonymous namespace

CurrentCommand::CurrentCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	Command
	(	p_command_word,
		p_aliases,
		"Print name of current activity",
		vector<HelpLine>
		{	HelpLine
			(	"Print the name of the current activity, or print a blank line if "
					"currently inactive"
			)
		},
		false
	),
	m_time_log(p_time_log)
{
	add_boolean_option(k_suppress_newline_flag, "Do not terminate output with a newline");
}

CurrentCommand::~CurrentCommand() = default;

Command::ErrorMessages
CurrentCommand::do_process
(	Config const& p_config,
	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	(void)p_config;  // silence compiler re. unused param
	TimePoint const n = now();
	auto const stints = m_time_log.get_stints(nullptr, &n, &n);
	if (!stints.empty())
	{
		p_ordinary_ostream << stints[0].activity();
	}
	if (!p_args.flags().count(k_suppress_newline_flag))
	{
		p_ordinary_ostream << endl;
	}
	return ErrorMessages();
}

}  // namespace swx
