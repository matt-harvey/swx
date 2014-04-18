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

#ifndef GUARD_interval_hpp_6955864893717628
#define GUARD_interval_hpp_6955864893717628

#include "seconds.hpp"
#include "time_point.hpp"
#include <chrono>
#include <ostream>
#include <vector>

namespace swx
{

/**
 * Represent an interval of time. It may be open (still running) or
 * closed (has an end).
 */
class Interval
{
// special member functions
public:

	explicit Interval
	(	TimePoint const& p_beginning,
		Seconds const& p_duration,
		bool p_is_live = false
	);

// ordinary member functions
public:
	Seconds duration() const;
	TimePoint beginning() const;
	TimePoint ending() const;
	bool is_live() const;

// member variables
private:
	bool m_is_live;
	TimePoint m_beginning;
	Seconds m_duration;

};  // class Interval

std::ostream& operator<<
(	std::ostream& os,
	std::vector<Interval> const& container
);

}  // namespace swx

#endif  // GUARD_interval_hpp_6955864893717628
