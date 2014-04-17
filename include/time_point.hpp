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

#ifndef GUARD_time_point_hpp_285827964211734
#define GUARD_time_point_hpp_285827964211734

#include "date.hpp"
#include <chrono>
#include <ctime>
#include <string>

namespace swx
{

typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

TimePoint now();

/**
 * The first TimePoint of the day on which p_time_point falls.
 */
TimePoint day_begin(TimePoint const& p_time_point);

/**
 * One TimePoint past the last TimePoint of the day on which p_time_point falls.
 */
TimePoint day_end(TimePoint const& p_time_point);

std::tm time_point_to_tm(TimePoint const& p_time_point);
TimePoint tm_to_time_point(std::tm const& p_tm);
TimePoint time_stamp_to_point(std::string const& p_time_stamp);
std::string time_point_to_stamp(TimePoint const& p_time_point);

}  // namespace swx

#endif  // GUARD_time_point_hpp_285827964211734
