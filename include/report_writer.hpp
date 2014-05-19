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

#ifndef GUARD_report_writer_hpp_6461996848910114
#define GUARD_report_writer_hpp_6461996848910114

#include "stint.hpp"
#include <ostream>
#include <vector>

namespace swx
{

class ReportWriter
{
// special member functions
protected:
	ReportWriter();
	ReportWriter(ReportWriter const& rhs) = delete;
	ReportWriter(ReportWriter&& rhs) = delete;
	ReportWriter& operator=(ReportWriter const& rhs) = delete;
	ReportWriter& operator=(ReportWriter&& rhs) = delete;
	virtual ~ReportWriter();

// ordinary member functions
public:
	void write(std::ostream& p_os, std::vector<Stint> const& p_stints);

// virtual member functions
private:
	virtual void
		do_pre_write(std::ostream& p_os, std::vector<Stint> const& p_stints);
	
	virtual void
		do_write(std::ostream& p_os, Stint const& p_stint) = 0;

	virtual void
		do_post_write(std::ostream& p_os, std::vector<Stint> const& p_stints);

};  // class ReportWriter

}  // namespace swx

#endif  // GUARD_report_writer_hpp_6461996848910114
