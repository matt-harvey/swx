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

#include "stream_utilities.hpp"
#include <cassert>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

namespace swx
{

/**
 * Manages application configuration options and the configuration file.
 */
class Config
{
// nested types
private:
    struct OptionData
    {
        OptionData() = default;
        OptionData
        (   std::string const& p_value,
            std::string const& p_description
        );
        std::string value;
        std::string description;
    };

// special member functions
public:
    explicit Config(std::string const& p_filepath);
    Config(Config const& rhs) = delete;
    Config(Config&& rhs) = delete;
    Config& operator=(Config const& rhs) = delete;
    Config& operator=(Config&& rhs) = delete;
    ~Config();

// ordinary member functions
public:
    std::string filepath() const;
    unsigned int output_rounding_numerator() const;
    unsigned int output_rounding_denominator() const;
    unsigned int output_precision() const;
    unsigned int output_width() const;
    std::string time_format() const;
    unsigned int formatted_buf_len() const;
    std::string editor() const;
    std::string path_to_log() const;

    /**
     * @returns a printable summary of configuration settings.
     */
    std::string summary() const;

private:
    void set_option_value(std::string const& p_key, std::string const& p_value);

    void unchecked_set_option
    (   std::string const& p_key,
        OptionData const& p_option_data
    );

    template <typename Value>
    Value get_option_value(std::string const& p_key) const;

    std::string get_raw_option_value(std::string const& p_key) const;
    void set_defaults();
    void initialize_config_file();

// member variables
private:
    std::string m_filepath;
    std::map<std::string, OptionData> m_map;

};  // class Config


// MEMBER TEMPLATE IMPLEMENTATIONS

template <typename Value>
Value
Config::get_option_value(std::string const& p_key) const
{
    Value ret;
    auto const raw_value = get_raw_option_value(p_key);
    std::stringstream ss(raw_value);
    ss >> ret;
    if (!ss)
    {
        std::ostringstream oss;
        enable_exceptions(oss);
        oss << "Could not parse value for configuration key \"" << p_key
            << "\" from value string \"" << raw_value << "\"";
        throw std::runtime_error(oss.str());
    }
    return ret;
}

template <>
inline
std::string
Config::get_option_value<std::string>(std::string const& p_key) const
{
    return get_raw_option_value(p_key);
}

}  // namespace swx

#endif  // GUARD_config_hpp_32019735189163934
