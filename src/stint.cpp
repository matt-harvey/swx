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

Stint::Stint(string const& p_activity_name, Interval const& p_interval):
	m_activity_name(p_activity_name),
	m_interval(p_interval)
{
}

string const&
Stint::activity_name() const
{
	return m_activity_name;
}

Interval
Stint::interval() const
{
	return m_interval;
}

ostream&
operator<<(ostream& os, vector<Stint> const& container)
{
	// TODO LOW PRIORITY Tidy this up.
	map<string, double> accum_map;
	double accum_hours = 0.0;
	string live_activity;
	assert (live_activity.empty());
	auto const gap = "  ";
	auto const rounding_num = Config::output_rounding_numerator();
	auto const rounding_den = Config::output_rounding_denominator();
	auto const output_width = Config::output_width();
	auto const output_precision = Config::output_precision();
	for (auto const& stint: container)
	{
		string const activity = stint.activity_name();
		if (!activity.empty())
		{
			StreamFlagGuard guard(os);
			Interval const interval = stint.interval();
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
			os << time_point_to_stamp(interval.beginning()) << gap
			   << time_point_to_stamp(interval.ending()) << gap;
			os << fixed << setprecision(output_precision) << right;
			os << setw(output_width)
			   << round(hours, rounding_num, rounding_den);
			if (interval.is_live())
			{
				os << '*';
				live_activity = activity;
			}
			else
			{
				os << ' ';
			}
			guard.reset();
			os << gap << activity << endl;
		}
	}
	os << endl;
	string::size_type max_activity_name_width = 0;
	for (auto const& pair: accum_map)
	{
		string const& activity_name = pair.first;
		auto const length = activity_name.length();
		if (length > max_activity_name_width)
		{
			max_activity_name_width = length;
		}
	}
	for (auto const& pair: accum_map)
	{
		StreamFlagGuard guard(os);
		string const& activity = pair.first;
		double const hours = pair.second;
		os << left << setw(max_activity_name_width) << activity;
		os << fixed
		   << setprecision(output_precision)
		   << right;
		os << setw(output_width) << round(hours, rounding_num, rounding_den);
		guard.reset();
		if (live_activity == activity)
		{
			os << '*';
		}
		os << endl;
	}
	// anonymous scope
	{
		StreamFlagGuard guard(os);
		os << '\n' << left << setw(max_activity_name_width) << "TOTAL";
		os << fixed
		   << setprecision(output_precision)
		   << right;
		os << setw(output_width)
		   << round(accum_hours, rounding_num, rounding_den) << endl;
	}
	if (!live_activity.empty())
	{
		os << "\n*ongoing" << endl;
	}
	return os;
}

}  // namespace swx
