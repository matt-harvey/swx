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

#ifndef GUARD_today_command_hpp_01995176124384243
#define GUARD_today_command_hpp_01995176124384243

#include "command.hpp"
#include "time_log.hpp"
#include <string>
#include <vector>

namespace swx
{

class TodayCommand: public Command
{
// special member functions
public:
	TodayCommand
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases,
		TimeLog& p_time_log
	);
	TodayCommand(TodayCommand const& rhs) = delete;
	TodayCommand(TodayCommand&& rhs) = delete;
	TodayCommand& operator=(TodayCommand const& rhs) = delete;
	TodayCommand& operator=(TodayCommand&& rhs) = delete;
	virtual ~TodayCommand();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

private:
	TimeLog& m_time_log;

};  // class TodayCommand

}  // namespace swx

#endif  // GUARD_today_command_hpp_01995176124384243
