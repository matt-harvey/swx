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
#include "config.hpp"
#include "interval.hpp"
#include "round.hpp"
#include "stint.hpp"
#include <ostream>
#include <vector>

using std::ostream;
using std::vector;

namespace swx
{

ReportWriter::ReportWriter(vector<Stint> const& p_stints):
	m_output_rounding_numerator(Config::output_rounding_numerator()),
	m_output_rounding_denominator(Config::output_rounding_denominator()),
	m_stints(p_stints)
{
}

ReportWriter::~ReportWriter() = default;

void
ReportWriter::write(ostream& p_os)
{
		do_preprocess_stints(p_os, m_stints);
		for (auto const& stint: m_stints) do_process_stint(p_os, stint);
		do_postprocess_stints(p_os, m_stints);
}

double
ReportWriter::round_hours(Interval const& p_interval) const
{
	return seconds_to_rounded_hours(p_interval.duration().count());
}

double
ReportWriter::seconds_to_rounded_hours(unsigned long long p_seconds) const
{
	return round
	(	p_seconds / 60.0 / 60.0,
		m_output_rounding_numerator,
		m_output_rounding_denominator
	);
}

void
ReportWriter::do_preprocess_stints(ostream& p_os, vector<Stint> const& p_stints)
{
	(void)p_os; (void)p_stints;  // silence compiler re. unused param.
	return;
}

void
ReportWriter::do_postprocess_stints(ostream& p_os, vector<Stint> const& p_stints)
{
	(void)p_os; (void)p_stints;  // silence compiler re. unused param.
	return;
}

}  // namespace swx
