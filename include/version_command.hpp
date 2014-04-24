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

#ifndef GUARD_version_command_hpp_12242928944641225
#define GUARD_version_command_hpp_12242928944641225

#include "command.hpp"
#include <ostream>
#include <string>

namespace swx
{

class VersionCommand: public Command
{
// special member functions
public:
	VersionCommand
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases
	);
	VersionCommand(VersionCommand const& rhs) = delete;
	VersionCommand(VersionCommand&& rhs) = delete;
	VersionCommand& operator=(VersionCommand const& rhs) = delete;
	VersionCommand& operator=(VersionCommand&& rhs) = delete;
	virtual ~VersionCommand();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

};  // class VersionCommand

}  // namespace swx

#endif  // GUARD_version_command_hpp_12242928944641225
