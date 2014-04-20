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

#ifndef GUARD_info_hpp_9351167767006516
#define GUARD_info_hpp_9351167767006516

#include <string>

namespace swx
{

/**
 * Manages information about the application as a whole, e.g. version.
 */
class Info
{
// special member functions
public:
	Info() = delete;
	Info(Info const& rhs) = delete;
	Info(Info&& rhs) = delete;
	Info& operator=(Info const& rhs) = delete;
	Info& operator=(Info&& rhs) = delete;
	~Info() = delete;

// static functions
public:
	static std::string application_name();
	static std::string version();
	static std::string home_dir();

};  // class Info

}  // namespace swx

#endif  // GUARD_info_hpp_9351167767006516
