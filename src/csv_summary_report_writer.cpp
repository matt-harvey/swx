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

#include "csv_summary_report_writer.hpp"
#include "activity_stats.hpp"
#include "csv_row.hpp"
#include "stint.hpp"
#include "summary_report_writer.hpp"
#include "time_point.hpp"
#include <map>
#include <ostream>
#include <string>
#include <vector>

using std::map;
using std::ostream;
using std::string;
using std::vector;

namespace swx
{

CsvSummaryReportWriter::CsvSummaryReportWriter
(   vector<Stint> const& p_stints,
    Options const& p_options,
    Flags::Type p_flags
):
    SummaryReportWriter(p_stints, p_options, p_flags)
{
}

CsvSummaryReportWriter::~CsvSummaryReportWriter() = default;

void
CsvSummaryReportWriter::do_write_summary
(   ostream& p_os,
    map<string, ActivityStats> const& p_activity_stats_map
)
{
    auto const add_time_info = [this](CsvRow& row, ActivityStats const& info)
    {
        row << seconds_to_rounded_hours(info.seconds);
        if (has_flag(Flags::include_beginning))
        {
            row << time_point_to_stamp(info.beginning, time_format(), formatted_buf_len());
        }
        if (has_flag(Flags::include_ending))
        {
            row << time_point_to_stamp(info.ending, time_format(), formatted_buf_len());
        }
    };

    if (has_flag(Flags::succinct))
    {
        ActivityStats total_info;
        for (auto const& pair: p_activity_stats_map) total_info += pair.second;
        CsvRow row;
        add_time_info(row, total_info);
        p_os << row;
    }
    else
    {
        for (auto const& pair: p_activity_stats_map)
        {
            auto const& activity = pair.first;
            auto const& info = pair.second;
            CsvRow row;
            row << activity;
            add_time_info(row, info);
            p_os << row;
        }
    }
}

}  // namespace swx
