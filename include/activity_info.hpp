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

#ifndef GUARD_activity_info_hpp_743408902428769
#define GUARD_activity_info_hpp_743408902428769

#include "time_point.hpp"

namespace swx
{

/**
 * Holds information about an activity for use in report.
 * 
 * \b seconds number of seconds spent on the activity
 *
 * \b beginning earliest time at which the activity was conducted during
 * the reported period
 *
 * \b ending latest time at which activity was conducted during the
 * reported period
 *
 * \b num_children number of children of node in activity tree
 *
 * @todo It feels wrong that num_children is held here. Should probably refactor
 * things so that each ActivityNode knows what its children are, and then
 * this wouldn't be required here.
 */
struct ActivityInfo
{
	explicit ActivityInfo
	(	unsigned long long p_seconds = 0,
		TimePoint const& p_beginning = TimePoint::max(),
		TimePoint const& p_ending = TimePoint::min(),
		unsigned int p_num_children = 0
	);
	unsigned long long seconds;
	unsigned int num_children;
	TimePoint beginning;
	TimePoint ending;

};  // struct ActivityInfo

ActivityInfo&
operator+=(ActivityInfo& lhs, ActivityInfo const& rhs);

ActivityInfo const
operator+(ActivityInfo lhs, ActivityInfo const& rhs);

}  // namespace swx

#endif  // GUARD_activity_info_hpp_743408902428769
