/*
 * Copyright 2015 Matthew Harvey
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

#include "activity_node.hpp"
#include "string_utilities.hpp"
#include <cassert>
#include <string>

using std::string;

namespace swx
{

namespace  // begin anonymous namespace
{
	bool
	is_ancestor_descendant
	(	ActivityNode const& lhs,
		ActivityNode const& rhs
	)
	{
		// TODO LOW PRIORITY This could be more efficient. We could probably
		// cache the components vector or something.
		auto const lhs_components = split(lhs.activity(), ' ');
		auto const rhs_components = split(rhs.activity(), ' ');
		decltype(lhs_components)::const_iterator lit = lhs_components.begin();
		decltype(lhs_components)::const_iterator lend = lhs_components.end();
		decltype(rhs_components)::const_iterator rit = rhs_components.begin();
		decltype(rhs_components)::const_iterator rend = rhs_components.end();
		for ( ; (lit != lend) && (rit != rend); ++lit, ++rit)
		{
			if (*lit != *rit)
			{
				return false;
			}
		}
		return true;
	}

}  // end anonymous namespace


ActivityNode::ActivityNode(string const& p_activity, unsigned int p_level):
	m_level(p_level),
	m_activity(p_activity)
{
}

string const&
ActivityNode::activity() const
{
	return m_activity;
}

unsigned int
ActivityNode::level() const
{
	return m_level;
}

unsigned int
ActivityNode::num_components() const
{
	// TODO This is inefficient
	return split(activity(), ' ').size();
}

void
ActivityNode::set_num_components(unsigned int p_num_components)
{
	auto const old_num_components = num_components();
	if (p_num_components > old_num_components)
	{
		for (unsigned int i = old_num_components; i != p_num_components; ++i)
		{
			m_activity += " .";  // TODO This is a brittle hack.
		}
	}
	return;
}

ActivityNode
ActivityNode::parent() const
{
	auto const components = split(activity(), ' ');
	auto const parent_level = level() + 1;
	if (components.size() == 0)
	{
		return ActivityNode("", parent_level);
	}
	if (components.size() == 1)
	{
		return ActivityNode(components[0], parent_level);
	}
	assert (components.size() > 1);
	auto const parent_activity = squish(components.begin(), components.end() - 1);
	return ActivityNode(parent_activity, parent_level);
}

string
ActivityNode::marginal_name() const
{
	return split(activity(), ' ').back();
}

bool
operator<(ActivityNode const& lhs, ActivityNode const& rhs)
{
	if (is_ancestor_descendant(lhs, rhs) && (lhs.level() != rhs.level()))
	{
		return lhs.level() > rhs.level();
	}
	else
	{
		return lhs.activity() < rhs.activity();
	}
}

}  // namespace swx
