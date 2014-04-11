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
#include "day_command.hpp"
#include "print_command.hpp"
#include "command.hpp"
#include "help_command.hpp"
#include "info.hpp"
#include "switch_command.hpp"
#include "time_log.hpp"
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
using std::pair;
using std::runtime_error;
using std::set;
using std::string;
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
	(	new SwitchCommand("switch", {"s"}, m_time_log)
	);
	create_command(switch_command);

	CommandPtr print_command
	(	new PrintCommand("print", {"p"}, m_time_log)
	);
	create_command(print_command);

	CommandPtr day_command
	(	new DayCommand("day", {"d"}, m_time_log)
	);
	create_command(day_command);
		
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
		int const ret =
			it->second->process(p_args, ordinary_ostream(), error_ostream());
		if (ret != 0)
		{
			error_ostream() << directions_to_get_help() << endl;
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

vector<pair<string, vector<string>>>
CommandManager::available_commands() const
{
	set<string> command_words;
	vector<pair<string, vector<string>>> ret;
	auto const b = m_command_map.begin();
	auto const e = m_command_map.end();
	for (auto it = b; it != e; ++it)
	{
		ostringstream oss;
		Command const& cp = *it->second;
		string const word = cp.command_word();
		if (command_words.find(word) == command_words.end())
		{
			command_words.insert(word);
			ret.push_back(make_pair(word, cp.aliases()));
		}
	}
	return ret;
}

string
CommandManager::directions_to_get_help()
{
	return
		string("For usage information, enter: '") +
		Info::application_name() +
		' ' +
		help_command_string() +
		"'.";
}

string
CommandManager::error_message_for_unrecognized_command
(	string const& p_command
)
{
	return string("Unrecognized subcommand: ") + p_command;
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
CommandManager::create_command(CommandPtr const& p_cpp)
{
	register_command_word(p_cpp->command_word(), p_cpp);
	for (auto const& alias: p_cpp->aliases())
	{
		register_command_word(alias, p_cpp);
	}
	return;
}

void
CommandManager::register_command_word
(	string const& p_word,
	CommandPtr const& p_cpp
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
	m_command_map[p_word] = p_cpp;
	return;
}

}  // namespace swx
