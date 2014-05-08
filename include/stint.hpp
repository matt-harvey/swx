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
#include <ostream>
#include <vector>

namespace swx
{

/**
 * Represents a period of time spent performing a specific activity.
 */
class Stint
{
// special member functions
public:

	/**
	 * Note \e p_activity is stored <em>as a reference</em>. The
	 * Stint object does not make its own copy of the string. The
	 * caller should ensure that the string used to initialize the Stint
	 * will persist for as long as the Stint persists.
	 */
	Stint(std::string const& p_activity, Interval const& p_interval);

// ordinary member functions
public:
	std::string const& activity() const;
	Interval interval() const;

// member variables
private:
	std::string const& m_activity;
	Interval m_interval;

};  // class Stint

std::ostream& print_stints_report
(	std::ostream& p_os,
	std::vector<Stint> const& p_container,
	bool p_print_summary = true,
	bool p_print_list = false
);

}  // namespace swx

#endif  // GUARD_stint_hpp_7450393879530204
