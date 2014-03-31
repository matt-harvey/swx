/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_time_point_hpp_285827964211734
#define GUARD_time_point_hpp_285827964211734

#include <chrono>
#include <ctime>

namespace swx
{

/**
 * Represents a point in time.
 */
typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

/**
 * Convenience function.
 */
TimePoint now();

/**
 * Convert to a std::tm.
 */
std::tm time_point_to_tm(TimePoint const& p_time_point);

/**
 * Convert from a std::tm.
 */
TimePoint tm_to_time_point(std::tm const& p_tm);

/**
 * The first TimePoint of the day on which p_time_point falls.
 */
TimePoint start_of_day(TimePoint const& p_time_point);

}  // namespace swx

#endif  // GUARD_time_point_hpp_285827964211734
