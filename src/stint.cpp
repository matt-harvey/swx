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

#include "stint.hpp"
#include "interval.hpp"
#include <string>

using std::string;

namespace swx
{

Stint::Stint(string const& p_activity_name, Interval const& p_interval):
	m_activity_name(p_activity_name),
	m_interval(p_interval)
{
}

string
Stint::activity_name() const
{
	return m_activity_name;
}

Interval
Stint::interval() const
{
	return m_interval;
}

}  // namespace swx
