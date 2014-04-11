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

#include "help_command_processor.hpp"
#include "command_processor.hpp"
#include "command_router.hpp"
#include "info.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::endl;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::runtime_error;
using std::string;
using std::vector;

namespace swx
{

HelpCommandProcessor::HelpCommandProcessor
(	string const& p_command_word,
	vector<string> const& p_aliases,
	CommandRouter const& p_command_router
):
	CommandProcessor(p_command_word, p_aliases),
	m_command_router(p_command_router)
{
}

HelpCommandProcessor::~HelpCommandProcessor()
{
}

CommandProcessor::ErrorMessages
HelpCommandProcessor::do_process
(	Arguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	ErrorMessages ret;
	if (p_args.empty())
	{
		vector<pair<string, vector<string>>> subcommands =
			m_command_router.available_commands();
		ostringstream oss;
		for (auto const& pair: subcommands)
		{
			oss << pair.first;
			if (!pair.second.empty())
			{
				for (auto const& alias: pair.second)
				{
					oss << ", " << alias;
				}
			}
			oss << '\n';
		}
		p_ordinary_ostream << "Usage: "
		                   << Info::application_name() << ' '
						   << "SUBCOMMAND [ARGS..]" << endl
						   << "\nAvailable subcommands are:\n\n"
		                   << oss.str() << "\n"
						   << "For more information on a particular subcommand,"
						   << " enter '"
						   << Info::application_name() << ' '
						   << command_word() << " SUBCOMMAND'." << endl;
		return ret;
	}
	else if (p_args.size() == 1)
	{
		try
		{
			p_ordinary_ostream << m_command_router.help_information(p_args[0])
							   << endl;			
		}
		catch (runtime_error&)
		{
			ostringstream oss;
			oss << CommandRouter::
					  error_message_for_unrecognized_command(p_args[0]);
			ret.push_back(oss.str());
		}
	}
	else
	{
		assert (p_args.size() > 1);
		ret.push_back("Too many arguments passed to this subcommand.");
	}
	return ret;	
}


std::vector<CommandProcessor::HelpLine>
HelpCommandProcessor::do_get_help_lines() const
{
	HelpLine const general_help_line("", "Print general help information");
	HelpLine const specific_help_line
	(	"SUBCOMMAND",
		"Print specific help information for SUBCOMMAND"
	);
	return vector<HelpLine>{general_help_line, specific_help_line};
}


}  // namespace swx
