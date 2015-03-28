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

#include "activity_node.hpp"
#include "report_writer.hpp"
#include "stint.hpp"
#include "time_point.hpp"
#include <map>
#include <ostream>
#include <vector>

namespace swx
{

class SummaryReportWriter: public ReportWriter
{
// nested types
protected:

	/**
	 * Holds information about an activity for use in report.
	 * 
	 * \b seconds number of seconds spent on the activity
	 *
	 * \b beginning earliest time at which the activity was conducted during
	 * the reported period
	 *
	 * \b ending latest time at which activity was conducted during the
	 * reported period
	 *
	 * \b num_children number of children of node in activity tree
	 */
	struct ActivityInfo
	{
		ActivityInfo
		(	unsigned long long p_seconds,
			TimePoint const& p_beginning,
			TimePoint const& p_ending,
			unsigned int p_num_children = 0
		);
		unsigned long long seconds;
		unsigned int num_children;
		TimePoint beginning;
		TimePoint ending;

	};

// special member functions
public:
	SummaryReportWriter
	(	std::vector<Stint> const& p_stints,
		Options const& p_options
	);
	SummaryReportWriter(SummaryReportWriter const& rhs) = delete;
	SummaryReportWriter(SummaryReportWriter&& rhs) = delete;
	SummaryReportWriter& operator=(SummaryReportWriter const& rhs) = delete;
	SummaryReportWriter& operator=(SummaryReportWriter&& rhs) = delete;
	virtual ~SummaryReportWriter();

// inherited virtual member functions
private:
	virtual void do_preprocess_stints
	(	std::ostream& p_os,
		std::vector<Stint> const& p_stints
	) override;

	virtual void do_process_stint
	(	std::ostream& p_os,
		Stint const& p_stint
	) override;

	virtual void do_postprocess_stints
	(	std::ostream& p_os,
		std::vector<Stint> const& p_stints
	) override;

// other virtual member functions
private:
	virtual void do_write_summary
	(	std::ostream& p_os,
		std::map<ActivityNode, ActivityInfo> const& p_activity_info_map
	) = 0;

// member variables
private:
	std::map<ActivityNode, ActivityInfo> m_activity_info_map;

};  // class SummaryReportWriter

}  // namespace swx

#endif  // GUARD_summary_report_writer_hpp_7957524563166092
