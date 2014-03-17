/*
 * Copyright 2014 Matthew Harvey
 */

#include "command_router.hpp"
#include "command_processor.hpp"
#include <cassert>
#include <string>
#include <vector>

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
		it->second->process(p_args);
	}
	return;
}

void
CommandRouter::process_unrecognized_command(string const& p_command) const
{
	// TODO	
}

}  // namespace swx
