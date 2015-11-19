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

#include "human_summary_report_writer.hpp"
#include "activity_stats.hpp"
#include "activity_node.hpp"
#include "activity_tree.hpp"
#include "arithmetic.hpp"
#include "stint.hpp"
#include "stream_flag_guard.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "summary_report_writer.hpp"
#include "time_point.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::endl;
using std::fixed;
using std::left;
using std::map;
using std::ostream;
using std::ostringstream;
using std::right;
using std::runtime_error;
using std::setprecision;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

HumanSummaryReportWriter::HumanSummaryReportWriter
(   vector<Stint> const& p_stints,
    Options const& p_options,
    Flags::Type p_flags
):
    SummaryReportWriter(p_stints, p_options, p_flags)
{
}

HumanSummaryReportWriter::~HumanSummaryReportWriter() = default;

void
HumanSummaryReportWriter::do_write_summary
(   ostream& p_os,
    map<std::string, ActivityStats> const& p_activity_stats_map
)
{
    if (has_flag(Flags::succinct)) write_succinct_summary(p_os, p_activity_stats_map);
    else if (has_flag(Flags::verbose)) write_flat_summary(p_os, p_activity_stats_map);
    else write_tree_summary(p_os, p_activity_stats_map);
}

void
HumanSummaryReportWriter::print_label_and_rounded_hours
(   ostream& p_os,
    string const& p_label,
    unsigned long long p_seconds,
    TimePoint const* p_beginning,
    TimePoint const* p_ending,
    unsigned int p_left_col_width
) const
{
    StreamFlagGuard guard(p_os);
    if (!p_label.empty())
    {
        p_os << left << setw(p_left_col_width) << p_label << ' '
             << right << setw(output_width());
    }
    p_os << fixed << setprecision(output_precision())
         << seconds_to_rounded_hours(p_seconds);
    guard.reset();

    if (p_beginning != nullptr)
    {
        p_os << "    "
             << time_point_to_stamp(*p_beginning, time_format(), formatted_buf_len());
    }
    if (p_ending != nullptr)
    {
        p_os << "    "
             << time_point_to_stamp(*p_ending, time_format(), formatted_buf_len());
    }
    p_os << endl;
}

void
HumanSummaryReportWriter::write_succinct_summary
(   ostream& p_os,
    map<std::string, ActivityStats> const& p_activity_stats_map
)
{
    ActivityStats total_info;
    for (auto const& pair: p_activity_stats_map) total_info += pair.second;
    print_label_and_rounded_hours
    (   p_os,
        string(),
        total_info.seconds,
        (has_flag(Flags::include_beginning) ?  &(total_info.beginning) : nullptr),
        (has_flag(Flags::include_ending) ? &(total_info.ending) : nullptr)
    );
}

void
HumanSummaryReportWriter::write_flat_summary
(   ostream& p_os,
    map<std::string, ActivityStats> const& p_activity_stats_map
)
{
    string::size_type left_col_width = 0;
    for (auto const& pair: p_activity_stats_map)
    {
        auto const& activity = pair.first;
        auto const activity_width = activity.length();
        if (activity_width > left_col_width) left_col_width = activity_width;
    }
    ActivityStats total_info;
    auto const include_beginning = has_flag(Flags::include_beginning);
    auto const include_ending = has_flag(Flags::include_ending);
    for (auto const& pair: p_activity_stats_map)
    {
        auto const& activity = pair.first;
        auto const& info = pair.second;
        total_info += info;
        print_label_and_rounded_hours
        (   p_os,
            activity,
            info.seconds,
            (include_beginning ? &(info.beginning) : nullptr),
            (include_ending ? &(info.ending) : nullptr),
            left_col_width
        );
    }
    p_os << endl;
    print_label_and_rounded_hours
    (   p_os,
        "TOTAL",
        total_info.seconds,
        (include_beginning ? &(total_info.beginning) : nullptr),
        (include_ending ? &(total_info.ending) : nullptr),
        left_col_width
    );
}

void
HumanSummaryReportWriter::write_tree_summary
(   ostream& p_os,
    map<std::string, ActivityStats> const& p_activity_stats_map
)
{
    if (p_activity_stats_map.empty())
    {
        p_os << endl;
        return;
    }
    ActivityTree const tree(p_activity_stats_map);
    ActivityTree::PrintNode const print_node = [this]
    (   ostream& p_ostream,
        unsigned int p_node_depth,
        string const& p_node_label,
        ActivityStats const& p_stats
    )
    {
        StreamFlagGuard guard(p_ostream);
        p_ostream << string(p_node_depth * (output_width() + 4), ' ')
                  << "[ "
                  << fixed << setprecision(output_precision())
                  << right << setw(output_width())
                  << seconds_to_rounded_hours(p_stats.seconds)
                  << " ]";
        guard.reset();
        if (has_flag(Flags::include_beginning))
        {
            auto const& b = p_stats.beginning;
            auto const s = time_point_to_stamp(b, time_format(), formatted_buf_len());
            p_ostream << "[ " << s << " ]";
        }
        if (has_flag(Flags::include_ending))
        {
            auto const& e = p_stats.ending;
            auto const s = time_point_to_stamp(e, time_format(), formatted_buf_len());
            p_ostream << "[ " << s << " ]";
        }
        p_ostream << ' ' << p_node_label << endl;
    };
    tree.print(p_os, print_node);
}

}  // namespace swx
