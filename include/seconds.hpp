/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_seconds_hpp_2689677263931221
#define GUARD_seconds_hpp_2689677263931221

#include <chrono>

namespace swx
{

/**
 * Represents a number of seconds elapsed.
 */
typedef std::chrono::duration<unsigned long long> Seconds;

}  // namespace swx

#endif  // GUARD_seconds_hpp_2689677263931221
