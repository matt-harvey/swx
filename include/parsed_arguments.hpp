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

#ifndef GUARD_parsed_arguments_hpp_2991627102248197
#define GUARD_parsed_arguments_hpp_2991627102248197

#include <set>
#include <string>
#include <vector>

namespace swx
{

/**
 * Represents a set of boolean single-character flags and a set of
 * non-flag arguments.
 */
class ParsedArguments
{
// special member functions
public:
	explicit ParsedArguments(std::vector<std::string> const& p_raw_args);

// ordinary member functions
public:

	/**
	 * @returns ordinary arguments in the order they appear.
	 */
	std::vector<std::string> ordinary_args() const;

	/**
	 * @returns single character flags in alphabetical order.
	 */
	std::string single_character_flags() const;

// member variables
private:
	std::set<char> m_single_character_flags;
	std::vector<std::string> m_ordinary_args;

};  // class ParsedArguments

}  // namespace swx

#endif  // GUARD_parsed_arguments_hpp_2991627102248197
