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

#ifndef GUARD_human_list_report_writer_hpp_40288006812468175
#define GUARD_human_list_report_writer_hpp_40288006812468175

#include "list_report_writer.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

// begin forward declarations

class Stint;

// end forward declarations

class HumanListReportWriter: public ListReportWriter
{
// special member functions
public:
	HumanListReportWriter(std::vector<Stint> const& p_stints);
	HumanListReportWriter(HumanListReportWriter const& rhs) = delete;
	HumanListReportWriter(HumanListReportWriter&& rhs) = delete;
	HumanListReportWriter& operator=(HumanListReportWriter const& rhs) = delete;
	HumanListReportWriter& operator=(HumanListReportWriter&& rhs) = delete;
	virtual ~HumanListReportWriter();

// inherited virtual functions
private:
	virtual void
		do_process_stint(std::ostream& p_os, Stint const& p_stint) override;

};  // class HumanListReportWriter

}  // namespace swx

#endif  // GUARD_human_list_report_writer_hpp_40288006812468175
