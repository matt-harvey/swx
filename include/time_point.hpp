/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_time_point_hpp_285827964211734
#define GUARD_time_point_hpp_285827964211734

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
TimePoint start_of_day(TimePoint const& p_time_point);

std::tm time_point_to_tm(TimePoint const& p_time_point);
TimePoint tm_to_time_point(std::tm const& p_tm);
TimePoint time_stamp_to_point(std::string const& p_time_stamp);
std::string time_point_to_stamp(TimePoint const& p_time_point);


}  // namespace swx

#endif  // GUARD_time_point_hpp_285827964211734
