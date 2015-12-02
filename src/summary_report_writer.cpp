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
#include "activity_stats.hpp"
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
#include <vector>

using std::map;
using std::ostringstream;
using std::ostream;
using std::runtime_error;
using std::string;
using std::vector;

namespace swx
{

SummaryReportWriter::SummaryReportWriter
(   vector<Stint> const& p_stints,
    Options const& p_options,
    Flags::Type p_flags
):
    ReportWriter(p_stints, p_options),
    m_flags(p_flags)
{
    assert (m_activity_stats_map.empty());
}

SummaryReportWriter::~SummaryReportWriter() = default;

void
SummaryReportWriter::do_preprocess_stints
(   ostream& p_os,
    vector<Stint> const& p_stints
)
{
    (void)p_os; (void)p_stints;  // silence compiler warnings re. unused params.
    assert (m_activity_stats_map.empty());
}

void
SummaryReportWriter::do_process_stint(ostream& p_os, Stint const& p_stint)
{
    (void)p_os;  // silence compiler warning re. unused param.
    auto const interval = p_stint.interval();
    unsigned long long const seconds = interval.duration().count();
    auto const& activity = p_stint.activity();
    if (!activity.empty())
    {
        m_activity_stats_map[activity] +=
            ActivityStats(seconds, interval.beginning(), interval.ending());
    }
}

void
SummaryReportWriter::do_postprocess_stints
(   ostream& p_os,
    vector<Stint> const& p_stints
)
{
    (void)p_stints;  // silence compiler warning re. unused param.
    do_write_summary(p_os, m_activity_stats_map);
    m_activity_stats_map.clear();  // hygienic even if unnecessary
}

bool
SummaryReportWriter::has_flag(Flags::Type p_flag) const
{
    return m_flags & p_flag;
}

}  // namespace swx
