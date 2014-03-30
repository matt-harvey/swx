/*
 * Copyright 2014 Matthew Harvey
 */

#include "command_router.hpp"
#include "print_command_processor.hpp"
#include "command_processor.hpp"
#include "help_command_processor.hpp"
#include "info.hpp"
#include "switch_command_processor.hpp"
#include "time_log.hpp"
#include "version_command_processor.hpp"
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

CommandRouter::CommandRouter(TimeLog& p_time_log): m_time_log(p_time_log)
{
	populate_command_processor_map();	
#	ifndef NDEBUG
		for (auto const& entry: m_command_processor_map)
		{
			assert (entry.second);
		}
#	endif
}

void
CommandRouter::populate_command_processor_map()
{
	CommandProcessorPtr version_processor
	(	new VersionCommandProcessor("version", {"v"})
	);
	create_command(version_processor);	
	
	CommandProcessorPtr help_processor
	(	new HelpCommandProcessor(help_command_string(), {"h"}, *this)
	);
	create_command(help_processor);

	CommandProcessorPtr switch_processor
	(	new SwitchCommandProcessor("switch", {"s"}, m_time_log)
	);
	create_command(switch_processor);

	CommandProcessorPtr print_processor
	(	new PrintCommandProcessor("print", {"p"}, m_time_log)
	);
	create_command(print_processor);
		
	return;
}

int
CommandRouter::process_command
(	string const& p_command,
	vector<string> const& p_args
) const
{
	auto const it = m_command_processor_map.find(p_command);
	if (it == m_command_processor_map.end())
	{
		process_unrecognized_command(p_command);
		return 1;
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
CommandRouter::help_information(string const& p_command) const
{
	ostringstream oss;
	auto const it = m_command_processor_map.find(p_command);
	if (it == m_command_processor_map.end())
	{
		throw runtime_error
		(	error_message_for_unrecognized_subcommand(p_command)
		);
	}
	return it->second->usage_descriptor();
}

vector<pair<string, vector<string>>>
CommandRouter::available_commands() const
{
	set<string> command_words;
	vector<pair<string, vector<string>>> ret;
	auto const b = m_command_processor_map.begin();
	auto const e = m_command_processor_map.end();
	for (auto it = b; it != e; ++it)
	{
		ostringstream oss;
		CommandProcessor const& cp = *it->second;
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
CommandRouter::directions_to_get_help()
{
	return
		string("For usage information, enter: '") +
		Info::application_name() +
		' ' +
		help_command_string() +
		"'.";
}

string
CommandRouter::error_message_for_unrecognized_subcommand
(	string const& p_command
)
{
	return string("Unrecognized subcommand: ") + p_command;
}

int
CommandRouter::process_unrecognized_command(string const& p_command) const
{
	error_ostream() << error_message_for_unrecognized_subcommand(p_command)
	                << endl
					<< directions_to_get_help()
					<< endl;
	return 1;
}

ostream&
CommandRouter::ordinary_ostream() const
{
	return cout;	
}

ostream&
CommandRouter::error_ostream() const
{
	return cerr;
}

void
CommandRouter::create_command(CommandProcessorPtr const& p_cpp)
{
	register_command_word(p_cpp->command_word(), p_cpp);
	for (auto const& alias: p_cpp->aliases())
	{
		register_command_word(alias, p_cpp);
	}
	return;
}

void
CommandRouter::register_command_word
(	string const& p_word,
	CommandProcessorPtr const& p_cpp
)
{
	if (m_command_processor_map.find(p_word) != m_command_processor_map.end())
	{
		ostringstream oss;
		oss << "Command processor word \""
		    << p_word
			<< "\" has already been registered.";
		throw std::runtime_error(oss.str());
	}
	m_command_processor_map[p_word] = p_cpp;
	return;
}

}  // namespace swx
