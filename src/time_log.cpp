/*
 * Copyright 2014 Matthew Harvey
 */

#include "time_log.hpp"
#include "activity.hpp"
#include "string_utilities.hpp"
#include "time_conversion.hpp"
#include "time_point.hpp"
#include "time_conversion.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>  // TEMP for logging
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using std::endl;
using std::getline;
using std::ifstream;
using std::isspace;
using std::ios;
using std::mktime;
using std::ofstream;
using std::remove;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::tm;

// TODO Need better error reporting on parsing error.

namespace chrono = std::chrono;

namespace swx
{

TimeLog::TimeLog(string const& p_filepath):
	m_is_loaded(false),
	m_filepath(p_filepath)
{
	assert (m_entries.empty());
	assert (m_activities.empty());
	assert (m_activity_map.empty());
}

void
TimeLog::append_entry
(	Activity const& p_activity,
	TimePoint const& p_time_point
)
{
	// TODO Make this atomic?
	mark_cache_as_stale();
	ofstream outfile(m_filepath.c_str(), ios::app);
	outfile << time_point_to_stamp(p_time_point)
	        << ' '
			<< p_activity.name()
			<< endl;
	return;
}

std::vector<Interval>
TimeLog::get_intervals_by_activity_name(string const& p_activity_name)
{
	load();
	std::vector<Interval> ret;
	auto const it = m_activity_map.find(p_activity_name);
	if (it == m_activity_map.end())
	{
		return ret;
	}
	auto const activity_id = it->second;
	for (decltype(m_entries)::size_type i = 0; i != m_entries.size(); ++i)
	{
		auto const& entry = m_entries[i];
		if (entry.activity_id == activity_id)
		{
			auto const time_point = entry.time_point;
			TimePoint* next_time_point_ptr = nullptr;
			bool is_live = false;
			if ((i + 1) == m_entries.size())
			{
				*next_time_point_ptr = now();
				is_live = true;
			}
			else
			{
				*next_time_point_ptr = m_entries[i + 1].time_point;
			}
			assert (next_time_point_ptr);
			Interval const interval
			(	time_point,
				chrono::duration_cast<Seconds>(*next_time_point_ptr - time_point),
				is_live
			);
			ret.push_back(std::move(interval));
		}
	}
	return ret;
}

void
TimeLog::clear_cache()
{
	m_entries.clear();
	m_activities.clear();
	m_activity_map.clear();
	return;
}

void
TimeLog::mark_cache_as_stale()
{
	m_is_loaded = false;
}

void
TimeLog::load()
{
	clear_cache();
	ifstream infile(m_filepath.c_str());
	string line;
	while (getline(infile, line))
	{
		load_entry(line);	
	}
	m_is_loaded = true;
	return;
}

ActivityId
TimeLog::register_activity(string const& p_activity_name)
{
	auto const it = m_activity_map.find(p_activity_name);
	if (it == m_activity_map.end())
	{
		// TODO Make this atomic?
		auto const ret = m_activity_names.size();
		m_activity_names.push_back(activity_name);
		m_activity_map[p_activity_name] = ret;
		return ret;
	}
	return it->second;
}

void
TimeLog::load_entry(string const& p_entry_string)
{
	static string const time_format("YYYY-MM-DD HH:MM:SS");
	if (p_entry_string.size() < time_format.size())
	{
		throw runtime_error("TimeLog parsing error.");	
	}
	auto it = p_entry_string.begin() + time_format.size();
	string const time_stamp(p_entry_string.begin(), it);
	string const activity_name = trim(string(it, p_entry_string.end()));
	auto const activity_id = register_activity(activity_name);
	auto const time_point = time_stamp_to_point(time_stamp);
	Entry entry(activity_id, time_point);
	m_entries.push_back(entry);
	return;
}

TimeLog::Entry::Entry(ActivityId p_activity_id, TimePoint const& p_time_point):
	activity_id(p_activity_id),
	time_point(p_time_point)
{
}

}  // namespace swx
