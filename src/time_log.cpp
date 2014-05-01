/*
 * Copyright 2014 Matthew Harvey
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "time_log.hpp"
#include "stint.hpp"
#include "string_utilities.hpp"
#include "time_point.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::endl;
using std::getline;
using std::ifstream;
using std::isspace;
using std::ios;
using std::move;
using std::ofstream;
using std::ostringstream;
using std::remove;
using std::runtime_error;
using std::size_t;
using std::string;
using std::stringstream;
using std::tm;
using std::upper_bound;
using std::vector;

namespace chrono = std::chrono;

namespace swx
{

namespace
{
	string::size_type expected_time_stamp_length()
	{
		static string const time_format = time_point_to_stamp(now());
		return time_format.length();
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
(	string const& p_activity,
	TimePoint const& p_time_point
)
{
	// TODO Make this atomic?
	mark_cache_as_stale();
	ofstream outfile(m_filepath.c_str(), ios::app);
	outfile << time_point_to_stamp(p_time_point)
	        << ' ' << p_activity << endl;
	return;
}

vector<Stint>
TimeLog::get_stints
(	string const* p_activity,
	TimePoint const* p_begin,
	TimePoint const* p_end
)
{
	load();	
	vector<Stint> ret;
	auto const e = m_entries.end();
	auto it = (p_begin? find_entry_just_before(*p_begin): m_entries.begin());
	auto const n = now();
	ActivityId const sought_activity_id = (p_activity? register_activity(*p_activity): 0);
	for ( ; (it != e) && (!p_end || (it->time_point < *p_end)); ++it)
	{
		ActivityId const current_activity_id = it->activity_id;
		if (!p_activity || (sought_activity_id == current_activity_id))
		{
			auto tp = it->time_point;
			if (p_begin && (tp < *p_begin)) tp = *p_begin;
			auto const next_it = it + 1;
			auto const done = (next_it == e);
			auto next_tp = (done? (n > tp? n: tp): next_it->time_point);
			if (p_end && (next_tp > *p_end)) next_tp = *p_end;
			assert (next_tp >= tp);
			assert (!p_begin || (tp >= *p_begin));
			assert (!p_end || (next_tp <= *p_end));
			auto const seconds = chrono::duration_cast<Seconds>(next_tp - tp);
			Interval const interval(tp, seconds, done);
			ret.push_back(Stint(id_to_activity(current_activity_id), interval));
		}
	}
	return ret;	
}

vector<string>
TimeLog::last_activities(size_t p_num)
{
	load();
	vector<string> ret;
	if (m_entries.empty())
	{
		return ret;
	}
	assert (m_entries.size() >= 1);
	auto const b = m_entries.begin();
	auto it = m_entries.end();
	--it;
	auto last_id = it->activity_id;
	assert (last_id == m_entries.back().activity_id);
	ret.push_back(id_to_activity(last_id));
	for (--it; (it != b) && (ret.size() != p_num); --it)
	{
		assert (ret.size() >= 1);
		auto const current_id = it->activity_id;
		if (current_id != last_id)
		{
			ret.push_back(id_to_activity(current_id));
			last_id = current_id;
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
	if (!m_is_loaded)
	{
		clear_cache();
		ifstream infile(m_filepath.c_str());
		string line;
		size_t line_number = 1;
		while (getline(infile, line))
		{
			load_entry(line, line_number);
			++line_number;
		}
		m_is_loaded = true;
	}
	return;
}

TimeLog::ActivityId
TimeLog::register_activity(string const& p_activity)
{
	auto const it = m_activity_map.find(p_activity);
	if (it == m_activity_map.end())
	{
		auto const ret = m_activities.size();
		m_activities.push_back(p_activity);
		try
		{
			m_activity_map.insert(ActivityMap::value_type(p_activity, ret));
		}
		catch (...)
		{
			m_activities.pop_back();
			throw;
		}
		return ret;
	}
	return it->second;
}

void
TimeLog::load_entry(string const& p_entry_string, size_t p_line_number)
{
	if (p_entry_string.size() < expected_time_stamp_length())
	{
		ostringstream oss;
		oss << "TimeLog parsing error at line "
		    << p_line_number << '.';
		throw runtime_error(oss.str());
	}
	auto it = p_entry_string.begin() + expected_time_stamp_length();
	assert (it > p_entry_string.begin());
	string const time_stamp(p_entry_string.begin(), it);
	string const activity = trim(string(it, p_entry_string.end()));
	auto const activity_id = register_activity(activity);
	auto const time_point = time_stamp_to_point(time_stamp);
	Entry entry(activity_id, time_point);
	if (!m_entries.empty())
	{
		auto const last_time_point = m_entries.back().time_point;
		if (entry.time_point < last_time_point)
		{		
			ostringstream oss;
			oss << "TimeLog entries out of order at line "
			    << p_line_number << '.'; 
			throw runtime_error(oss.str());
		}
	}
	m_entries.push_back(entry);
	return;
}

string const&
TimeLog::id_to_activity(ActivityId p_activity_id)
{
	load();
	return m_activities.at(p_activity_id);
}

vector<TimeLog::Entry>::const_iterator
TimeLog::find_entry_just_before(TimePoint const& p_time_point)
{
	load();
	auto const comp = [](Entry const& lhs, Entry const& rhs)
	{
		return lhs.time_point < rhs.time_point;
	};
	auto const b = m_entries.begin(), e = m_entries.end();
	Entry const dummy(0, p_time_point);
	auto it = upper_bound(b, e, dummy, comp);
	for ( ; (it != b) && ((it == e) || (it->time_point > p_time_point)); --it)
	{
	}
	return it;
}

TimeLog::Entry::Entry(ActivityId p_activity_id, TimePoint const& p_time_point):
	activity_id(p_activity_id),
	time_point(p_time_point)
{
}

}  // namespace swx
