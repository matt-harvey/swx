/*
 * Copyright 2014 Matthew Harvey
 */

#include "command_router.hpp"
#include "command_processor.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::vector;

namespace swx
{

CommandRouter::CommandRouter()
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
	// TODO
}

void
CommandRouter::process_command
(	string const& p_command,
	vector<string> const& p_args
) const
{
	auto const it = m_command_processor_map.find(p_command);
	if (it == m_command_processor_map.end())
	{
		process_unrecognized_command(p_command);
	}
	else
	{
		assert (it->second);
		it->second->process(p_args, ordinary_ostream(), error_ostream());
	}
	return;
}

void
CommandRouter::process_unrecognized_command(string const& p_command) const
{
	error_ostream() << "Unrecognized subcommand: " << p_command << endl;
	return;
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

}  // namespace swx
