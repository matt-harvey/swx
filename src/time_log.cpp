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
#include "atomic_writer.hpp"
#include "file_utilities.hpp"
#include "stint.hpp"
#include "stream_utilities.hpp"
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
#include <boost/regex.hpp>

using boost::match_posix;
using boost::regex;
using boost::regex_search;
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
}

void
TimeLog::append_entry(string const& p_activity)
{
	AtomicWriter writer(m_filepath);
	mark_cache_as_stale();
	writer.append(time_point_to_stamp(now()));
	if (!p_activity.empty())
	{
		writer.append(" ");
		writer.append(p_activity);
	}
	writer.append("\n");
	writer.commit();
	mark_cache_as_stale();
	return;
}

vector<Stint>
TimeLog::get_stints
(	string const* p_sought_activity,
	TimePoint const* p_begin,
	TimePoint const* p_end,
	bool p_use_regex
)
{
	load();	
	regex reg;
	if (!p_sought_activity) p_use_regex = false;
	if (p_use_regex)
	{
		assert (p_sought_activity);
		reg = regex(*p_sought_activity, regex::extended);
	}
	vector<Stint> ret;
	auto const e = m_entries.end();
	auto it = (p_begin? find_entry_just_before(*p_begin): m_entries.begin());
	auto const n = now();
	auto const sought_id =
		(p_sought_activity? register_activity(*p_sought_activity): 0);
	for ( ; (it != e) && (!p_end || (it->time_point < *p_end)); ++it)
	{
		ActivityId const activity_id = it->activity_id;
		if (!p_sought_activity || (sought_id == activity_id) || p_use_regex)
		{
			auto const& activity = id_to_activity(activity_id);
			if (!p_use_regex || regex_search(activity, reg, match_posix))
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
				auto const duration = next_tp - tp;
				auto const seconds = chrono::duration_cast<Seconds>(duration);
				Interval const interval(tp, seconds, done);
				ret.push_back(Stint(activity, interval));
			}
		}
	}
	return ret;	
}

string
TimeLog::last_activity_to_match(string const& p_regex)
{
	load();
	regex const reg(p_regex, regex::extended);
	typedef decltype(m_entries)::const_reverse_iterator RevIter;
	ActivityId const empty_activity_id = register_activity("");
	for (RevIter rit = m_entries.rbegin(); rit != m_entries.rend(); ++rit)
	{
		ActivityId const id = rit->activity_id;
		if (id != empty_activity_id)
		{
			string const& activity = id_to_activity(id);
			if (regex_search(activity, reg, match_posix))
			{
				return activity;
			}
		}
	}
	return string();
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
	typedef decltype(m_entries)::const_reverse_iterator RevIter;
	for (RevIter it = m_entries.rbegin(); it != m_entries.rend(); ++it)
	{
		if (ret.size() == p_num)
		{
			break;
		}
		auto const& activity = id_to_activity(it->activity_id);
		if (!activity.empty() && (ret.empty() || (activity != ret.back())))
		{
			ret.push_back(activity);
		}
	}
	assert (ret.size() <= p_num);
	assert (ret.size() <= m_entries.size());
	return ret;
}

bool
TimeLog::is_active()
{
	load();
	if (m_entries.empty())
	{
		return false;
	}
	return !id_to_activity(m_entries.back().activity_id).empty();
}

bool
TimeLog::has_activity(string const& p_activity)
{
	load();
	return m_activities.find(p_activity) != m_activities.end();
}

void
TimeLog::clear_cache()
{
	m_entries.clear();
	m_activities.clear();
	mark_cache_as_stale();
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
		if (file_exists_at(m_filepath))
		{
			ifstream infile(m_filepath.c_str());
			enable_exceptions(infile);
			string line;
			size_t line_number = 1;
			while (infile.peek() != EOF)
			{
				getline(infile, line);
				load_entry(line, line_number);
				++line_number;
			}
			if (!m_entries.empty() && (m_entries.back().time_point > now()))
			{
				throw runtime_error
				(	"The final entry in the time log is future-dated. "
				    "Future dated entries are not supported."
				);
			}
		}
		m_is_loaded = true;
	}
	return;
}

TimeLog::ActivityId
TimeLog::register_activity(string const& p_activity)
{
	return &*(m_activities.insert(p_activity).first);
}

void
TimeLog::load_entry(string const& p_entry_string, size_t p_line_number)
{
	if (p_entry_string.size() < expected_time_stamp_length())
	{
		ostringstream oss;
		enable_exceptions(oss);
		oss << "Error parsing the time log at line "
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
			enable_exceptions(oss);
			oss << "Time log entries out of order at line "
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
	return *p_activity_id;
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
