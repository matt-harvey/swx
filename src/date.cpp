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

#include "date.hpp"
#include "time_point.hpp"
#include <cassert>

using std::tm;

namespace swx
{

namespace
{
	template <typename L, typename R>
	bool divides_evenly(L x, R y)
	{
		return x % y == 0;
	}

	bool is_leap_year(unsigned int p_year)
	{
		if (divides_evenly(p_year, 4))
		{
			if (divides_evenly(p_year, 100))
			{
				return divides_evenly(p_year, 400);
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	bool days_in_month(unsigned char p_month, bool p_is_leap_year)
	{
		switch (p_month)
		{
		case 0: return 31;
		case 1: return (p_is_leap_year? 29: 28);
		case 2: return 31;
		case 3: return 30;
		case 4: return 31;
		case 5: return 30;
		case 6: return 31;
		case 7: return 31;
		case 8: return 30;
		case 9: return 31;
		case 10: return 30;
		case 11: return 31;
		default: assert(false);
		}
	}

	bool is_last_day_of_month(Date const& p_date)
	{
		return
			p_date.day_of_month() ==
			days_in_month(p_date.month(), is_leap_year(p_date.year()));
	}

	bool is_last_month_of_year(unsigned char p_month)
	{
		return p_month == 11;
	}
}

Date
Date::today()
{
	tm const time_tm = time_point_to_tm(now());
	return Date(time_tm.tm_year, time_tm.tm_mon, time_tm.tm_mday);
}

Date::Date
(	unsigned int p_year,
	unsigned char p_month,
	unsigned char p_day_of_month
):
	m_day_of_month(p_day_of_month),
	m_month(p_month),
	m_year(p_year)
{
}

unsigned int
Date::year() const
{
	return m_year;
}

unsigned char
Date::month() const
{
	return m_month;
}

unsigned char
Date::day_of_month() const
{
	return m_day_of_month;
}

Date const&
Date::operator++()
{
	if (is_last_day_of_month(*this))
	{
		m_day_of_month = 1;
		if (is_last_month_of_year(month()))
		{
			++m_year;
			m_month = 0;
		}
		else
		{
			++m_month;
		}
	}
	else
	{
		++m_day_of_month;	
	}
	return *this;
}

Date
Date::operator++(int)
{
	Date const ret(*this);
	++*this;
	return ret;
}

}  // namespace swx
