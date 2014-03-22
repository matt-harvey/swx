/*
 * Copyright 2014 Matthew Harvey
 */

#include "time_log.hpp"
#include "time_conversion.hpp"
#include "time_point.hpp"
#include "activity.hpp"
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

namespace
{
	string trim(string const& p_string)
	{
		auto it = p_string.begin();
		for ( ; it != p_string.end() && isspace(*it); ++it)
		{
		}
		string ret(it, p_string.end());
		it = ret.end();
		if (!ret.empty()) --it;
		string::size_type num_to_pop = 0;
		for ( ; it >= p_string.begin() && isspace(*it); --it, ++num_to_pop)
		{
		}
		assert (num_to_pop <= ret.size());
		ret.resize(ret.size() - num_to_pop);
		return ret;
	}

}  // end anonymous namespace

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

Activity*
TimeLog::get_activity(string const& p_activity_name)
{
	load();
	auto const it = m_activity_map.find(p_activity_name);
	if (it == m_activity_map.end())
	{
		return nullptr;
	}
	return &(m_activities.at(it->second));
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
		auto const ret = m_activities.size();
		Activity const activity(p_activity_name);
		m_activities.push_back(activity);
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
	if (m_entries.size() >= 2)
	{
		// Then we have an Interval, which we need to push onto the
		// appropriate Activity.
		auto const& previous_entry = m_entries[m_entries.size() - 2];
		auto const previous_activity_id = previous_entry.activity_id;
		auto const previous_time_point = previous_entry.time_point;
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

TimeLog::Entry::Entry(ActivityId p_activity_id, TimePoint const& p_time_point):
	activity_id(p_activity_id),
	time_point(p_time_point)
{
}

}  // namespace swx
