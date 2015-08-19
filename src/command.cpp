/*
 * Copyright 2014, 2015 Matthew Harvey
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

#include "command.hpp"
#include "config.hpp"
#include "exit_code.hpp"
#include "help_line.hpp"
#include "info.hpp"
#include "placeholder.hpp"
#include "stream_flag_guard.hpp"
#include "stream_utilities.hpp"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::endl;
using std::left;
using std::make_pair;
using std::ostream;
using std::ostringstream;
using std::make_pair;
using std::numeric_limits;
using std::pair;
using std::runtime_error;
using std::set;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

namespace
{
    
    char const k_option_prefix = '-';
    char const k_double_dash_option_char = '-';

}  // end anonymous namespace

struct Command::Option
{
    Option
    (   char p_character,
        HelpLine const& p_help_line,
        bool* p_presence_target = nullptr,
        std::string* p_arg_target = nullptr
    ):
        presence_target(p_presence_target),
        arg_target(p_arg_target),
        character(p_character),
        help_line(p_help_line)
    {
    }
    char character;
    bool* presence_target;
    std::string* arg_target;
    HelpLine help_line;
};

Command::Command
(   string const& p_command_word,
    vector<string> const& p_aliases,
    string const& p_usage_summary,
    vector<HelpLine> const& p_help_lines,
    bool p_accept_ordinary_args
):
    m_accept_ordinary_args(p_accept_ordinary_args),
    m_command_word(p_command_word),
    m_usage_summary(p_usage_summary),
    m_aliases(p_aliases),
    m_help_lines(p_help_lines)
{
}

Command::~Command() = default;

void
Command::add_option
(   char p_character,
    HelpLine const& p_help_line,
    bool* p_presence_target,
    std::string* p_arg_target
)
{
    if (m_options.count(p_character))
    {
        ostringstream oss;
        enable_exceptions(oss);
        oss << "Option already enabled for this Command: " << p_character;
        throw runtime_error(oss.str());
    }
    m_options.emplace
    (   p_character,
        Option(p_character, p_help_line, p_presence_target, p_arg_target)
    );
    if (m_accept_ordinary_args)
    {
        try
        {
            Option const double_dash_option
            (   k_double_dash_option_char,
                "Treat any dash-prefixed arguments after this option as "
                    "ordinary arguments, rather than options"
            );
            m_options.emplace(k_double_dash_option_char, double_dash_option);
        }
        catch (...)
        {
            m_options.erase(p_character);
            throw;
        }
    }
    return;
}

ExitCode
Command::process
(   Config const& p_config,
    vector<string> const& p_args,
    ostream& p_ordinary_ostream,
    ostream& p_error_ostream
)
{
    vector<string> ordinary_args;
    auto const recognize_double_dash = m_options.count(k_double_dash_option_char);
    if (m_accept_ordinary_args && !recognize_double_dash)
    {
        ordinary_args = p_args;
    }
    else
    {
        ostringstream oss;
        enable_exceptions(oss);
        oss << k_option_prefix << k_double_dash_option_char;
        string const double_dash = oss.str();
        auto const args_end = p_args.end();

        // examine each argument in turn
        for (auto it = p_args.begin(); it != args_end; ++it)
        {
            auto const& arg = *it;

            if (recognize_double_dash && (arg == double_dash))
            {
                // double-dash ends the options, so treat remaining arguments as
                // ordinary arguments, not options
                copy(it + 1, p_args.end(), back_inserter(ordinary_args));
                break;
            }
            else if (!arg.empty() && (arg[0] == k_option_prefix))
            {
                // arg is an option (e.g. "-a"), or a cluster of options (e.g. "-abc")
                auto const arg_end = arg.end();

                // look at each character in arg in turn
                for (auto chit = arg.begin() + 1; chit != arg_end; ++chit)
                {
                    auto const c = *chit;
                    auto const opt_it = m_options.find(c);
                    if (opt_it == m_options.end())
                    {
                        p_error_ostream << "Unrecognized option: " << c << "\nAborted."
                                        << endl;
                        return EXIT_FAILURE;
                    }
                    assert (opt_it != m_options.end());
                    auto const& opt = opt_it->second;
                    auto* const presence_target = opt.presence_target;
                    if (presence_target != nullptr) *presence_target = true;
                    auto* const arg_target = opt.arg_target;
                    if (arg_target == nullptr)
                    {
                        continue;  // option does not take an argument
                    }
                    // option takes an argument
                    assert (chit + 1 <= arg_end);
                    assert (it + 1 <= args_end);
                    auto const joined_arg_present = (chit + 1 != arg_end);   // e.g. "-a9"
                    auto const separated_arg_present = (it + 1 != args_end); // e.g. "-a 9"
                    if (!joined_arg_present && !separated_arg_present)
                    {
                        p_error_ostream << "Option " << c << " requires argument.\nAborted."
                                        << endl;
                        return EXIT_FAILURE;
                    }
                    if (joined_arg_present) *arg_target = string(chit + 1, arg_end);
                    else *arg_target = *(++it);
                    break;  // we've finished with this option-cluster
                }
            }
            else
            {
                if (!m_accept_ordinary_args)
                {
                    p_error_ostream << "Too many arguments.\nAborted" << endl;
                    return EXIT_FAILURE;
                }
                ordinary_args.push_back(*it);
            }
        }
    }
    auto const error_messages = do_process(p_config, ordinary_args, p_ordinary_ostream);
    for (auto const& message: error_messages) p_error_ostream << message << endl;
    return error_messages.empty() ? EXIT_SUCCESS : EXIT_FAILURE;
}

string
Command::usage_summary() const
{
    return m_usage_summary;
}

string
Command::usage_descriptor() const
{
    // TODO LOW PRIORITY This should handle wrapping of the right-hand cell
    // to a reasonable width if necessary.
    using ColWidth = string::size_type;
    ColWidth command_word_length = m_command_word.length();
    ColWidth left_col_width = command_word_length;
    auto const app_name = Info::application_name();
    for (auto const& line: m_help_lines)
    {
        ColWidth const left_cell_width =
            line.args_descriptor().length() + command_word_length;
        if (left_cell_width > left_col_width) left_col_width = left_cell_width;
    }
    left_col_width += app_name.length() + 1 + m_command_word.length() + 2;
    ostringstream oss;
    enable_exceptions(oss);
    oss << "Usage:\n";
    for (auto const& line: m_help_lines)
    {
        StreamFlagGuard guard(oss);
        oss << "\n  " << setw(left_col_width) << left
            << (app_name + ' ' + m_command_word + ' ' + line.args_descriptor())
            << "  ";
        guard.reset();
        oss << line.usage_descriptor();
    }
    if (!m_aliases.empty())
    {
        oss << "\n\nAliased as: ";
        auto it = m_aliases.begin();
        oss << *it;
        for (++it; it != m_aliases.end(); ++it) oss << ", " << *it;
    }
    left_col_width = 6;
    if (!m_options.empty())
    {
        oss << "\n\nOptions:\n";
        for (auto const& option: m_options)
        {
            auto const width = option.second.help_line.args_descriptor().size() + 6;
            if (width > left_col_width) left_col_width = width;
        }
        for (auto const& option: m_options)
        {
            StreamFlagGuard guard(oss);
            string const option_str{'-', option.first};
            auto const& help_line = option.second.help_line;
            oss << "\n  " << setw(left_col_width) << left
                << (option_str + " " + help_line.args_descriptor());
            guard.reset();
            oss << help_line.usage_descriptor();
        }
    }
    if (does_support_placeholders())
    {
        auto const placeholder_lines = placeholder_help(left_col_width);
        if (!placeholder_lines.empty())
        {
            oss << "\n\nPlaceholders:\n";
            for (auto const& line: placeholder_lines)
            {
                oss << "\n  " << line;
            }
        }
    }
    return oss.str();
}

string const&
Command::command_word() const
{
    return m_command_word;
}

vector<string> const&
Command::aliases() const
{
    return m_aliases;
}

bool
Command::does_support_placeholders() const
{
    return false;
}

}  // namespace swx
