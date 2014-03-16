/*
 * Copyright 2014 Matthew Harvey
 */

#include "time_log.hpp"
#include "time_point.hpp"
#include "activity.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

using std::getline;

#ifndef __GNUC__
	using std::get_time;
#endif

using std::ifstream;
using std::mktime;
using std::remove;
using std::runtime_error;
using std::string;
using std::stringstream;

namespace chrono = std::chrono;

namespace swx
{

namespace
{
	string trim(string p_string);
	TimePoint time_stamp_to_point(string const& p_time_stamp);

	string trim(string p_string)
	{
		stringstream ss;
		ss << p_string;
		string ret;
		ss >> p_string;
		return p_string;
	}

	TimePoint time_stamp_to_point(string const& p_time_stamp)
	{
		tm tm;
		static string const format("%Y-%m-%d %H:%M:%S");
		bool parse_error = false;
#		ifdef __GNUC__
			if (strptime(p_time_stamp.c_str(), format.c_str(), &tm) == nullptr)
			{
				parse_error = true;
			}
#		else
			stringstream ss;
			ss << p_time_stamp;
			ss >> get_time(&tm, format.c_str());
			if (!ss)
			{
				parse_error = true;
			}
#		endif
		if (parse_error)
		{
			throw runtime_error("Could not parse timestamp.");
		}
		return chrono::system_clock::from_time_t(mktime(&tm));
	}

}  // end anonymous namespace

TimeLog::TimeLog(string const& p_filepath)
{
	assert (m_entries.empty());
	assert (m_activities.empty());
	assert (m_activity_map.empty());
	load(p_filepath);
}

void
TimeLog::load(string const& p_filepath)
{
	ifstream infile(p_filepath.c_str());
	string line;
	while (getline(infile, line))
	{
		register_entry(line);	
	}
	return;
}

ActivityId
TimeLog::register_activity(string const& p_activity_name)
{
	auto const it = m_activity_map.find(p_activity_name);
	if (it == m_activity_map.end())
	{
		// TODO Make this atomic?
		auto const ret = m_activities.size();
		Activity const activity(p_activity_name);
		m_activities.push_back(activity);
		m_activity_map[p_activity_name] = ret;
		return ret;
	}
	return it->second;
}

void
TimeLog::register_entry(string const& p_entry_string)
{
	static string const time_format("YYYY-MM-DD HH:MM:SS");
	if (p_entry_string.size() < time_format.size())
	{
		throw runtime_error("TimeLog parsing error.");	
	}
	auto it = p_entry_string.begin() + time_format.size();
	string const time_stamp(p_entry_string.begin(), it);
	string const activity_name = trim(string(it, p_entry_string.end()));
	Entry entry;
	entry.activity_id = register_activity(activity_name);
	entry.time_point = time_stamp_to_point(time_stamp);
	m_entries.push_back(entry);
	if (m_entries.size() >= 2)
	{
		// Then we have an Interval, which we need to push onto the
		// appropriate Activity.
		auto const& previous_entry = m_entries[m_entries.size() - 2];
		auto const previous_activity_id = previous_entry.activity_id;
		auto const previous_time_point = previous_entry.time_point;
		auto const time_point = entry.time_point;
		if (time_point < previous_time_point)
		{
			throw std::runtime_error("Time points out of order.");
		}
		Interval interval
		(	previous_time_point,
			chrono::duration_cast<Seconds>(time_point - previous_time_point)
		);
		Activity& previous_activity = m_activities[previous_activity_id];
		previous_activity.push_stint(interval);
	}
	return;
}

}  // namespace swx
