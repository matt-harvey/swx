/*
 * Copyright 2014 Matthew Harvey
 */

#include "time_point.hpp"
#include <chrono>
#include <ctime>

namespace chrono = std::chrono;

using std::localtime;
using std::mktime;
using std::tm;
using std::time_t;

namespace swx
{

TimePoint now()
{
	return chrono::system_clock::now();
}

std::tm
time_point_to_tm(TimePoint const& p_time_point)
{
	time_t const time_time_t = chrono::system_clock::to_time_t(p_time_point);
	tm* const time_tm_ptr = localtime(&time_time_t);
	tm const ret = *time_tm_ptr;
	return ret;
}

TimePoint
tm_to_time_point(tm const& p_tm)
{
	tm time_tm = p_tm;
	time_t const time_time_t = mktime(&time_tm);
	return chrono::system_clock::from_time_t(time_time_t);
}

TimePoint
start_of_day(TimePoint const& p_time_point)
{
	tm time_tm = time_point_to_tm(p_time_point);
	time_tm.tm_hour = time_tm.tm_min = time_tm.tm_sec = 0;
	return tm_to_time_point(time_tm);
}

}   // namespace swx
