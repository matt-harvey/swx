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

#include "command.hpp"
#include "help_line.hpp"
#include "info.hpp"
#include "placeholder.hpp"
#include "stream_flag_guard.hpp"
#include "stream_utilities.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
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
	
	char const k_flag_prefix = '-';

	pair<char, string> const k_double_dash_option =
		make_pair
		(	'-',
			"Treat any dash-prefixed arguments after this flag as "
				"ordinary arguments, rather than flags"
		);

}  // end anonymous namespace

Command::ParsedArguments::ParsedArguments
(	vector<string> const& p_raw_args,
	bool p_recognize_double_dash,
	bool p_accept_ordinary_args
)
{
	if (p_accept_ordinary_args && !p_recognize_double_dash)
	{
		m_ordinary_args = p_raw_args;
		return;
	}
	ostringstream oss;
	enable_exceptions(oss);
	oss << k_flag_prefix << k_double_dash_option.first;
	string const double_dash = oss.str();
	for (auto it = p_raw_args.begin(); it != p_raw_args.end(); ++it)
	{
		auto const& arg = *it;
		if (p_recognize_double_dash && (arg == double_dash))
		{
			copy(it + 1, p_raw_args.end(), back_inserter(m_ordinary_args));
			break;
		}
		else if (!arg.empty() && (arg[0] == k_flag_prefix))
		{
			m_single_character_flags.insert(arg.begin() + 1, arg.end());
		}
		else
		{
			m_ordinary_args.push_back(*it);	
		}
	}
}

vector<string>
Command::ParsedArguments::ordinary_args() const
{
	return m_ordinary_args;
}

string
Command::ParsedArguments::single_character_flags() const
{
	string const ret
	(	m_single_character_flags.begin(),
		m_single_character_flags.end()
	);
#	ifndef NDEBUG
		for (auto it = ret.begin(); it != ret.end(); ++it)
		{
			auto const next = it + 1;
			if (next != ret.end())
			{
				assert (*it < *next);
			}
		}
#	endif
	return ret;
}

bool
Command::ParsedArguments::has_flag(char c) const
{
	return m_single_character_flags.find(c) != m_single_character_flags.end();
}

Command::Command
(	string const& p_command_word,
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
Command::add_boolean_option(char p_flag, string const& p_description)
{
	if (has_boolean_option(p_flag))
	{
		ostringstream oss;
		enable_exceptions(oss);
		oss << "Flag already enabled for this Command: " << p_flag;
		throw runtime_error(oss.str());
	}
	m_boolean_options.insert(make_pair(p_flag, p_description));
	if (m_accept_ordinary_args)
	{
		try
		{
			m_boolean_options.insert(k_double_dash_option);
		}
		catch (...)
		{
			m_boolean_options.erase(p_flag);
			throw;
		}
	}
	return;
}

string
Command::category() const
{
	return do_get_category();
}

bool
Command::has_boolean_option(char p_flag) const
{
	return m_boolean_options.find(p_flag) != m_boolean_options.end();
}

int
Command::process
(	Arguments const& p_args,
	ostream& p_ordinary_ostream,
	ostream& p_error_ostream
)
{
	ParsedArguments const parsed_args
	(	p_args,
		has_boolean_option(k_double_dash_option.first),
		m_accept_ordinary_args
	);
	if (!m_accept_ordinary_args && !parsed_args.ordinary_args().empty())
	{
		p_error_ostream << "Too many arguments.\nAborted" << endl;
		return 1;
	}
	auto const flags = parsed_args.single_character_flags();
	assert (!m_boolean_options.empty() || flags.empty());
	bool has_unrecognized_option = false;
	for (auto c: flags)
	{
		if (!has_boolean_option(c))
		{
			p_error_ostream << "Unrecognized option: " << c << endl;
			has_unrecognized_option = true;
		}
	}
	if (has_unrecognized_option)
	{
		p_error_ostream << "Aborted" << endl;
		return 1;
	}
	auto const error_messages = do_process(parsed_args, p_ordinary_ostream);
	for (auto const& message: error_messages)
	{
		p_error_ostream << message << endl;
	}
	if (error_messages.empty())
	{
		assert (!has_unrecognized_option);
		return 0;
	}
	assert (error_messages.size() > 0);
	return 1;
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
	typedef string::size_type ColWidth;
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
	if (!m_boolean_options.empty())
	{
		oss << "\n\nOptions:\n";
		for (auto const& option: m_boolean_options)
		{
			StreamFlagGuard guard(oss);
			string const flag{'-', option.first};
			string const& description = option.second;
			oss << "\n  " << setw(left_col_width) << left
			    << flag;
			guard.reset();
			oss << description;
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

string
Command::do_get_category() const
{
	return "Miscellaneous";
}

bool
Command::does_support_placeholders() const
{
	return false;
}

}  // namespace swx
