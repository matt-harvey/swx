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
#include "string_utilities.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
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
using std::find;
using std::function;
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
    
    auto const k_option_prefix = '-';
    auto const k_double_dash_option_alias = "-";

}  // end anonymous namespace

struct Command::Option
{
    Option
    (   vector<string> const& p_aliases,
        HelpLine const& p_help_line,
        function<void()> const& p_callback,
        string* p_arg_target = nullptr
    );
    HelpLine help_line;
    function<void()> callback;
    string* arg_target = nullptr;
    vector<string> aliases;
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
Command::add_option(Option const& p_option)
{
    if (m_accept_ordinary_args && m_options.empty())
    {
        assert (m_options_map.find("-") == m_options_map.end());
        auto const double_dash_option = Option
        (   vector<string>{k_double_dash_option_alias},
            "Treat any dash-prefixed arguments after this option as "
                "ordinary arguments, rather than options",
            []() { ; /* do nothing */ },
            nullptr
        );
        m_options_map.emplace(k_double_dash_option_alias, m_options.size());
        m_options.push_back(double_dash_option);
    }
    for (auto const& alias: p_option.aliases)
    {
        if (m_options_map.find(alias) != m_options_map.end())
        {
            ostringstream oss;
            enable_exceptions(oss);
            oss << "Option already enabled for this Command: " << alias;
            throw runtime_error(oss.str());
        }
        m_options_map.emplace(alias, m_options.size());
    }
    m_options.push_back(p_option);
}

void
Command::add_option
(   vector<string> const& p_aliases,
    HelpLine const& p_help_line,
    function<void()> const& p_callback,
    string* p_arg_target
)
{
    add_option(Option(p_aliases, p_help_line, p_callback, p_arg_target));
}

ExitCode
Command::process
(   Config const& p_config,
    vector<string> const& p_args,
    ostream& p_ordinary_ostream,
    ostream& p_error_ostream
)
{
    // TODO MEDIUM PRIORITY This is a mess. Break it down into smaller functions
    // and tidy it up.
    
    vector<string> ordinary_args;
    auto const recognize_double_dash =
        (m_options_map.find(k_double_dash_option_alias) != m_options_map.end());
    if (m_accept_ordinary_args && !recognize_double_dash)
    {
        ordinary_args = p_args;
    }
    else
    {
        ostringstream oss;
        enable_exceptions(oss);
        oss << k_option_prefix << k_double_dash_option_alias;
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
                if (arg.size() >= 2 && arg[1] == k_option_prefix)
                {
                    // arg is a long option
                    assert (arg.end() >= arg.begin() + 2);
                    assert (string(arg.begin(), arg.begin() + 2) == "--");
                    auto const arg_body = string(arg.begin() + 2, arg.end());
                    auto const eq_it = find(arg_body.begin(), arg_body.end(), '=');
                    string option_alias;
                    string option_argument;
                    bool option_argument_found = false;
                    if (eq_it == arg_body.end())
                    {
                        option_alias = arg_body;
                    }
                    else
                    {
                        // arg takes the form "--option=value"
                        option_alias = string(arg_body.begin(), eq_it);
                        option_argument = string(eq_it + 1, arg_body.end());
                        option_argument_found = true;
                    }
                    auto const opt_it = m_options_map.find(option_alias);
                    if (opt_it == m_options_map.end())
                    {
                        p_error_ostream << "Unrecognized option: " << option_alias
                                        << "\nAborted." << endl;
                        return EXIT_FAILURE;
                    }
                    if (option_alias.size() == 1)
                    {
                        p_error_ostream << "Short option \"" << option_alias
                                        << "\" must be passed with a single dash ('-').\n"
                                        << "Aborted." << endl;
                        return EXIT_FAILURE;
                    }
                    auto const& opt = m_options[opt_it->second];
                    if (opt.callback != nullptr) opt.callback();
                    if (opt.arg_target == nullptr)
                    {
                        if (option_argument_found)
                        {
                            p_error_ostream << "Option \"" << option_alias
                                            << "\" does not take an argument.\n"\
                                            << "Aborted." << endl;
                            return EXIT_FAILURE;
                        }
                        continue;  // option does not take an argument
                    }
                    assert (opt.arg_target != nullptr);
                    if (option_argument_found)
                    {
                        *opt.arg_target = option_argument;
                    }
                    else
                    {
                        ++it;
                        if (it == args_end)
                        {
                            p_error_ostream << "Option \"" << option_alias
                                            << "\" requires argument.\n"
                                            << "Aborted." << endl;
                            return EXIT_FAILURE;
                        }
                        else
                        {
                            *opt.arg_target = *it;
                        }
                    }
                }
                else
                {
                    // arg is a short option (e.g. "-a"), or a cluster of short options
                    // (e.g. "-abc")
                    auto const arg_end = arg.end();

                    // look at each character in arg in turn
                    for (auto chit = arg.begin() + 1; chit != arg_end; ++chit)
                    {
                        auto const c = *chit;
                        auto const opt_it = m_options_map.find(string(1, c));
                        if (opt_it == m_options_map.end())
                        {
                            p_error_ostream << "Unrecognized option: " << c << "\nAborted."
                                            << endl;
                            return EXIT_FAILURE;
                        }
                        assert (opt_it != m_options_map.end());
                        auto const& opt = m_options[opt_it->second];
                        if (opt.callback != nullptr) opt.callback();
                        if (opt.arg_target == nullptr)
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
                            p_error_ostream << "Option \"" << c << "\" requires argument.\n"
                                            << "Aborted." << endl;
                            return EXIT_FAILURE;
                        }
                        if (joined_arg_present) *opt.arg_target = string(chit + 1, arg_end);
                        else *opt.arg_target = *(++it);
                        break;  // we've finished with this option-cluster
                    }
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
    left_col_width += 2 + app_name.length();
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
        oss << ": " << wrap(line.usage_descriptor(), left_col_width + 6);
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
            auto width = option.help_line.args_descriptor().size() + 1;
            for (auto const& alias: option.aliases)
            {
                auto const alias_length = alias.size();
                width += 4;
                if (alias_length > 1) width += alias_length;
            }
            if (width > left_col_width) left_col_width = width;
        }
        for (auto const& option: m_options)
        {
            StreamFlagGuard guard(oss);
            auto const& help_line = option.help_line;
            ostringstream oss2;
            auto const& option_aliases = option.aliases;
            for (vector<string>::size_type i = 0; i != option_aliases.size(); ++i)
            {
                oss2 << (option_aliases[i].size() == 1 ? "-": "--") << option_aliases[i];
                if (i + 1 != option_aliases.size()) oss2 << ',';
                oss2 << ' ';
            }
            oss2 << help_line.args_descriptor();
            oss << "\n  " << setw(left_col_width) << left << oss2.str();
            guard.reset();
            oss << ": " << wrap(help_line.usage_descriptor(), left_col_width + 4);
        }
    }
    if (does_support_placeholders())
    {
        oss << "\n\nPlaceholders:\n";
        write_placeholder_help(oss, left_col_width);
    }
    oss << endl;
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

Command::Option::Option
(   vector<string> const& p_aliases,
    HelpLine const& p_help_line,
    function<void()> const& p_callback,
    string* p_arg_target
):
    help_line(p_help_line),
    callback(p_callback),
    arg_target(p_arg_target),
    aliases(p_aliases)
{
}

}  // namespace swx
