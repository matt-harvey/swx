/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_command_router_hpp_6901861572126794
#define GUARD_command_router_hpp_6901861572126794

#include "command_processor.hpp"
#include "time_log.hpp"
#include <memory>
#include <ostream>
#include <string>
#include <map>
#include <vector>

namespace swx
{

class CommandRouter
{
// nested types
private:
	typedef
		std::shared_ptr<CommandProcessor>
		CommandProcessorPtr;
	typedef
		std::map<std::string, CommandProcessorPtr>
		CommandProcessorMap;

// special member functions
public:
	explicit CommandRouter(TimeLog& p_time_log);
	CommandRouter(CommandRouter const& rhs) = delete;
	CommandRouter(CommandRouter&& rhs) = delete;
	CommandRouter& operator=(CommandRouter const& rhs) = delete;
	CommandRouter& operator=(CommandRouter&& rhs) = delete;
	~CommandRouter() = default;

// ordinary member functions
private:

	void populate_command_processor_map();

public:

	int process_command
	(	std::string const& p_command,
		std::vector<std::string> const& p_args
	) const;

private:

	int process_unrecognized_command(std::string const& p_command) const;

	std::ostream& ordinary_ostream() const;
	std::ostream& error_ostream() const;

	void create_command(CommandProcessorPtr const& p_cpp);

// member variables
private:
	TimeLog& m_time_log;
	CommandProcessorMap m_command_processor_map;

};  // class CommandRouter

}  // namespace swx

#endif  // GUARD_command_router_hpp_6901861572126794
