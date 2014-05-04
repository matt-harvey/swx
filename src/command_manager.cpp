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

#include "command_manager.hpp"
#include "between_command.hpp"
#include "command.hpp"
#include "help_command.hpp"
#include "info.hpp"
#include "parsed_arguments.hpp"
#include "print_command.hpp"
#include "resume_command.hpp"
#include "since_command.hpp"
#include "switch_command.hpp"
#include "today_command.hpp"
#include "time_log.hpp"
#include "until_command.hpp"
#include "version_command.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::ostringstream;
using std::runtime_error;
using std::set;
using std::string;
using std::tuple;
using std::vector;

namespace swx
{

namespace
{
	string help_command_string()
	{
		return "help";
	}

}  // end anonymous namespace

CommandManager::CommandManager(TimeLog& p_time_log): m_time_log(p_time_log)
{
	populate_command_map();	
#	ifndef NDEBUG
		for (auto const& entry: m_command_map)
		{
			assert (entry.second);
		}
#	endif
}

void
CommandManager::populate_command_map()
{
	CommandPtr version_command
	(	new VersionCommand("version", {"v"})
	);
	create_command(version_command);	
	
	CommandPtr help_command
	(	new HelpCommand(help_command_string(), {"h"}, *this)
	);
	create_command(help_command);

	CommandPtr switch_command
	(	new SwitchCommand("switch", {"sw"}, m_time_log)
	);
	create_command(switch_command);

	CommandPtr resume_command
	(	new ResumeCommand("resume", {"r"}, m_time_log)
	);
	create_command(resume_command);

	CommandPtr print_command
	(	new PrintCommand("print", {"p"}, m_time_log)
	);
	create_command(print_command);

	CommandPtr today_command
	(	new TodayCommand("today", {"t"}, m_time_log)
	);
	create_command(today_command);

	CommandPtr since_command
	(	new SinceCommand("since", {"si"}, m_time_log)
	);
	create_command(since_command);

	CommandPtr until_command
	(	new UntilCommand("until", {"u"}, m_time_log)
	);
	create_command(until_command);

	CommandPtr between_command
	(	new BetweenCommand("between", {"b"}, m_time_log)
	);
	create_command(between_command);

	return;
}

int
CommandManager::process_command
(	string const& p_command,
	vector<string> const& p_args
) const
{
	auto const it = m_command_map.find(p_command);
	if (it == m_command_map.end())
	{
		return process_unrecognized_command(p_command);
	}
	else
	{
		assert (it->second);
		auto const command_ptr = it->second;
		int const ret = command_ptr->process
		(	ParsedArguments(p_args),
			ordinary_ostream(),
			error_ostream()
		);
		if (ret != 0)
		{
			error_ostream() << directions_to_get_help(p_command) << endl;
		}
		return ret;
	}
}

string
CommandManager::help_information(string const& p_command) const
{
	ostringstream oss;
	auto const it = m_command_map.find(p_command);
	if (it == m_command_map.end())
	{
		throw runtime_error
		(	error_message_for_unrecognized_command(p_command)
		);
	}
	return it->second->usage_descriptor();
}

vector<tuple<string, vector<string>, string>>
CommandManager::available_commands() const
{
	set<string> command_words;
	vector<tuple<string, vector<string>, string>> ret;
	auto const b = m_command_map.begin();
	auto const e = m_command_map.end();
	for (auto it = b; it != e; ++it)
	{
		ostringstream oss;
		Command const& c = *it->second;
		string const word = c.command_word();
		if (command_words.find(word) == command_words.end())
		{
			command_words.insert(word);
			ret.push_back(make_tuple(word, c.aliases(), c.usage_summary()));
		}
	}
	return ret;
}

string
CommandManager::directions_to_get_help()
{
	ostringstream oss;
	oss << "For usage information, enter '"
	    << Info::application_name()
		<< ' '
		<< help_command_string()
		<< "'.";
	return oss.str();
}

string
CommandManager::directions_to_get_help(string const& p_command)
{
	ostringstream oss;
	oss << "For usage information, enter '"
	    << Info::application_name()
		<< ' '
		<< help_command_string()
		<< ' '
		<< p_command
		<< "'.";
	return oss.str();
}

string
CommandManager::error_message_for_unrecognized_command
(	string const& p_command
)
{
	return string("Unrecognized command: ") + p_command;
}

int
CommandManager::process_unrecognized_command(string const& p_command) const
{
	error_ostream() << error_message_for_unrecognized_command(p_command)
	                << endl
					<< directions_to_get_help()
					<< endl;
	return 1;
}

ostream&
CommandManager::ordinary_ostream() const
{
	return cout;	
}

ostream&
CommandManager::error_ostream() const
{
	return cerr;
}

void
CommandManager::create_command(CommandPtr const& p_cp)
{
	register_command_word(p_cp->command_word(), p_cp);
	for (auto const& alias: p_cp->aliases())
	{
		register_command_word(alias, p_cp);
	}
	return;
}

void
CommandManager::register_command_word
(	string const& p_word,
	CommandPtr const& p_cp
)
{
	if (m_command_map.find(p_word) != m_command_map.end())
	{
		ostringstream oss;
		oss << "Command word \""
		    << p_word
			<< "\" has already been registered.";
		throw std::runtime_error(oss.str());
	}
	m_command_map[p_word] = p_cp;
	return;
}

}  // namespace swx
