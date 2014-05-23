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

#include <ostream>
#include <vector>
#include "stint.hpp"

namespace swx
{

// begin forward declarations

class Interval;

// end forward declarations

class ReportWriter
{
// special member functions
protected:
	explicit ReportWriter(std::vector<Stint> const& p_stints);
	ReportWriter(ReportWriter const& rhs) = delete;
	ReportWriter(ReportWriter&& rhs) = delete;
	ReportWriter& operator=(ReportWriter const& rhs) = delete;
	ReportWriter& operator=(ReportWriter&& rhs) = delete;
	virtual ~ReportWriter();

// ordinary member functions
public:
	void write(std::ostream& p_os);

protected:

	/**
	 * @returns duration of \e p_interval rounded according to the rounding
	 * behaviour specified in the configuration file.
	 */
	double round_hours(Interval const& p_interval) const;

	/**
	 * Converts a number of seconds to a double representing the number
	 * of hours, rounded according to the rounding behaviour specified in
	 * the configuration file.
	 */
	double seconds_to_rounded_hours(unsigned long long p_seconds) const;

// virtual member functions
private:
	virtual void do_preprocess_stints
	(	std::ostream& p_os,
		std::vector<Stint> const& p_stints
	);
	
	virtual void do_process_stint(std::ostream& p_os, Stint const& p_stint) = 0;

	virtual void do_postprocess_stints
	(	std::ostream& p_os,
		std::vector<Stint> const& p_stints
	);

// member variables
private:
	unsigned int m_output_rounding_numerator;
	unsigned int m_output_rounding_denominator;
	std::vector<Stint> const& m_stints;

};  // class ReportWriter

}  // namespace swx

#endif  // GUARD_report_writer_hpp_6461996848910114
