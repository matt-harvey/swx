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
#include <vector>

using std::numeric_limits;
using std::ostringstream;
using std::ostream;
using std::runtime_error;
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

SummaryReportWriter::SummaryReportWriter() = default;

SummaryReportWriter::~SummaryReportWriter() = default;

void
SummaryReportWriter::do_pre_write(ostream& p_os, vector<Stint> const& p_stints)
{
	(void)p_os;  // silence compiler warning re. unused parameter
	if (m_activity_seconds_map.empty())
	{
		for (auto const& stint: p_stints)
		{
			unsigned long long const seconds =
				stint.interval().duration().count();
			auto const& activity = stint.activity();
			auto const it = m_activity_seconds_map.find(activity);
			if (it == m_activity_seconds_map.end())
			{
				m_activity_seconds_map.insert(make_pair(activity, seconds));
			}
			else
			{
				auto& accum = it->second;
				if (!addition_is_safe(accum, seconds))
				{
					ostringstream oss;
					enable_exceptions(oss);
					oss << "Time spent on activity \"" << activity <<
					    << "\" is too great to be totalled correctly.";
					throw runtime_error(oss.str());
				}
				assert (addition_is_safe(accum, seconds));
				accum += seconds;
			}
		}
	}
	return;
}

}  // namespace swx
