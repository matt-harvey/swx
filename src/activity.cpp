/*
 * Copyright 2014 Matthew Harvey
 */

#include "activity.hpp"
#include "interval.hpp"
#include <cassert>
#include <string>
#include <vector>

using std::string;

namespace swx
{

Activity::Activity(string const& p_name): m_name(p_name)
{
	assert (m_stints.empty());
}

void
Activity::push_stint(Interval const& p_stint)
{
	m_stints.push_back(p_stint);
	return;
}

}  // namespace swx
