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

#ifndef GUARD_config_hpp_32019735189163934
#define GUARD_config_hpp_32019735189163934

#include <cassert>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

namespace swx
{

// TODO Remember that options need to default to values per the macros
// defined in CMakeLists.txt.
class Config
{
// static accessors
private:
	static Config& instance();
public:
	static unsigned int output_rounding_numerator();
	static unsigned int output_rounding_denominator();
	static unsigned int output_precision();
	static unsigned int output_width();
	static std::string format_string();
	static unsigned int formatted_buf_len();

// special member functions
private:
	Config();
	Config(Config const& rhs) = delete;
	Config(Config&& rhs) = delete;
	Config& operator=(Config const& rhs) = delete;
	Config& operator=(Config&& rhs) = delete;
	~Config();

// accessors
private:
	template <typename Value>
	void set_option(std::string const& p_key, Value const& p_value);

	template <typename Value>
	Value get_option(std::string const& p_key);

// helper functions
private:
	void load();
	void save();
	void do_load_from(std::string const& p_filepath);
	void do_save_to(std::string const& p_filepath);

// member variables
private:
	bool m_is_loaded;
	std::string m_filepath;
	std::map<std::string, std::string> m_map;

};  // class Config


// MEMBER TEMPLATE IMPLEMENTATIONS

template <typename Value>
void
Config::set_option(std::string const& p_key, Value const& p_value)
{
	load();
	std::ostringstream oss;
	oss << p_value;
	m_map[p_key] = oss.str();
	return;
}

template <typename Value>
Value
Config::get_option(std::string const& p_key)
{
	load();
	Value ret;
	auto const it = m_map.find(p_key);
	if (it == m_map.end())
	{
		std::ostringstream oss;
		oss << "Could not find configuration key: " << p_key;
		throw std::runtime_error(oss.str());
	}
	assert (it != m_map.end());
	std::string const raw = it->second;
	std::stringstream ss;
	ss << raw;
	ss >> ret;
	if (!ss)
	{
		std::ostringstream oss;
		oss << "Could not parse value for configuration key: " << p_key;
		throw std::runtime_error(oss.str());
	}
	return ret;
}

}  // namespace swx

#endif  // GUARD_config_hpp_32019735189163934
