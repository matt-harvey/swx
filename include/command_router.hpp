/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_command_router_hpp_6901861572126794
#define GUARD_command_router_hpp_6901861572126794

#include "command_processor.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace swx
{

class CommandRouter
{
// nested types
private:
	typedef
		std::unique_ptr<CommandProcessor>
		CommandProcessorPtr;
	typedef
		std::unordered_map<std::string, CommandProcessorPtr>
		CommandProcessorMap;

// special member functions
public:
	CommandRouter();
	CommandRouter(CommandRouter const& rhs) = delete;
	CommandRouter(CommandRouter&& rhs) = delete;
	CommandRouter& operator=(CommandRouter const& rhs) = delete;
	CommandRouter& operator=(CommandRouter&& rhs) = delete;
	~CommandRouter() = default;

// ordinary member functions
private:

	void populate_command_processor_map();

	void process_command
	(	std::string const& p_command,
		std::vector<std::string> const& p_args
	) const;

	void process_unrecognized_command(std::string const& p_command) const;

// member variables
private:
	CommandProcessorMap m_command_processor_map;

};  // class CommandRouter

}  // namespace swx

#endif  // GUARD_command_router_hpp_6901861572126794
