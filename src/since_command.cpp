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

#include "since_command.hpp"
#include "command.hpp"
#include "help_line.hpp"
#include "reporting_command.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <cassert>
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

SinceCommand::SinceCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	TimeLog& p_time_log
):
	ReportingCommand
	(	p_command_word,
		p_aliases,
		"Print summary of activities since a given time",
		vector<HelpLine>
		{	HelpLine
			(	"Print summary of time spent on all activities since "
					"TIMESTAMP",
				"<TIMESTAMP>"
			),
			HelpLine
			(	"Print summary of time spent on ACTIVITY since TIMESTAMP",
				"<TIMESTAMP> <ACTIVITY>"
			)
		},
		p_time_log
	)
{
}

SinceCommand::~SinceCommand() = default;

Command::ErrorMessages
SinceCommand::do_process
(	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	Arguments const oargs = p_args.ordinary_args();
	if (oargs.empty())
	{
		return {"Too few arguments passed to this command."};
	}
	else
	{
		unique_ptr<TimePoint> time_point_ptr;
		try
		{
			time_point_ptr.reset
			(	new TimePoint(time_stamp_to_point(oargs[0]))
			);
		}
		catch (runtime_error&)
		{
			ostringstream oss;
			enable_exceptions(oss);
			oss << "Could not parse timestamp: " << oargs[0];
			return {oss.str()};
		}
		assert (oargs.size() >= 1);
		print_report
		(	p_ordinary_ostream,
			p_args.single_character_flags(),
			vector<string>(oargs.begin() + 1, oargs.end()),
			time_point_ptr.get()
		);
	}
	return ErrorMessages();
}

}  // namespace swx
