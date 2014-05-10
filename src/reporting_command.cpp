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
#include "placeholder.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <memory>
#include <ostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::unique_ptr;
using std::vector;

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
	add_boolean_option
	(	'l',
		"Instead of printing a summary, print a list of individual "
			"activity stints during the relevant period"
	);
	add_boolean_option
	(	'v',
		"Print both a list of individual activity stints, and a "
			"summary of the time spent on each activity"
	);
	add_boolean_option
	(	'r',
		"Treat ACTIVITY as a (POSIX extended) regular expression, and include "
			"all activities that match it"
	);
}

ReportingCommand::~ReportingCommand()
{
}

ostream&
ReportingCommand::print_report
(	ostream& p_os,
	string const& p_options,
	string const* p_activity,
	TimePoint const* p_begin,
	TimePoint const* p_end
)
{
	bool detail = false;
	bool summary = true;
	bool use_regex = false;
	for (char c: p_options)
	{
		switch (c)
		{
		case 'l':
			detail = true;
			summary = false;
			break;
		case 'r':
			use_regex = true;
			break;
		case 'v':
			detail = true;
			summary = true;
			break;
		}
	}
	// TODO MEDIUM PRIORITY Activity args and squished and expanded multiple
	// times on the way to being processed. This is completely unnecessary.
	unique_ptr<string> activity_ptr;
	if (p_activity)
	{
		vector<string> const expanded =
			expand_placeholders(split(*p_activity, ' '), m_time_log);
		activity_ptr.reset
		(	new string(squish(expanded.begin(), expanded.end()))
		);
	}
	return print_stints_report
	(	p_os,
		m_time_log.get_stints(activity_ptr.get(), p_begin, p_end, use_regex),
		summary,
		detail
	);
}

string
ReportingCommand::do_get_category() const
{
	return "Reporting";
}

bool
ReportingCommand::does_support_placeholders() const
{
	return true;
}

}  // namespace swx
