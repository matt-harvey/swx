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
#include "arithmetic.hpp"
#include "seconds.hpp"
#include "stint.hpp"
#include "stream_utilities.hpp"
#include <cassert>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::make_pair;
using std::map;
using std::ostringstream;
using std::ostream;
using std::runtime_error;
using std::string;
using std::vector;

namespace swx
{

SummaryReportWriter::SummaryReportWriter(vector<Stint> const& p_stints):
	ReportWriter(p_stints)
{
	assert (m_activity_seconds_map.empty());
}

SummaryReportWriter::~SummaryReportWriter() = default;

void
SummaryReportWriter::do_preprocess_stints
(	ostream& p_os,
	vector<Stint> const& p_stints
)
{
	(void)p_os; (void)p_stints;  // silence compiler warnings re. unused params.
	m_activity_seconds_map.empty();
	return;
}

void
SummaryReportWriter::do_process_stint(std::ostream& p_os, Stint const& p_stint)
{
	(void)p_os;  // silence compiler warning re. unused param.
	unsigned long long const seconds = p_stint.interval().duration().count();
	auto const& activity = p_stint.activity();
	if (!activity.empty())
	{
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
				oss << "Time spent on activity \"" << activity
					<< "\" is too great to be totalled correctly.";
				throw runtime_error(oss.str());
			}
			assert (addition_is_safe(accum, seconds));
			accum += seconds;
		}
	}
	return;
}

void
SummaryReportWriter::do_postprocess_stints
(	ostream& p_os,
	vector<Stint> const& p_stints
)
{
	(void)p_stints;  // silence compiler warning re. unused param.
	do_write_summary(p_os, m_activity_seconds_map);
	return;
}

}  // namespace swx
