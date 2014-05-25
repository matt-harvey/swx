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
#include "csv_utilities.hpp"
#include "stint.hpp"
#include "time_point.hpp"
#include <ostream>
#include <vector>

using std::ostream;
using std::vector;

namespace swx
{

CsvListReportWriter::CsvListReportWriter(vector<Stint> const& p_stints):
	ListReportWriter(p_stints)
{
}

CsvListReportWriter::~CsvListReportWriter() = default;

void
CsvListReportWriter::do_process_stint(ostream& p_os, Stint const& p_stint)
{
	if (!p_stint.activity().empty())
	{
		auto const interval = p_stint.interval();
		output_csv_row
		(	p_os,
			time_point_to_stamp(interval.beginning()),
			time_point_to_stamp(interval.ending()),
			round_hours(interval),
			p_stint.activity()
		);
	}
}

}  // namespace swx
