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

#ifndef GUARD_summary_report_writer_hpp_7957524563166092
#define GUARD_summary_report_writer_hpp_7957524563166092

#include "report_writer.hpp"
#include "seconds.hpp"
#include "stint.hpp"
#include <map>
#include <ostream>
#include <vector>

namespace swx
{

class SummaryReportWriter: public ReportWriter
{
// special member functions
public:
	explicit SummaryReportWriter(std::vector<Stint> const& p_stints);
	SummaryReportWriter(SummaryReportWriter const& rhs) = delete;
	SummaryReportWriter(SummaryReportWriter&& rhs) = delete;
	SummaryReportWriter& operator=(SummaryReportWriter const& rhs) = delete;
	SummaryReportWriter& operator=(SummaryReportWriter&& rhs) = delete;
	virtual ~SummaryReportWriter();

// inherited virtual member functions
private:
	virtual void do_post_write
	(	std::ostream& p_os,
		std::vector<Stint> const& p_stints
	) override;

	virtual void do_write_activity_hours
	(	std::ostream& p_os,
		std::string const& p_activity,
		double p_rounded_hours
	) = 0;

};  // class SummaryReportWriter

}  // namespace swx

#endif  // GUARD_summary_report_writer_hpp_7957524563166092
