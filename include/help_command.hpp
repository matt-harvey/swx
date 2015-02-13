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

#ifndef GUARD_help_command_hpp_8550137443724153
#define GUARD_help_command_hpp_8550137443724153

#include "command.hpp"
#include <string>
#include <vector>

namespace swx
{

// begin forward declarations

class CommandManager;
class Config;

// end forward declarations

class HelpCommand: public Command
{
// special member functions
public:
	HelpCommand
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases,
		CommandManager const& p_command_manager
	);

	HelpCommand(HelpCommand const& rhs) = delete;
	HelpCommand(HelpCommand&& rhs) = delete;
	HelpCommand& operator=(HelpCommand const& rhs) = delete;
	HelpCommand& operator=(HelpCommand&& rhs) = delete;
	virtual ~HelpCommand();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Config const& p_config,
		ParsedArguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

// member variables
private:
	CommandManager const& m_command_manager;

};  // class HelpCommand

}  // namespace swx

#endif  // GUARD_help_command_hpp_8550137443724153
