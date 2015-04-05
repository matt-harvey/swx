/*
 * Copyright 2015 Matthew Harvey
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

#include "activity_info.hpp"
#include "arithmetic.hpp"
#include "time_point.hpp"
#include <algorithm>
#include <stdexcept>

using std::max;
using std::min;
using std::runtime_error;

namespace swx
{

ActivityInfo::ActivityInfo
(	unsigned long long p_seconds,
	TimePoint const& p_beginning,
	TimePoint const& p_ending,
	unsigned int p_num_children
):
	seconds(p_seconds),
	num_children(p_num_children),
	beginning(p_beginning),
	ending(p_ending)
{
}

ActivityInfo&
operator+=(ActivityInfo& lhs, ActivityInfo const& rhs)
{
	if (!addition_is_safe(lhs.seconds, rhs.seconds))
	{
		throw runtime_error("Cannot safely sum number of seconds spent on activity.");
	}
	lhs.seconds += rhs.seconds;
	lhs.beginning = min(lhs.beginning, rhs.beginning);
	lhs.ending = max(lhs.ending, rhs.ending);
	return lhs;
}

ActivityInfo const
operator+(ActivityInfo lhs, ActivityInfo const& rhs)
{
	return lhs += rhs;
}

}  // namespace swx
