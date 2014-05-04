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

#include "until_command.hpp"
#include "command.hpp"
#include "help_line.hpp"
#include "parsed_arguments.hpp"
#include "reporting_command.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using std::ostream;
using std::ostringstream;
using std::runtime_error;
using std::string;
using std::unique_ptr;
using std::vector;

namespace swx
{

UntilCommand::UntilCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	ReportingCommand
	(	p_command_word,
		p_aliases,
		"Print summary of activities until a given time",
		vector<HelpLine>
		{	HelpLine
			(	"Print summary of time spent on all activities until "
					"TIMESTAMP",
				"TIMESTAMP"
			),
			HelpLine
			(	"Print summary of time spent on ACTIVITY until TIMESTAMP",
				"TIMESTAMP ACTIVITY"
			)
		},
		p_time_log
	)
{
}

UntilCommand::~UntilCommand()
{
}

Command::ErrorMessages
UntilCommand::do_process
(	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	Arguments const args = p_args.ordinary_args();
	ErrorMessages ret;
	if (args.empty())
	{
		ret.push_back("Too few arguments passed to this command.");
	}
	else
	{
		unique_ptr<TimePoint> time_point_ptr;
		try
		{
			time_point_ptr.reset
			(	new TimePoint(time_stamp_to_point(args[0]))
			);
		}
		catch (runtime_error&)
		{
			ostringstream oss;
			oss << "Could not parse timestamp: " << args[0];
			ret.push_back(oss.str());
			return ret;
		}
		if (args.size() == 1)
		{
			print_report
			(	p_ordinary_ostream,
				nullptr,
				nullptr,
				time_point_ptr.get()
			);
		}
		else
		{
			string const activity = squish(args.begin() + 1, args.end());
			print_report
			(	p_ordinary_ostream,
				&activity,
				nullptr,
				time_point_ptr.get()
			);
		}
	}
	return ret;
}

}  // namespace swx