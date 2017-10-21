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

#ifndef GUARD_application_hpp_6901861572126794
#define GUARD_application_hpp_6901861572126794

#include "command.hpp"
#include "config.hpp"
#include "exit_code.hpp"
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
class Application
{
// nested types
private:
    using CommandMap = std::map<std::string, std::shared_ptr<Command>>;
    using Commands = std::vector<std::shared_ptr<Command>>;

    struct CommandGroup
    {
        explicit CommandGroup(std::string const& p_label): label(p_label)
        {
        }
        std::string label;
        Commands commands;
    };

// special member functions
public:
    explicit Application(std::string const& p_config_path);
    Application(Application const& rhs) = delete;
    Application(Application&& rhs) = delete;
    Application& operator=(Application const& rhs) = delete;
    Application& operator=(Application&& rhs) = delete;
    ~Application();

// ordinary and static member functions
private:
    void populate_command_map();

public:

    ExitCode process_command
    (   std::string const& p_command,
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
    ExitCode process_unrecognized_command(std::string const& p_command) const;
    std::ostream& ordinary_ostream() const;
    std::ostream& error_ostream() const;

    template <typename CommandT, typename ... Args>
    void create_command(CommandGroup& p_command_group, Args&& ... p_args);

    void register_command_word
    (   std::string const& p_word,
        std::shared_ptr<Command> const& p_cp
    );

// member variables
private:
    Config m_config;
    TimeLog m_time_log;
    CommandMap m_command_map;
    std::vector<CommandGroup> m_command_groups;

};  // class Application


// member template implementations

template <typename CommandT, typename ... Args>
void
Application::create_command(CommandGroup& p_command_group, Args&& ... p_args)
{
    auto command = std::make_shared<CommandT>(std::forward<Args>(p_args) ...);
    register_command_word(command->command_word(), command);
    for (auto const& alias: command->aliases())
    {
        register_command_word(alias, command);
    }
    p_command_group.commands.push_back(command);
}

}  // namespace swx

#endif  // GUARD_application_hpp_6901861572126794
