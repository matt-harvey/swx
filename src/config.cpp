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
#include <cassert>
#include <map>
#include <string>
#include <utility>

using std::map;
using std::pair;
using std::string;

namespace swx
{

namespace
{
	pair<string, string> parse_config_line(string const& p_line)
	{
		pair<string, string> ret;
		string temp;
		for (auto it = p_line.begin(); it != p_line.end(); ++it)
		{
			if (*it == '=')
			{
				ret.first = temp;	
				// TODO IN THE MIDDLE OF WORKING ON THIS
			}
		}
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
	save();
}

void
Config::load()
{
	if (!m_is_loaded)
	{
		do_load_from(m_filepath);
		m_is_loaded = true;
	}
	return;
}

void
Config::save()
{
	if (m_is_loaded)
	{
		do_save_to(m_filepath);
	}
	return;
}

void
Config::do_load_from(string const& p_filepath)
{
	// TODO Implement.
}

void
Config::do_save_to(string const& p_filepath)
{
	// TODO Implement.
}

}  // namespace swx
