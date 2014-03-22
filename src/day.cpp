/*
 * Copyright 2014 Matthew Harvey
 */

#include "day.hpp"

namespace swx
{

Day::Day(int p_year, int p_yday):
	m_year(p_year),
	m_yday(p_yday)
{
}

int
Day::year() const
{
	return m_year;
}

int
Day::yday() const
{
	return m_yday;
}

bool
operator==(Day lhs, Day rhs)
{
	return
		(lhs.year() == rhs.year()) &&
		(rhs.yday() == rhs.yday());
}

bool
operator!=(Day lhs, Day rhs)
{
	return !(lhs == rhs);
}

bool
operator<(Day lhs, Day rhs)
{
	return
		(lhs.year() < rhs.year()) ||
		((lhs.year() == rhs.year()) && (lhs.yday() < rhs.yday()));
}

bool
operator>(Day lhs, Day rhs)
{
	return rhs < lhs;
}

bool
operator<=(Day lhs, Day rhs)
{
	return (lhs == rhs) || (lhs < rhs);
}

bool
operator>=(Day lhs, Day rhs)
{
	return (lhs == rhs) || (lhs > rhs);
}

}  // namespace swx
