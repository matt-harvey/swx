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
using std::vector;

namespace swx
{

ActivityNode::ActivityNode(string const& p_activity, unsigned int p_level):
	m_level(p_level),
	m_components(split(p_activity, ' '))
{
}

ActivityNode::ActivityNode
(	vector<string>::const_iterator const& p_begin,
	vector<string>::const_iterator const& p_end,
	unsigned int p_level
):
	m_level(p_level),
	m_components(p_begin, p_end)
{
}

bool
ActivityNode::operator<(ActivityNode const& rhs) const
{
	if (is_ancestor_descendant(*this, rhs) && (level() != rhs.level()))
	{
		return level() > rhs.level();
	}
	else
	{
		return activity() < rhs.activity();  // TODO inefficient
	}
}

string
ActivityNode::activity() const
{
	return squish(m_components.begin(), m_components.end());
}

unsigned int
ActivityNode::level() const
{
	return m_level;
}

unsigned int
ActivityNode::num_components() const
{
	return m_components.size();
}

void
ActivityNode::set_num_components(unsigned int p_num_components)
{
	auto const old_num_components = num_components();
	while (num_components() < p_num_components)
	{
		m_components.push_back(".");  // TODO This is a brittle hack.
	}
	return;
}

ActivityNode
ActivityNode::parent() const
{
	auto const parent_level = level() + 1;
	switch (num_components())
	{
	case 0:
		return ActivityNode("", parent_level);
	case 1:
		return ActivityNode(m_components[0], parent_level);
	default:
		assert (m_components.size() > 1);
		return ActivityNode(m_components.begin(), m_components.end() - 1, parent_level);
	}
}

string
ActivityNode::marginal_name() const
{
	assert (m_components.size() > 0);  // TODO are we really guaranteed this?
	return m_components.back();
}

bool
ActivityNode::is_ancestor_descendant
(	ActivityNode const& lhs,
	ActivityNode const& rhs
)
{
	auto lit = lhs.m_components.begin();
	auto const lend = lhs.m_components.end();
	auto rit = rhs.m_components.begin();
	auto const rend = rhs.m_components.end();
	for ( ; (lit != lend) && (rit != rend); ++lit, ++rit)
	{
		if (*lit != *rit)
		{
			return false;
		}
	}
	return true;
}

}  // namespace swx
