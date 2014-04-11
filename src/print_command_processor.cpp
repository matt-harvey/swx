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

#include "print_command_processor.hpp"
#include "command_processor.hpp"
#include "interval.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using std::move;
using std::string;
using std::vector;

namespace swx
{

PrintCommandProcessor::PrintCommandProcessor
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	CommandProcessor(p_command_word, p_aliases),
	m_time_log(p_time_log)
{
}

PrintCommandProcessor::~PrintCommandProcessor()
{
}

CommandProcessor::ErrorMessages
PrintCommandProcessor::do_process
(	Arguments const& p_args,
	std::ostream& p_ordinary_ostream
)
{
	string const activity_name = squish(p_args.begin(), p_args.end());
	p_ordinary_ostream <<
		m_time_log.get_intervals_by_activity_name(move(activity_name));
	return ErrorMessages();
}

vector<CommandProcessor::HelpLine>
PrintCommandProcessor::do_get_help_lines() const
{
	HelpLine const basic_usage_help_line
	(	"ACTIVITY",
		"Print a summary of time spent on ACTIVITY"
	);
	HelpLine const special_usage_help_line
	(	"",
		"Print a summary of time not spent on any activity"
	);
	return vector<HelpLine>{basic_usage_help_line, special_usage_help_line};
}

}  // namespace swx
