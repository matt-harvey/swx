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

#ifndef GUARD_help_command_processor_hpp_8550137443724153
#define GUARD_help_command_processor_hpp_8550137443724153

#include "command_processor.hpp"
#include <string>
#include <vector>

namespace swx
{

// begin forward declarations

class CommandRouter;

// end forward declarations

class HelpCommandProcessor: public CommandProcessor
{
// special member functions
public:
	HelpCommandProcessor
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases,
		CommandRouter const& p_command_router
	);

	HelpCommandProcessor(HelpCommandProcessor const& rhs) = delete;
	HelpCommandProcessor(HelpCommandProcessor&& rhs) = delete;
	HelpCommandProcessor& operator=(HelpCommandProcessor const& rhs) = delete;
	HelpCommandProcessor& operator=(HelpCommandProcessor&& rhs) = delete;
	virtual ~HelpCommandProcessor();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

	std::vector<HelpLine> do_get_help_lines() const override;

// member variables
private:
	CommandRouter const& m_command_router;

};  // class HelpCommandProcessor

}  // namespace swx

#endif  // GUARD_help_command_processor_hpp_8550137443724153
