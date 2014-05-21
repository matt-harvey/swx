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

#ifndef GUARD_list_report_writer_hpp_6743347196254575
#define GUARD_list_report_writer_hpp_6743347196254575

#include "report_writer.hpp"
#include "stint.hpp"
#include <vector>

namespace swx
{

class ListReportWriter: public ReportWriter
{
// special member functions
public:
	explicit ListReportWriter(std::vector<Stint> const& p_stints);
	ListReportWriter(ListReportWriter const& rhs) = delete;
	ListReportWriter(ListReportWriter&& rhs) = delete;
	ListReportWriter& operator=(ListReportWriter const& rhs) = delete;
	ListReportWriter& operator=(ListReportWriter&& rhs) = delete;
	virtual ~ListReportWriter();

};  // class ListReportWriter

}  // namespace swx

#endif  // GUARD_list_report_writer_hpp_6743347196254575
