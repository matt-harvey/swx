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

#ifndef GUARD_day_command_hpp_01995176124384243
#define GUARD_day_command_hpp_01995176124384243

#include "reporting_command.hpp"
#include "time_log.hpp"
#include <string>
#include <vector>

namespace swx
{

class DayCommand: public ReportingCommand
{
// special member functions
public:
	DayCommand
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases,
		TimeLog& p_time_log
	);
	DayCommand(DayCommand const& rhs) = delete;
	DayCommand(DayCommand&& rhs) = delete;
	DayCommand& operator=(DayCommand const& rhs) = delete;
	DayCommand& operator=(DayCommand&& rhs) = delete;
	virtual ~DayCommand();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	ParsedArguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

};  // class DayCommand

}  // namespace swx

#endif  // GUARD_day_command_hpp_01995176124384243
