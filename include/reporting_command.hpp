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

#ifndef GUARD_reporting_command_hpp_869979514679862
#define GUARD_reporting_command_hpp_869979514679862

#include "command.hpp"
#include "help_line.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

class ReportingCommand: public Command
{
// special member functions
public:
	ReportingCommand
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases,
		std::string const& p_usage_summary,
		std::vector<HelpLine> const& p_help_lines,
		TimeLog& p_time_log
	);
	ReportingCommand(ReportingCommand const& rhs) = delete;
	ReportingCommand(ReportingCommand&& rhs) = delete;
	ReportingCommand& operator=(ReportingCommand const& rhs) = delete;
	ReportingCommand& operator=(ReportingCommand&& rhs) = delete;
	virtual ~ReportingCommand();

// ordinary member functions
protected:
	
	/**
	 * Pass non-null pointers to filter by activity name and/or date
	 * range, or pass null pointers to ignore a particular filter.
	 * Caller retains ownership of pointed-to memory.
	 */
	std::ostream& print_report
	(	std::ostream& p_os,
		std::string const* p_activity = nullptr,
		TimePoint const* p_begin = nullptr,
		TimePoint const* p_end = nullptr
	);

// member variables
private:
	TimeLog& m_time_log;

};  // class ReportingCommand

}  // namespace swx

#endif  // GUARD_reporting_command_hpp_869979514679862
