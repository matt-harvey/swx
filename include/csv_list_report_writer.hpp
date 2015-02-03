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

#ifndef GUARD_csv_list_report_writer_hpp_7141246202933251
#define GUARD_csv_list_report_writer_hpp_7141246202933251

#include "list_report_writer.hpp"
#include "stint.hpp"
#include <ostream>
#include <vector>

namespace swx
{

class CsvListReportWriter: public ListReportWriter
{
// special member functions
public:
	CsvListReportWriter
	(	std::vector<Stint> const& p_stints,
		Options const& p_options
	);
	CsvListReportWriter(CsvListReportWriter const& rhs) = delete;
	CsvListReportWriter(CsvListReportWriter&& rhs) = delete;
	CsvListReportWriter& operator=(CsvListReportWriter const& rhs) = delete;
	CsvListReportWriter& operator=(CsvListReportWriter&& rhs) = delete;
	virtual ~CsvListReportWriter();

// inherited virtual member functions
private:
	virtual void
		do_process_stint (std::ostream& p_os, Stint const& p_stint) override;

};  // class CsvListReportWriter

}  // namespace swx

#endif  // GUARD_csv_list_report_writer_hpp_7141246202933251
