/*
 * Copyright 2014 Matthew Harvey
 */

#include "activity.hpp"
#include "interval.hpp"
#include "seconds.hpp"
#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace swx
{

Activity::Activity(string const& p_name): m_name(p_name)
{
	assert (m_stints.empty());
}

string
Activity::name() const
{
	return m_name;
}

void
Activity::push_stint(Interval const& p_stint)
{
	m_stints.push_back(p_stint);
	return;
}

vector<Interval> const&
Activity::stints() const
{
	return m_stints;
}

Seconds
Activity::duration() const
{
	Seconds ret(0);
	for (auto const& stint: m_stints) ret += stint.duration();
	return ret;
}

}  // namespace swx
