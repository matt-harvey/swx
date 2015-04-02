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

SummaryReportWriter::SummaryReportWriter
(	vector<Stint> const& p_stints,
	Options const& p_options
):
	ReportWriter(p_stints, p_options)
{
	assert (m_activity_info_map.empty());
}

SummaryReportWriter::~SummaryReportWriter() = default;

void
SummaryReportWriter::do_preprocess_stints
(	ostream& p_os,
	vector<Stint> const& p_stints
)
{
	(void)p_os; (void)p_stints;  // silence compiler warnings re. unused params.
	assert (m_activity_info_map.empty());
	return;
}

void
SummaryReportWriter::do_process_stint(std::ostream& p_os, Stint const& p_stint)
{
	(void)p_os;  // silence compiler warning re. unused param.
	auto const interval = p_stint.interval();
	unsigned long long const seconds = interval.duration().count();
	auto const& activity = p_stint.activity();
	if (!activity.empty())
	{
		auto const it = m_activity_info_map.find(activity);
		if (it == m_activity_info_map.end())
		{
			ActivityInfo const info
			(	seconds,
				interval.beginning(),
				interval.ending()
			);
			m_activity_info_map.insert(make_pair(activity, info));
		}
		else
		{
			auto& info = it->second;
			auto& accum = info.seconds;
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
			assert (interval.beginning() >= info.beginning);
			assert (interval.ending() >= info.ending);
			info.ending = interval.ending();
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
	do_write_summary(p_os, m_activity_info_map);
	m_activity_info_map.clear();  // hygienic even if unnecessary
	return;
}

SummaryReportWriter::ActivityInfo::ActivityInfo
(	unsigned long long p_seconds,
	TimePoint const& p_beginning,
	TimePoint const& p_ending,
	unsigned int p_num_children
):
	seconds(p_seconds),
	num_children(p_num_children),
	beginning(p_beginning),
	ending(p_ending)
{
}

}  // namespace swx
