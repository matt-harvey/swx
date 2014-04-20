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

class Config
{
// nested types
private:
	struct OptionData
	{
		OptionData() = default;
		OptionData
		(	std::string const& p_value,
			std::string const& p_description
		);
		std::string value;
		std::string description;  // should be one line
	};

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
	static std::string path_to_log();

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
	void set_option_value(std::string const& p_key, std::string const& p_value);

	void unchecked_set_option
	(	std::string const& p_key,
		OptionData const& p_option_data
	);

	template <typename Value>
	Value get_option(std::string const& p_key);

// helper functions
private:
	void load();
	void do_load_from(std::string const& p_filepath);
	void set_defaults();
	void initialize_config_file(std::string const& p_filepath);

// member variables
private:
	bool m_is_loaded;
	std::string m_filepath;
	std::map<std::string, OptionData> m_map;

};  // class Config


// MEMBER TEMPLATE IMPLEMENTATIONS

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
	std::stringstream ss(it->second.value);
	ss >> ret;
	if (!ss)
	{
		std::ostringstream oss;
		oss << "Could not parse value for configuration key \"" << p_key
		    << "\" from value string \"" << it->second.value << "\"";
		throw std::runtime_error(oss.str());
	}
	return ret;
}

}  // namespace swx

#endif  // GUARD_config_hpp_32019735189163934
