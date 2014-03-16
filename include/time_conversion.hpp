/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_time_conversion_hpp_4901403805660738
#define GUARD_time_conversion_hpp_4901403805660738

#include "time_point.hpp"
#include <string>

namespace swx
{

TimePoint time_stamp_to_point(std::string const& p_time_stamp);

std::string time_point_to_stamp(TimePoint const& p_time_point);

}  // namespace swx

#endif  // GUARD_time_conversion_hpp_4901403805660738
