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

#ifndef GUARD_print_command_23685825347754091
#define GUARD_print_command_23685825347754091

#include "reporting_command.hpp"
#include "time_log.hpp"
#include <string>
#include <vector>

namespace swx
{

class PrintCommand: public ReportingCommand
{
// special member functions
public:
	PrintCommand
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases,
		TimeLog& p_time_log
	);
	PrintCommand(PrintCommand const& rhs) = delete;
	PrintCommand(PrintCommand&& rhs) = delete;
	PrintCommand& operator=(PrintCommand const& rhs) = delete;
	PrintCommand& operator=(PrintCommand&& rhs) = delete;
	virtual ~PrintCommand();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

};  // class PrintCommand

}  // namespace swx

#endif  // GUARD_print_command_hpp_23685825347754091
