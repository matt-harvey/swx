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
#include "config.hpp"
#include "stream_flag_guard.hpp"
#include "time_log.hpp"
#include <memory>
#include <ostream>
#include <string>
#include <map>
#include <unordered_set>
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
    using CommandMap = std::map<std::string, std::shared_ptr<Command>>;

// special member functions
public:
    explicit CommandManager(TimeLog& p_time_log);
    CommandManager(CommandManager const& rhs) = delete;
    CommandManager(CommandManager&& rhs) = delete;
    CommandManager& operator=(CommandManager const& rhs) = delete;
    CommandManager& operator=(CommandManager&& rhs) = delete;
    ~CommandManager();

// ordinary and static member functions
private:
    void populate_command_map();

public:

    int process_command
    (   Config const& p_config,
        std::string const& p_command,
        std::vector<std::string> const& p_args
    ) const;

    /**
     * @returns a string providing help information for a particular
     * command.
     *
     * @throws std::runtime_error if p_command is not a registered
     * command word.
     */
    std::string help_information(std::string const& p_command) const;

    /**
     * @returns a string providing general help information.
     */
    std::string help_information() const;

    static std::string directions_to_get_help();
    static std::string directions_to_get_help(std::string const& p_command);

    static std::string error_message_for_unrecognized_command
    (   std::string const& p_command
    );

private:
    int process_unrecognized_command(std::string const& p_command) const;
    std::ostream& ordinary_ostream() const;
    std::ostream& error_ostream() const;

    template <typename CommandT, typename ... Args>
    void create_command(Args&& ... p_args);

    void register_command_word
    (   std::string const& p_word,
        std::shared_ptr<Command> const& p_cp
    );

// member variables
private:
    TimeLog& m_time_log;
    CommandMap m_command_map;

};  // class CommandManager


// member template implementations

template <typename CommandT, typename ... Args>
void
CommandManager::create_command(Args&& ... p_args)
{
    auto command = std::make_shared<CommandT>(std::forward<Args>(p_args) ...);
    register_command_word(command->command_word(), command);
    for (auto const& alias: command->aliases())
    {
        register_command_word(alias, command);
    }
    return;
}

}  // namespace swx

#endif  // GUARD_command_manager_hpp_6901861572126794
