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

#include "config_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
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

namespace
{
	char filepath_flag()
	{
		return 'f';
	}
}

ConfigCommand::ConfigCommand
(	string const& p_command_word,
	vector<string> const& p_aliases
):
	Command
	(	p_command_word,
		p_aliases,
		"Print configuration information",
		vector<HelpLine>
		{	HelpLine("Print summary of all configuration settings")
		},
		false
	)
{
	add_boolean_option
	(	filepath_flag(),
		"Instead of printing configuration settings, print the location of the "
			"configuration file"
	);
}

ConfigCommand::~ConfigCommand()
{
}

Command::ErrorMessages
ConfigCommand::do_process
(	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	if (p_args.has_flag(filepath_flag()))
	{
		p_ordinary_ostream << Config::filepath() << endl;
	}
	else
	{
		p_ordinary_ostream << Config::summary();
	}
	return ErrorMessages();
}

}  // namespace swx
