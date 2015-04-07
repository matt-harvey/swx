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

#ifndef GUARD_activity_stats_hpp_743408902428769
#define GUARD_activity_stats_hpp_743408902428769

#include "time_point.hpp"

namespace swx
{

/**
 * Holds statistics about an activity for use in report.
 * 
 * \b seconds number of seconds spent on the activity
 *
 * \b beginning earliest time at which the activity was conducted during
 * the reported period
 *
 * \b ending latest time at which activity was conducted during the
 * reported period
 */
struct ActivityStats
{
	explicit ActivityStats
	(	unsigned long long p_seconds = 0,
		TimePoint const& p_beginning = TimePoint::max(),
		TimePoint const& p_ending = TimePoint::min()
	);
	unsigned long long seconds;
	TimePoint beginning;
	TimePoint ending;

};  // struct ActivityStats

ActivityStats&
operator+=(ActivityStats& lhs, ActivityStats const& rhs);

ActivityStats const
operator+(ActivityStats lhs, ActivityStats const& rhs);

}  // namespace swx

#endif  // GUARD_activity_stats_hpp_743408902428769
