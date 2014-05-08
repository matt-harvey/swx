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

#include "stint.hpp"
#include "config.hpp"
#include "interval.hpp"
#include "round.hpp"
#include "stream_flag_guard.hpp"
#include <cassert>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <map>
#include <string>
#include <vector>

using std::endl;
using std::fixed;
using std::ios_base;
using std::left;
using std::map;
using std::ostream;
using std::right;
using std::setprecision;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

namespace
{
	ostream& print_activity
	(	ostream& p_os,
		string const& p_name,
		unsigned int p_width
	)
	{
		StreamFlagGuard guard(p_os);
		p_os << left << setw(p_width) << p_name;
		return p_os;
	}

	ostream& print_hours
	(	ostream& p_os,
		double p_hours,
		unsigned int p_rounding_num,
		unsigned int p_rounding_den,
		unsigned int p_precision,
		unsigned int p_width
	)
	{
		StreamFlagGuard guard(p_os);
		p_os << fixed << setprecision(p_precision) << right
		    << setw(p_width)
		    << round(p_hours, p_rounding_num, p_rounding_den);
		return p_os;
	}

	ostream& print_time_point(ostream& p_os, TimePoint const& p_time_point)
	{
		StreamFlagGuard guard(p_os);
		p_os << time_point_to_stamp(p_time_point);
		return p_os;
	}

}  // end anonymous namespace

Stint::Stint(string const& p_activity, Interval const& p_interval):
	m_activity(p_activity),
	m_interval(p_interval)
{
}

string const&
Stint::activity() const
{
	return m_activity;
}

Interval
Stint::interval() const
{
	return m_interval;
}

ostream&
operator<<(ostream& os, vector<Stint> const& container)
{
	map<string, double> accum_map;
	double accum_hours = 0.0;
	string live_activity;
	auto const gap = "  ";
	auto const rounding_num = Config::output_rounding_numerator();
	auto const rounding_den = Config::output_rounding_denominator();
	auto const w = Config::output_width();
	auto const prec = Config::output_precision();
	bool last_activity_empty = true;
	for
	(	vector<Stint>::const_iterator it = container.begin();
		it != container.end();
		++it
	)
	{
		string const activity = it->activity();
		if (activity.empty())
		{
			if (!last_activity_empty) os << endl;	
			last_activity_empty = true;
		}
		else
		{
			last_activity_empty = false;
			auto const interval = it->interval();
			double hours = interval.duration().count() / 60.0 / 60.0;
			accum_hours += hours;
			auto const it = accum_map.find(activity);
			if (it == accum_map.end())
			{
				accum_map[activity] = hours;
			}
			else
			{
				it->second += hours;
			}
			print_time_point(os, interval.beginning()) << gap;
			print_time_point(os, interval.ending()) << gap;
			print_hours(os, hours, rounding_num, rounding_den, prec, w);
			if (interval.is_live())
			{
				os << '*';
				live_activity = activity;
			}
			else
			{
				os << ' ';
			}
			os << gap << activity << endl;
		}
	}
	os << endl;
	string::size_type max_name_width = 0;
	for (auto const& pair: accum_map)
	{
		string const& activity = pair.first;
		auto const length = activity.length();
		if (length > max_name_width) max_name_width = length;
	}
	for (auto const& pair: accum_map)
	{
		string const& activity = pair.first;
		double const hours = pair.second;
		print_activity(os, activity, max_name_width);
		print_hours(os, hours, rounding_num, rounding_den, prec, w);
		if (live_activity == activity) os << '*';
		os << endl;
	}
	os << endl;
	print_activity(os, "TOTAL", max_name_width);
	print_hours(os, accum_hours, rounding_num, rounding_den, prec, w);
	os << endl;
	if (!live_activity.empty()) os << "\n*ongoing" << endl;
	return os;
}

}  // namespace swx
