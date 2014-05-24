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
#include "csv_utilities.hpp"
#include "stint.hpp"
#include "summary_report_writer.hpp"
#include <ostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

namespace swx
{

CsvSummaryReportWriter::CsvSummaryReportWriter(vector<Stint> const& p_stints):
	SummaryReportWriter(p_stints)
{
}

CsvSummaryReportWriter::~CsvSummaryReportWriter()
{
}

void
CsvSummaryReportWriter::do_write_activity_hours
(	ostream& p_os,
	string const& p_activity,
	double p_rounded_hours
)
{
	output_csv_row(p_os, p_activity, p_rounded_hours);
	return;
}

}  // namespace swx
