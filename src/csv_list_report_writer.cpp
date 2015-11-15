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

#include "csv_list_report_writer.hpp"
#include "csv_row.hpp"
#include "stint.hpp"
#include "stream_flag_guard.hpp"
#include "time_point.hpp"
#include <iomanip>
#include <ostream>
#include <vector>

using std::setprecision;
using std::ostream;
using std::vector;

namespace swx
{

CsvListReportWriter::CsvListReportWriter
(   vector<Stint> const& p_stints,
    Options const& p_options
):
    ListReportWriter(p_stints, p_options)
{
}

CsvListReportWriter::~CsvListReportWriter() = default;

void
CsvListReportWriter::do_process_stint(ostream& p_os, Stint const& p_stint)
{
    if (!p_stint.activity().empty())
    {
        StreamFlagGuard guard(p_os);
        p_os << setprecision(output_precision());
        auto const interval = p_stint.interval();
        CsvRow row;
        row << time_point_to_stamp(interval.beginning(), time_format(), formatted_buf_len())
            << time_point_to_stamp(interval.ending(), time_format(), formatted_buf_len())
            << round_hours(interval)
            << p_stint.activity();
        p_os << row;
    }
}

}  // namespace swx
