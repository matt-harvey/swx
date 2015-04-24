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

#include "human_list_report_writer.hpp"
#include "config.hpp"
#include "interval.hpp"
#include "stint.hpp"
#include "stream_flag_guard.hpp"
#include "time_point.hpp"
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::endl;
using std::fixed;
using std::ostream;
using std::right;
using std::setprecision;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

HumanListReportWriter::HumanListReportWriter
(   vector<Stint> const& p_stints,
    Options const& p_options
):
    ListReportWriter(p_stints, p_options)
{
}

HumanListReportWriter::~HumanListReportWriter() = default;

void
HumanListReportWriter::do_process_stint(ostream& p_os, Stint const& p_stint)
{
    auto const activity = p_stint.activity();
    if (activity.empty())
    {
        p_os << endl;
    }
    else
    {
        StreamFlagGuard guard(p_os);
        auto const interval = p_stint.interval();
        p_os << time_point_to_stamp(interval.beginning(), time_format(), formatted_buf_len()) << "  ";
        p_os << time_point_to_stamp(interval.ending(), time_format(), formatted_buf_len()) << "  ";
        p_os << fixed
             << setprecision(output_precision())
             << right
             << setw(output_width())
             << round_hours(interval)
             << "  ";
        guard.reset();
        p_os << p_stint.activity() << endl;
    }
    return;
}

}  // namespace swx
