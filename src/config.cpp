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
#include "info.hpp"
#include "string_utilities.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using std::find;
using std::getline;
using std::ifstream;
using std::map;
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

	LineType parse_line(string const& p_line, pair<string, string>& p_out)
	{
		string const line = trim(p_line);
		if (line.empty() || line[0] == '#')
		{
			return LineType::blank;
		}
		auto const it = find(line.begin(), line.end(), '=');
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

}  // end anonymous namespace

Config&
Config::instance()
{
	static Config ret;	
	return ret;
}

unsigned int
Config::output_rounding_numerator()
{
	return instance().get_option<unsigned int>("output_rounding_numerator");
}

unsigned int
Config::output_rounding_denominator()
{
	return instance().get_option<unsigned int>("output_rounding_denominator");
}

unsigned int
Config::output_precision()
{
	return instance().get_option<unsigned int>("output_precision");
}

unsigned int
Config::output_width()
{
	return instance().get_option<unsigned int>("output_width");
}

string
Config::format_string()
{
	return instance().get_option<string>("format_string");
}

unsigned int
Config::formatted_buf_len()
{
	return instance().get_option<unsigned int>("formatted_buf_len");
}

Config::Config():
	m_is_loaded(false),
	// TODO HIGH PRIORITY Make filepath portable
	m_filepath(Info::data_dir() + "/config")
{
	assert (m_map.empty());
}

Config::~Config()
{
}

void
Config::set_option(string const& p_key, string const& p_value)
{
	m_map[p_key] = p_value;
	return;
}

void
Config::load()
{
	if (!m_is_loaded)
	{
		set_defaults();
		do_load_from(m_filepath);
		m_is_loaded = true;
	}
	return;
}

void
Config::do_load_from(string const& p_filepath)
{
	ifstream infile(p_filepath.c_str());
	string line;
	size_t line_number = 1;
	while (getline(infile, line))
	{
		pair<string, string> pair;
		auto const line_type =  parse_line(line, pair);
		if (line_type == LineType::error)
		{
			ostringstream oss;
			oss << "Parsing error in config file at "
			    << p_filepath
				<< ", line "
				<< line_number;
			throw runtime_error(oss.str());
		}
		else if (line_type == LineType::entry)
		{
			set_option(pair.first, pair.second);
		}
		++line_number;
	}
	return;
}

void
Config::set_defaults()
{
	set_option("output_rounding_numerator", "1");
	set_option("output_rounding_denominator", "4");
	set_option("output_precision", "2");
	set_option("output_width", "6");
	set_option("format_string", "%Y-%m-%dT%H:%M:%S");
	set_option("formatted_buf_len", "80");
	return;
}

}  // namespace swx
