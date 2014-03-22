/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_day_hpp_7447718948799986
#define GUARD_day_hpp_7447718948799986

namespace swx
{

class Day
{
public:
// special member functions
	Day(int p_year, int p_yday);

// ordinary member functions
public:
	int year() const;
	int yday() const;

// member variables
private:
	int m_year;
	int m_yday;

};  // class Day

bool operator==(Day lhs, Day rhs);
bool operator!=(Day lhs, Day rhs);
bool operator<(Day lhs, Day rhs);
bool operator>(Day lhs, Day rhs);
bool operator<=(Day lhs, Day rhs);
bool operator>=(Day lhs, Day rhs);

}  // namespace swx

#endif  // GUARD_day_hpp_7447718948799986
