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

#ifndef GUARD_date_hpp_3886306613630758
#define GUARD_date_hpp_3886306613630758

namespace swx
{

class Date
{
// static factory functions
public:
	static Date today();

// special member functions
public:

	Date
	(	unsigned int p_year,          // years since 1900
		unsigned char p_month,        // 0-11
		unsigned char p_day_of_month  // 1-31
	);

	Date(Date const& rhs) = default;
	Date(Date&& rhs) = default;
	Date& operator=(Date const& rhs) = default;
	Date& operator=(Date&& rhs) = default;
	~Date() = default;

// getters
public:
	unsigned int year() const;           // years since 1900
	unsigned char month() const;         // 0-11
	unsigned char day_of_month() const;  // 1-31

// operators
public:
	// increment by one day
	Date const& operator++();        // prefix
	Date operator++(int);            // postfix

// member variables
private:
	unsigned char m_day_of_month;   // 1-31
	unsigned char m_month;          // 0-11
	unsigned int m_year;            // years since 1900

};  // class Date

}  // namespace swx

#endif  // GUARD_date_hpp_3886306613630758
