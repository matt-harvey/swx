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

#ifndef GUARD_yesterday_command_hpp_3429149679573788
#define GUARD_yesterday_command_hpp_3429149679573788

#include "reporting_command.hpp"
#include "time_log.hpp"
#include <string>
#include <vector>

namespace swx
{

class YesterdayCommand: public ReportingCommand
{
// special member functions
public:
	YesterdayCommand
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases,
		TimeLog& p_time_log
	);
	YesterdayCommand(YesterdayCommand const& rhs) = delete;
	YesterdayCommand(YesterdayCommand&& rhs) = delete;
	YesterdayCommand& operator=(YesterdayCommand const& rhs) = delete;
	YesterdayCommand& operator=(YesterdayCommand&& rhs) = delete;
	virtual ~YesterdayCommand();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	ParsedArguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

};  // class YesterdayCommand

}  // namespace swx

#endif  // GUARD_yesterday_command_hpp_3429149679573788
