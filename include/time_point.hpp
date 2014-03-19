/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_time_point_hpp_285827964211734
#define GUARD_time_point_hpp_285827964211734

#include <chrono>

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

// INLINE FUNCTION IMPLEMENTATIONS
inline
TimePoint now()
{
	return std::chrono::system_clock::now();
}

}  // namespace swx

#endif  // GUARD_time_point_hpp_285827964211734
