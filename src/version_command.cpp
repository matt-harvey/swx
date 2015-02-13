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

#include "version_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include "info.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::string;
using std::vector;

namespace swx
{

VersionCommand::VersionCommand
(	string const& p_command_word,
	vector<string> const& p_aliases
):
	Command
	(	p_command_word,
		p_aliases,
		"Print version information",
		vector<HelpLine>{ HelpLine("Print version information") },
		false
	)
{
}

VersionCommand::~VersionCommand() = default;

Command::ErrorMessages
VersionCommand::do_process
(	Config const& p_config,
	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	(void)p_args;  // suppress warning re. unused param.
	p_ordinary_ostream << Info::application_name()
					   << " version "
					   << Info::version()
					   << endl;
	return ErrorMessages();
}

}  // namespace swx
