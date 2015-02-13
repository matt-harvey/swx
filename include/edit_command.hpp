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

#ifndef GUARD_edit_command_hpp_5450043350202052
#define GUARD_edit_command_hpp_5450043350202052

#include "command.hpp"
#include <ostream>
#include <string>

namespace swx
{

// begin forward declarations

class Config;

// end forward declarations

class EditCommand: public Command
{
// special member functions
public:
	EditCommand
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases
	);
	EditCommand(EditCommand const& rhs) = delete;
	EditCommand(EditCommand&& rhs) = delete;
	EditCommand& operator=(EditCommand const& rhs) = delete;
	EditCommand& operator=(EditCommand&& rhs) = delete;
	~EditCommand();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Config const& p_config,
		ParsedArguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

};  // class EditCommand

}  // namespace swx

#endif  // GUARD_edit_command_hpp_5450043350202052
