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

ActivityNode::ActivityNode(string const& p_activity):
	m_components(split(p_activity, ' '))
{
}

ActivityNode::ActivityNode
(	vector<string>::const_iterator const& p_begin,
	vector<string>::const_iterator const& p_end
):
	m_components(p_begin, p_end)
{
}

bool
ActivityNode::operator<(ActivityNode const& rhs) const
{
	if (is_ancestor_descendant(*this, rhs) && (num_components() != rhs.num_components()))
	{
		return num_components() < rhs.num_components();
	}
	else
	{
		return m_components < rhs.m_components;
	}
}

string
ActivityNode::activity() const
{
	return squish(m_components.begin(), m_components.end());
}

string
ActivityNode::marginal_name() const
{
	return m_components.empty() ? "" : m_components.back();
}

unsigned int
ActivityNode::num_components() const
{
	return m_components.size();
}

ActivityNode
ActivityNode::parent() const
{
	assert (!m_components.empty());
	return ActivityNode(m_components.begin(), m_components.end() - 1);
}

void
ActivityNode::set_num_components(unsigned int p_num_components)
{
	auto const old_num_components = num_components();
	while (num_components() < p_num_components)
	{
		m_components.push_back("");
	}
	return;
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
