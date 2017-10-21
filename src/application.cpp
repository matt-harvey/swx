/*
 * Copyright 2014, 2017 Matthew Harvey
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

#include "application.hpp"
#include "command.hpp"
#include "config.hpp"
#include "config_command.hpp"
#include "current_command.hpp"
#include "day_command.hpp"
#include "edit_command.hpp"
#include "exit_code.hpp"
#include "help_command.hpp"
#include "info.hpp"
#include "placeholder.hpp"
#include "print_command.hpp"
#include "rename_command.hpp"
#include "resume_command.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "switch_command.hpp"
#include "time_log.hpp"
#include "version_command.hpp"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::left;
using std::map;
using std::move;
using std::ostream;
using std::ostringstream;
using std::runtime_error;
using std::set;
using std::setw;
using std::shared_ptr;
using std::string;
using std::unordered_set;
using std::vector;

namespace swx
{

namespace
{
    string const k_help_command_string("help");

}  // end anonymous namespace

Application::Application(string const& p_config_path):
    m_config(p_config_path),
    m_time_log(m_config.path_to_log(), m_config.time_format(), m_config.formatted_buf_len())
{
    using V = vector<string>;

    CommandGroup rec("Recording commands");
    create_command<SwitchCommand>(rec, "switch", V{"s"}, m_time_log);
    create_command<ResumeCommand>(rec, "resume", V{}, m_time_log);
    m_command_groups.push_back(move(rec));

    CommandGroup rep("Reporting commands");
    create_command<PrintCommand>(rep, "print", V{"p"}, m_time_log);
    create_command<DayCommand>(rep, "day", V{"d"}, m_time_log);
    m_command_groups.push_back(move(rep));

    CommandGroup edit("Editing commands");
    create_command<RenameCommand>(edit, "rename", V{}, m_time_log);
    create_command<EditCommand>(edit, "edit", V{"e"});
    m_command_groups.push_back(move(edit));

    CommandGroup misc("Miscellaneous commands");
    create_command<CurrentCommand>(misc, "current", V{"c"}, m_time_log);
    create_command<ConfigCommand>(misc, "config", V{});
    create_command<HelpCommand>(misc, k_help_command_string, V{"--help", "-h"}, *this);
    create_command<VersionCommand>(misc, "version", V{"--version"});
    m_command_groups.push_back(move(misc));

#   ifndef NDEBUG
        for (auto const& entry: m_command_map)
        {
            assert (entry.second);
        }
#   endif
}

Application::~Application() = default;

ExitCode
Application::process_command(string const& p_command, vector<string> const& p_args) const
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
        auto const ret = command_ptr->process
        (   m_config,
            p_args,
            ordinary_ostream(),
            error_ostream()
        );
        if (ret != EXIT_SUCCESS)
        {
            error_ostream() << directions_to_get_help(p_command) << endl;
        }
        return ret;
    }
}

string
Application::help_information(string const& p_command) const
{
    auto const it = m_command_map.find(p_command);
    if (it == m_command_map.end())
    {
        throw runtime_error
        (   error_message_for_unrecognized_command(p_command)
        );
    }
    return it->second->usage_descriptor();
}

string
Application::help_information() const
{
    ostringstream oss;
    enable_exceptions(oss);
    oss << "Usage: "
        << Info::application_name()
        << " <COMMAND> [OPTIONS...] [ARGUMENTS...] [OPTIONS...]\n";
    string::size_type width = 0;
    for (auto const& group: m_command_groups)
    {
        for (auto const& command_ptr: group.commands)
        {
            auto const& command = *command_ptr;
            string::size_type current_width = command.command_word().size();
            for (auto const& alias: command.aliases())
            {
                current_width += 2;
                current_width += alias.size();
            }
            current_width += 4;
            width = ((current_width > width)? current_width: width);
        }
    }
    for (auto const& group: m_command_groups)
    {
        oss << '\n' << group.label << ":\n\n";
        for (auto const& command_ptr: group.commands)
        {
            auto const& command = *command_ptr;
            StreamFlagGuard guard(oss);
            ostringstream oss2;
            enable_exceptions(oss2);
            oss2 << command.command_word();
            for (auto const& alias: command.aliases())
            {
                oss2 << ", " << alias;
            }
            oss << "  " << setw(width) << left << oss2.str();
            guard.reset();
            oss << ": " << wrap(command.usage_summary(), width + 4) << endl;
        }
    }
    oss << "\nFor more information on a particular command, enter '"
        << Info::application_name() << ' '
        << k_help_command_string << " <COMMAND>'.\n";
    return oss.str();
}

string
Application::directions_to_get_help()
{
    ostringstream oss;
    enable_exceptions(oss);
    oss << "For usage information, enter '"
        << Info::application_name()
        << ' '
        << k_help_command_string
        << "'.";
    return oss.str();
}

string
Application::directions_to_get_help(string const& p_command)
{
    ostringstream oss;
    enable_exceptions(oss);
    oss << "For usage information, enter '"
        << Info::application_name()
        << ' '
        << k_help_command_string
        << ' '
        << p_command
        << "'.";
    return oss.str();
}

string
Application::error_message_for_unrecognized_command
(   string const& p_command
)
{
    return string("Unrecognized command: ") + p_command;
}

ExitCode
Application::process_unrecognized_command(string const& p_command) const
{
    error_ostream() << error_message_for_unrecognized_command(p_command)
                    << endl
                    << directions_to_get_help()
                    << endl;
    return EXIT_FAILURE;
}

ostream&
Application::ordinary_ostream() const
{
    return cout;
}

ostream&
Application::error_ostream() const
{
    return cerr;
}

void
Application::register_command_word
(   string const& p_word,
    shared_ptr<Command> const& p_cp
)
{
    if (m_command_map.find(p_word) != m_command_map.end())
    {
        ostringstream oss;
        enable_exceptions(oss);
        oss << "Command word \""
            << p_word
            << "\" has already been registered.";
        throw runtime_error(oss.str());
    }
    m_command_map[p_word] = p_cp;
}

}  // namespace swx
