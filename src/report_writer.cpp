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

#include "report_writer.hpp"
#include "arithmetic.hpp"
#include "config.hpp"
#include "interval.hpp"
#include "stint.hpp"
#include <ostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

namespace swx
{

ReportWriter::ReportWriter
(	vector<Stint> const& p_stints,
	Options const& p_options
):
	m_options(p_options),
	m_stints(p_stints)
{
}

ReportWriter::~ReportWriter() = default;

unsigned int
ReportWriter::output_precision() const
{
	return m_options.output_precision;
}

unsigned int
ReportWriter::output_width() const
{
	return m_options.output_width;
}

string const&
ReportWriter::time_format() const
{
	return m_options.time_format;
}

unsigned int
ReportWriter::formatted_buf_len() const
{
	return m_options.formatted_buf_len;
}

double
ReportWriter::seconds_to_rounded_hours(unsigned long long p_seconds) const
{
	return round
	(	p_seconds / 60.0 / 60.0,
		m_options.output_rounding_numerator,
		m_options.output_rounding_denominator
	);
}

double
ReportWriter::round_hours(Interval const& p_interval) const
{
	return seconds_to_rounded_hours(p_interval.duration().count());
}

void
ReportWriter::write(ostream& p_os)
{
		do_preprocess_stints(p_os, m_stints);
		for (auto const& stint: m_stints) do_process_stint(p_os, stint);
		do_postprocess_stints(p_os, m_stints);
}

void
ReportWriter::do_preprocess_stints(ostream& p_os, vector<Stint> const& p_stints)
{
	(void)p_os; (void)p_stints;  // silence compiler re. unused params.
	return;
}

void
ReportWriter::do_postprocess_stints
(	ostream& p_os,
	vector<Stint> const& p_stints
)
{
	(void)p_os; (void)p_stints;  // silence compiler re. unused params.
	return;
}

ReportWriter::Options::Options
(	unsigned int p_output_rounding_numerator,
	unsigned int p_output_rounding_denominator,
	unsigned int p_output_precision,
	unsigned int p_output_width,
	unsigned int p_formatted_buf_len,
	string const& p_time_format
):
	output_rounding_numerator(p_output_rounding_numerator),
	output_rounding_denominator(p_output_rounding_denominator),
	output_precision(p_output_precision),
	output_width(p_output_width),
	formatted_buf_len(p_formatted_buf_len),
	time_format(p_time_format)
{
}

}  // namespace swx
