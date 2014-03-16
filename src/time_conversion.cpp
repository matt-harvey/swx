/*
 * Copyright 2014 Matthew Harvey
 */

#include "time_conversion.hpp"
#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdexcept>

namespace chrono = std::chrono;

using std::localtime;
using std::runtime_error;
using std::strftime;
using std::strlen;
using std::string;
using std::stringstream;
using std::time_t;
using std::tm;

#ifndef __GNUC__
	using std::get_time;
#endif

namespace swx
{

TimePoint time_stamp_to_point(string const& p_time_stamp)
{
	// don't make this static - caused odd bug with strptime
	char const format[] = SWX_FORMAT_STRING;

	tm tm;
	bool parse_error = false;
#	ifdef __GNUC__
		if (strptime(p_time_stamp.c_str(), format, &tm) == nullptr)
		{
			parse_error = true;
		}
#	else
		stringstream ss;
		ss << p_time_stamp;
		ss >> get_time(&tm, format);
		if (!ss)
		{
			parse_error = true;
		}
#	endif
	if (parse_error)
	{
		throw runtime_error("Could not parse timestamp.");
	}
	return chrono::system_clock::from_time_t(mktime(&tm));
}

string time_point_to_stamp(TimePoint const& p_time_point)
{
	// don't make this static - caused odd bug with strptime
	char const format[] = SWX_FORMAT_STRING;
	time_t const time_time_t = chrono::system_clock::to_time_t(p_time_point);
	auto const time_tmp = localtime(&time_time_t);
	char buf[SWX_FORMATTED_BUF_LEN];
	if (strftime(buf, SWX_FORMATTED_BUF_LEN, format, time_tmp) == 0)
	{
		throw runtime_error("Error formatting TimePoint.");
	}
	return string(buf);
}

}  // namespace swx
