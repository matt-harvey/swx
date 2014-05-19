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
#include "stint.hpp"
#include <ostream>
#include <vector>

using std::ostream;
using std::vector;

namespace swx
{

ReportWriter::ReportWriter()
{
}

ReportWriter::~ReportWriter()
{
}

void
ReportWriter::write(ostream& p_os, vector<Stint> const& p_stints)
{
		do_pre_write(p_os, p_stints);
		for (auto const& stint: p_stint)
		{
			do_write(p_os, stint);
		}
		do_post_write(p_os, p_stints);
}

void
ReportWriter::do_pre_write(ostream& p_os, vector<Stint> const& p_stints)
{
	(void)p_os; (void)p_stints;  // silence compiler re. unused param.
	return;
}

void
ReportWriter::do_post_write(ostream& p_os, vector<Stint> const& p_stints)
{
	(void)p_os; (void)p_stints;  // silence compiler re. unused param.
	return;
}

}  // namespace swx
