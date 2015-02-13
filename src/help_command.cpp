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

#include "help_command.hpp"
#include "help_line.hpp"
#include "command.hpp"
#include "command_manager.hpp"
#include "config.hpp"
#include "info.hpp"
#include "stream_utilities.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::endl;
using std::ostream;
using std::runtime_error;
using std::string;
using std::vector;

namespace swx
{

HelpCommand::HelpCommand
(	string const& p_command_word,
	vector<string> const& p_aliases,
	CommandManager const& p_command_manager
):
	Command
	(	p_command_word,
		p_aliases,
		"Print usage information",
		vector<HelpLine>
		{	HelpLine("Print general usage information"),
			HelpLine("Print usage information for COMMAND", "<COMMAND>")
		}
	),
	m_command_manager(p_command_manager)
{
}

HelpCommand::~HelpCommand() = default;

Command::ErrorMessages
HelpCommand::do_process
(	Config const& p_config,
	ParsedArguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	(void)p_config;  // silence compiler re. unused param
	ErrorMessages ret;
	Arguments const args = p_args.ordinary_args();
	if (args.empty())
	{
		p_ordinary_ostream << m_command_manager.help_information();	
	}
	else if (args.size() == 1)
	{
		try
		{
			p_ordinary_ostream << m_command_manager.help_information(args[0])
							   << endl;			
		}
		catch (runtime_error&)
		{
			ret.push_back
			(	CommandManager::error_message_for_unrecognized_command(args[0])
			);
		}
	}
	else
	{
		assert (args.size() > 1);
		ret.push_back("Too many arguments passed to this command.");
	}
	return ret;	
}

}  // namespace swx
