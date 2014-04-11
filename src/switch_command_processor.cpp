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

#include "switch_command_processor.hpp"
#include "command_processor.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::move;
using std::ostream;
using std::ostringstream;
using std::string;
using std::vector;

namespace swx
{

SwitchCommandProcessor::SwitchCommandProcessor
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	CommandProcessor(p_command_word, p_aliases),
	m_time_log(p_time_log)
{
}

SwitchCommandProcessor::~SwitchCommandProcessor()
{
}

CommandProcessor::ErrorMessages
SwitchCommandProcessor::do_process
(	Arguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	(void)p_ordinary_ostream;  // ignore param.
	TimePoint const time_point = now();
	string const activity_name(squish(p_args.begin(), p_args.end()));
	m_time_log.append_entry(move(activity_name), time_point);
	return ErrorMessages();
}

vector<CommandProcessor::HelpLine>
SwitchCommandProcessor::do_get_help_lines() const
{
	HelpLine const switching_line
	(	"ACTIVITY",
		"Start accruing time to ACTIVITY, and stop accruing time to the "
			"current activity (if any)"
	);
	HelpLine const stopping_line
	(	"",
		"Stop accruing time to any activity"
	);
	return vector<HelpLine>{switching_line, stopping_line};
}

}  // namespace swx
