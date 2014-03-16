/*
 * Copyright 2014 Matthew Harvey
 */

#include "time_log.hpp"
#include "time_point.hpp"
#include "activity.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

using std::getline;
using std::ifstream;
using std::remove;
using std::string;
using std::stringstream;

namespace swx
{

namespace
{
	string trim(string p_orig)
	{
		stringstream ss;
		ss << p_orig;
		string ret;
		ss >> ret;
		return ret;
	}

	TimePoint time_stamp_to_point(string const& p_time_stamp)
	{
		// TODO Implement properly.
		(void)p_time_stamp;
		return TimePoint();
	}

}  // end anonymous namespace

TimeLog::TimeLog(std::string const& p_filepath)
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
TimeLog::register_activity(std::string const& p_activity_name)
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
	// TODO Also need to push stint onto Activity
	return it->second;
}

void
TimeLog::register_entry(string const& p_entry_string)
{
	static string const time_format("YYYY-MM-DD HH:MM:SS");
	if (p_entry_string.size() < time_format.size())
	{
		throw std::runtime_error("TimeLog parsing error.");	
	}
	auto it = p_entry_string.begin() + time_format.size();
	string const time_stamp(p_entry_string.begin(), it);
	string const activity_name = trim(string(it, p_entry_string.end()));
	Entry entry;
	entry.activity_id = register_activity(activity_name);
	entry.time_point = time_stamp_to_point(time_stamp);
	m_entries.push_back(entry);
	return;
}

}  // namespace swx
