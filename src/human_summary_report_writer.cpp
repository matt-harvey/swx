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
#include "arithmetic.hpp"
#include "stint.hpp"
#include "stream_flag_guard.hpp"
#include "summary_report_writer.hpp"
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
using std::right;
using std::runtime_error;
using std::setprecision;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

HumanSummaryReportWriter::HumanSummaryReportWriter(vector<Stint> const& p_stints):
	SummaryReportWriter(p_stints)
{
}

HumanSummaryReportWriter::~HumanSummaryReportWriter() = default;

void
HumanSummaryReportWriter::do_write_summary
(	ostream& p_os,
	map<string, unsigned long long> const& p_activity_seconds_map
)
{
	string::size_type left_col_width = 0;
	for (auto const& pair: p_activity_seconds_map)
	{
		string const& activity = pair.first;
		auto const activity_width = activity.length();
		if (activity_width > left_col_width) left_col_width = activity_width;
	}
	unsigned long long total_seconds = 0;
	for (auto const& pair: p_activity_seconds_map)
	{
		unsigned long long const seconds = pair.second;
		if (!addition_is_safe(total_seconds, seconds))
		{
			throw runtime_error
			(	"Time spent on activities is too great to be totalled "
					"safely."
			);
		}
		assert (addition_is_safe(total_seconds, seconds));
		total_seconds += seconds;
		string const& activity = pair.first;
		print_label_and_rounded_hours(p_os, activity, seconds, left_col_width);
	}
	p_os << endl;
	print_label_and_rounded_hours(p_os, "TOTAL", total_seconds, left_col_width);
	return;
}

void
HumanSummaryReportWriter::print_label_and_rounded_hours
(	ostream& p_os,
	string const& p_label,
	unsigned long long p_seconds,
	unsigned int p_left_col_width
) const
{
	StreamFlagGuard guard(p_os);
	p_os << left << setw(p_left_col_width) << p_label << ' ';
	p_os << fixed
	     << setprecision(output_precision())
		 << right
		 << setw(output_width())
		 << seconds_to_rounded_hours(p_seconds)
		 << endl;
	return;
}

}  // namespace swx
