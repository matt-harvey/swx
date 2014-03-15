/*
 * Copyright 2014 Matthew Harvey
 */

#include "activity.hpp"
#include "interval.hpp"
#include <jewel/exception.hpp>
#include <string>

using std::string;

namespace swx
{

Activity::Activity(string const& p_name, string const& p_description):
	m_name(p_name),
	m_description(p_description)
{
	JEWEL_ASSERT (m_stints.empty());
	JEWEL_ASSERT (is_valid());
}

void
Activity::push_stint(Interval const& p_stint)
{
	auto const& previous = m_stints.back();
	if (is_open(previous))
	{
		JEWEL_ASSERT (is_valid());
		JEWEL_THROW
		(	UnclosedStintException,
			"Previous stint for this Activity is not yet closed."
		);
	}
	JEWEL_ASSERT (previous.maybe_ending());
	if (p_stint.beginning() < *(previous.maybe_ending()))
	{
		JEWEL_ASSERT (is_valid());
		JEWEL_THROW
		(	StintOrderException,
			"Cannot push stint that starts before the previous stint ends."
		);
	}
	m_stints.push_back(p_stint);
	JEWEL_ASSERT (is_valid());
	return;
}

bool
Activity::is_valid() const
{
	if (m_stints.empty())
	{
		return true;
	}
	JEWEL_ASSERT (m_stints.size() >= 1);
	typedef decltype(m_stints)::size_type Sz;
	Sz const target = m_stints.size() - 1;
	for (Sz i = 0; i != target; ++i)
	{
		auto const& stint = m_stints[i];
		if (is_open(stint))
		{
			return false;
		}
		JEWEL_ASSERT (stint.maybe_ending());
		JEWEL_ASSERT (i + 1 < m_stints.size());
		if (*(stint.maybe_ending()) > m_stints[i + 1].beginning())
		{
			return false;
		}
	}
	return true;
}

}  // namespace swx
