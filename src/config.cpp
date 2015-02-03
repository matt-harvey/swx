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

#include "config.hpp"
#include "atomic_writer.hpp"
#include "info.hpp"
#include "file_utilities.hpp"
#include "string_utilities.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <unistd.h>

using std::endl;
using std::find;
using std::getenv;
using std::getline;
using std::ifstream;
using std::map;
using std::ofstream;
using std::ostringstream;
using std::pair;
using std::runtime_error;
using std::string;

namespace swx
{

namespace
{
	enum class LineType
	{
		blank,
		entry,
		error
	};

	char const k_commenting_char = '#';
	char const k_separator = '=';

	LineType parse_line(string const& p_line, pair<string, string>& p_out)
	{
		string const line = trim(p_line);
		if (line.empty() || line[0] == k_commenting_char)
		{
			return LineType::blank;
		}
		auto const it = find(line.begin(), line.end(), k_separator);
		if (it == line.end() || it == line.begin())
		{
			return LineType::error;
		}
		string first(line.begin(), it);
		assert (it != line.end());
		string second(it + 1, line.end());
		p_out.first = trim(first);
		p_out.second = trim(second);
		return LineType::entry;
	}

	string comment_out(string const& s)
	{
		if (s.empty())
		{
			return s;
		}
		ostringstream oss;
		enable_exceptions(oss);
		oss << k_commenting_char;
		string::size_type const sz = s.size();
		if (!((sz == 0) || (s == "\n")))
		{
			oss << ' ';
		}
		for (string::size_type i = 0; i != sz; ++i)
		{
			auto const c = s[i];
		 	oss << c;
			if ((c == '\n') && (i + 1 != sz))
			{
				oss << k_commenting_char;
				if (s[i + 1] != '\n')
				{
					oss << ' ';
				}
			}
		}
		return oss.str();
	}

}  // end anonymous namespace

Config::OptionData::OptionData
(	string const& p_value,
	string const& p_description
):
	value(p_value),
	description(p_description)
{
}

Config&
Config::instance()
{
	static Config ret;
	return ret;
}

unsigned int
Config::output_rounding_numerator()
{
	return instance().
		get_option_value<unsigned int>("output_rounding_numerator");
}

unsigned int
Config::output_rounding_denominator()
{
	return instance().
		get_option_value<unsigned int>("output_rounding_denominator");
}

unsigned int
Config::output_precision()
{
	return instance().get_option_value<unsigned int>("output_precision");
}

unsigned int
Config::output_width()
{
	return instance().get_option_value<unsigned int>("output_width");
}

string
Config::time_format()
{
	return instance().get_option_value<string>("format_string");
}

unsigned int
Config::formatted_buf_len()
{
	return instance().get_option_value<unsigned int>("formatted_buf_len");
}

string
Config::editor()
{
	return instance().get_option_value<string>("editor");
}

string
Config::path_to_log()
{
	return instance().get_option_value<string>("path_to_log");
}

string
Config::filepath()
{
	static string const ret = Info::home_dir() + "/.swxrc";  // non-portable
	return ret;
}

string
Config::summary()
{
	ostringstream oss;
	enable_exceptions(oss);
	for (auto const& entry: instance().m_map)
	{
		oss << entry.first << '=' <<  entry.second.value << '\n';
	}
	return oss.str();
}

Config::Config()
{
	set_defaults();
	if (!file_exists_at(filepath()))
	{
		initialize_config_file();
		return;
	}
	ifstream infile(filepath().c_str());
	enable_exceptions(infile);
	string line;
	size_t line_number = 1;
	while (infile.peek() != EOF)
	{
		getline(infile, line);
		pair<string, string> pair;
		auto const line_type =  parse_line(line, pair);
		if (line_type == LineType::error)
		{
			ostringstream oss;
			enable_exceptions(oss);
			oss << "Parsing error in config file at "
			    << filepath()
				<< ", line "
				<< line_number;
			throw runtime_error(oss.str());
		}
		else if (line_type == LineType::entry)
		{
			set_option_value(pair.first, pair.second);
		}
		++line_number;
	}
}

Config::~Config() = default;

void
Config::set_option_value(string const& p_key, string const& p_value)
{
	auto const it = m_map.find(p_key);
	if (it == m_map.end())
	{
		string const errmsg = "Unrecognized configuration key: " + p_key;
		throw runtime_error(errmsg);
	}
	it->second.value = p_value;
	return;
}

void
Config::unchecked_set_option
(	string const& p_key,
	OptionData const& p_option_data
)
{
	m_map[p_key] = p_option_data;
	return;
}

string
Config::get_raw_option_value(std::string const& p_key)
{
	auto const it = m_map.find(p_key);
	if (it == m_map.end())
	{
		string const errmsg = "Could not find configuration key: " + p_key;
		throw runtime_error(errmsg);
	}
	assert (it != m_map.end());
	return it->second.value;
}

void
Config::set_defaults()
{
	string const rounding_explanation
	(	"output_rounding_numerator and output_rounding_denominator together\n"
		"determine rounding behaviour when printing a duration figure.\n"
		"For example, if output_rounding_numerator is 1 and\n"
		"output_rounding_denominator is 4, and the output duration is\n"
		"measured in hours, then the output will be rounded to the nearest\n"
		"quarter of an hour."
	);
	unchecked_set_option
	(	"output_rounding_numerator",
		OptionData("1", rounding_explanation)
	);
	unchecked_set_option
	(	"output_rounding_denominator",
		OptionData("4", rounding_explanation)
	);
	unchecked_set_option
	(	"output_precision",
		OptionData
		(	"2",
			"Determines the number of decimal places of precision for\n"
			"durations when output in decimal format."
		)
	);
	unchecked_set_option
	(	"output_width",
		OptionData("6", "Field width when printing durations.")
	);
	unchecked_set_option
	(	"format_string",
		OptionData
		(	"%Y-%m-%dT%H:%M",
			"Determines the format used when parsing and printing timestamps.\n"
			"See the documentation for the C function strftime for details.\n"
			"If you change this, you should also review the formatted_buf_len\n"
			"option to ensure it will be adequate.\n"
			"Note, changing this option will NOT cause the timestamps already\n"
			"entered in the data file to be retroactively reformatted. This\n"
			"will cause parsing errors unless you manually reformat the old\n"
			"entries to the new format. It is therefore best to decide on a\n"
			"format when you first run the program, and then stick with it."
		)
	);
	unchecked_set_option
	(	"formatted_buf_len",
		OptionData
		(	"50",
			"Should be set to a value that is at least one greater than the\n"
			"length of the longest string expected to be printed as a result\n"
			"of formatting a time point using format_string."
		)
	);
	char const* env_editor = getenv("EDITOR");  // non-portable
	unchecked_set_option
	(	"editor",
		OptionData
		(	(env_editor? env_editor: "vi"),  // non-portable
			"Editor to be invoked by file editing commands. Must be callable\n"
			"by name from the command line, with filepath as argument."
		)
	);

	// non-portable
	auto user_name = getlogin();
	string const stem = (user_name? user_name: "time_log");
	unchecked_set_option
	(	"path_to_log",
		OptionData
		(	Info::home_dir() + "/" + stem + ".swx",
			"Path to file in which time log is recorded."
		)
	);

	return;
}

void
Config::initialize_config_file()
{
	AtomicWriter writer(filepath());
	ostringstream oss;
	enable_exceptions(oss);
	oss << "Configuration options for " << Info::application_name()
	    << " can be set in this file.\n"
		<< "\n"
		<< "SYNTAX:\n"
		<< "\tkey" << k_separator << "value\n"
		<< "\tBlank lines are permitted.\n"
		<< "\tDo NOT place quotes around value.\n"
		<< "\tvalue may contain whitespace.\n"
		<< "\tComments must occupy a line to themselves beginning with '"
		<< k_commenting_char << "'.";
	writer.append_line(comment_out(oss.str()));
	writer.append_line(string(80, k_commenting_char));
	for (auto const& entry: m_map)
	{
		OptionData const data = entry.second;
		writer.append_line();
		writer.append_line(comment_out(data.description));
		writer.append_line(comment_out("Default: " + data.value));
		writer.append_line
		(	comment_out
			(	"To customize, uncomment and edit the following line "
					"accordingly."
			)
		);
		writer.append_line(comment_out(entry.first + k_separator + data.value));
	}
	writer.commit();
	return;
}

}  // namespace swx
