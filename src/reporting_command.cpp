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

#include "reporting_command.hpp"
#include "command.hpp"
#include "help_line.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

// TODO MEDIUM PRIORITY There should be a "brief" option to print a summary of
// totals only, rather than the list plus summary.

namespace swx
{

ReportingCommand::ReportingCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	string const& p_usage_summary,
	vector<HelpLine> const& p_help_line,
	TimeLog& p_time_log
):
	Command(p_command_word, p_aliases, p_usage_summary, p_help_line),
	m_time_log(p_time_log)
{
}

ReportingCommand::~ReportingCommand()
{
}

ostream&
ReportingCommand::print_report
(	ostream& p_os,
	string const* p_activity,
	TimePoint const* p_begin,
	TimePoint const* p_end
)
{
	return p_os << m_time_log.get_stints(p_activity, p_begin, p_end);
}

}  // namespace swx
