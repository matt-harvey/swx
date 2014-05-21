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

#include "summary_report_writer.hpp"
#include "seconds.hpp"
#include "stint.hpp"
#include "stream_utilities.hpp"
#include <cassert>
#include <limits>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using std::map;
using std::numeric_limits;
using std::ostringstream;
using std::ostream;
using std::runtime_error;
using std::string;
using std::vector;

namespace swx
{

namespace
{
	template <typename T>
	bool addition_is_safe(T p_x, T p_y);

	template <>
	bool addition_is_safe(unsigned long long p_x, unsigned long long p_y)
	{
		return p_x <= numeric_limits<unsigned long long>::max() - p_y;
	}

}  // end anonymous namespace

SummaryReportWriter::SummaryReportWriter(vector<Stint> const& p_stints):
	ReportWriter(p_stints)
{
}

SummaryReportWriter::~SummaryReportWriter() = default;

void
SummaryReportWriter::do_post_write(ostream& p_os, vector<Stint> const& p_stints)
{
	map<string, unsigned long long> activity_seconds_map;
	for (auto const& stint: p_stints)
	{
		unsigned long long const seconds =
			stint.interval().duration().count();
		auto const& activity = stint.activity();
		auto const it = activity_seconds_map.find(activity);
		if (it == activity_seconds_map.end())
		{
			activity_seconds_map.insert(make_pair(activity, seconds));
		}
		else
		{
			auto& accum = it->second;
			if (!addition_is_safe(accum, seconds))
			{
				ostringstream oss;
				enable_exceptions(oss);
				oss << "Time spent on activity \"" << activity
					<< "\" is too great to be totalled correctly.";
				throw runtime_error(oss.str());
			}
			assert (addition_is_safe(accum, seconds));
			accum += seconds;
		}
	}
	for (auto const& pair: activity_seconds_map)
	{
		do_write_activity_hours
		(	p_os,
			pair.first,
			seconds_to_rounded_hours(pair.second)
		);
	}
	return;
}

}  // namespace swx
