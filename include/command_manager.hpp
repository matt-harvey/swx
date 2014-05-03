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

#ifndef GUARD_command_manager_hpp_6901861572126794
#define GUARD_command_manager_hpp_6901861572126794

#include "command.hpp"
#include "time_log.hpp"
#include <memory>
#include <ostream>
#include <string>
#include <map>
#include <utility>
#include <vector>

namespace swx
{

/**
 * Manages the various commands provided by the application.
 *
 * Processes command line arguments, by passing them to the appropriate
 * Command instance.
 */
class CommandManager
{
// nested types
private:
	typedef
		std::shared_ptr<Command>
		CommandPtr;
	typedef
		std::map<std::string, CommandPtr>
		CommandMap;

// special member functions
public:
	explicit CommandManager(TimeLog& p_time_log);
	CommandManager(CommandManager const& rhs) = delete;
	CommandManager(CommandManager&& rhs) = delete;
	CommandManager& operator=(CommandManager const& rhs) = delete;
	CommandManager& operator=(CommandManager&& rhs) = delete;
	~CommandManager() = default;

// ordinary and static member functions
private:
	void populate_command_map();

public:

	int process_command
	(	std::string const& p_command,
		std::vector<std::string> const& p_args
	) const;

	/**
	 * @throws std::runtime_error if p_command is not a registered
	 * command word.
	 */
	std::string help_information(std::string const& p_command) const;

	/**
	 * In each element \e e of the returned vector, \e e.first is
	 * the main command word, \e e.second is a vector of aliases
	 * for that command word, and \e e.third is the usage summary for
	 * that command.
	 *
	 * @todo LOW PRIORITY Give this a nicer interface.
	 */
	std::vector<std::tuple<std::string, std::vector<std::string>, std::string>>
		available_commands() const;

	static std::string directions_to_get_help();
	static std::string directions_to_get_help(std::string const& p_command);

	static std::string error_message_for_unrecognized_command
	(	std::string const& p_command
	);

private:
	int process_unrecognized_command(std::string const& p_command) const;
	std::ostream& ordinary_ostream() const;
	std::ostream& error_ostream() const;
	void create_command(CommandPtr const& p_cp);
	void register_command_word
	(	std::string const& p_word,
		CommandPtr const& p_cp
	);

// member variables
private:
	TimeLog& m_time_log;
	CommandMap m_command_map;

};  // class CommandManager

}  // namespace swx

#endif  // GUARD_command_manager_hpp_6901861572126794
