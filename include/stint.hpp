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

#ifndef GUARD_stint_hpp_7450393879530204
#define GUARD_stint_hpp_7450393879530204

#include "interval.hpp"
#include <string>

namespace swx
{

class Stint
{
// special member functions
public:
	Stint(std::string const& p_activity_name, Interval const& p_interval);
	Stint(Stint const& rhs) = default;
	Stint(Stint&& rhs) = default;
	Stint& operator=(Stint const& rhs) = default;
	Stint& operator=(Stint&& rhs) = default;
	~Stint() = default;

// ordinary member functions
public:
	std::string activity_name() const;
	Interval interval() const;

// member variables
private:
	std::string const& m_activity_name;
	Interval m_interval;

};  // class Stint

}  // namespace swx

#endif  // GUARD_stint_hpp_7450393879530204
